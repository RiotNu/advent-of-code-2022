#include "pch.h"
#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/graphviz.hpp>

namespace Puzzle16A
{
	struct Path
	{
		int score = 0;
		int time = 0;
		int flow = 0;
		std::vector<int> nodeIndices;
		std::vector<bool> opened;

		int GetCurrentLocation() const { return nodeIndices.back(); }

		bool operator==(const Path&) const = default;
		auto operator<=>(const Path&) const = default;
	};
}

template<>
struct std::hash<Puzzle16A::Path>
{
	size_t operator()(const Puzzle16A::Path& value) const
	{
		auto result = 0ull;
		Utilities::HashCombine(result, value.score, value.time, value.flow);
		Utilities::HashRange(result, value.nodeIndices.begin(), value.nodeIndices.end());
		return result;
	}
};

namespace Puzzle16A
{
	using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::no_property, boost::property<boost::edge_weight_t, int>>;

	constexpr auto initialPosition = "AA";
	constexpr auto timeBudget = 30;

	// All nodes that have nonzero flow.
	auto nodesWithFlow = std::unordered_set<std::string>{};

	// Maps node name to flow value.
	auto nodeToFlow = std::unordered_map<std::string, int>{};

	// Maps node name to index in the graph.
	auto nodeToIndex = std::unordered_map<std::string, int>{};

	// Maps index in graph to node name.
	auto indexToNode = std::vector<std::string>{};

	Graph BuildGraphFromInput()
	{
		auto inputGraph = Graph{};
		for (const auto& line : Utilities::ReadAllLinesInFile("Puzzle16.input"))
		{
			auto tokens = Utilities::SplitString(line, "; ");
			auto [_, current, flow] = scn::scan_tuple<std::string, int>(tokens[0], "Valve {} has flow rate={}");

			auto connectedNodes = std::vector<std::string>{};
			if (tokens[1].starts_with("tunnels"))
			{
				auto nodes = Utilities::SplitString(tokens[1], "tunnels lead to valves ");
				connectedNodes = Utilities::SplitString(nodes[1], ", ");
			}
			else
			{
				connectedNodes.emplace_back(tokens[1].substr(tokens[1].size() - 2));
			}

			nodeToFlow[current] = flow;
			if (flow > 0)
			{
				nodesWithFlow.insert(current);
			}

			const auto trackNodeIfNew = [](const std::string& node)
			{
				if (!nodeToIndex.contains(node))
				{
					indexToNode.emplace_back(node);
					nodeToIndex[node] = static_cast<int>(indexToNode.size()) - 1;
				}
			};

			trackNodeIfNew(current);
			for (const auto& next : connectedNodes)
			{
				trackNodeIfNew(next);

				// No need to add edges if we already processed in the other direction.
				if (!nodeToFlow.contains(next))
				{
					constexpr auto weight = 1;
					boost::add_edge(nodeToIndex[current], nodeToIndex[next], weight, inputGraph);
				}
			}
		}

		return inputGraph;
	}

	Graph BuildWeightedGraph(const Graph& inputGraph)
	{
		auto graph = Graph{};
		auto alreadyEvaluated = std::unordered_set<int>{};
		auto importantNodes = nodesWithFlow | std::ranges::to<std::vector>();
		if (!nodesWithFlow.contains(initialPosition))
		{
			// Consider AA first because there won't be a path back.
			importantNodes.emplace_back(initialPosition);
			std::ranges::sort(importantNodes);
		}

		// Dijkstra from each important node (nodes with flow and initial position) to find shortest paths between important nodes.
		// Remove non-important nodes and set the edge weight to traverse through them to their distance.
		for (auto importantNode : importantNodes)
		{
			auto start = nodeToIndex[importantNode];
			auto predecessors = std::vector<Graph::vertex_descriptor>(boost::num_vertices(inputGraph));
			auto distances = std::vector<int>(boost::num_vertices(inputGraph));
			boost::dijkstra_shortest_paths(inputGraph, start, boost::distance_map(distances.data()).predecessor_map(predecessors.data()));

			for (auto i = 0; i < distances.size(); ++i)
			{
				if (i == start || alreadyEvaluated.contains(i))
				{
					// Don't create edges to self or to nodes where we've already done the evaluation (as we already made the edge).
					continue;
				}

				auto next = indexToNode[i];
				if (!nodesWithFlow.contains(next))
				{
					// Discard paths to nodes that don't have flow.
					continue;
				}

				// Add weight 1 to account for turning on the flow.
				boost::add_edge(start, i, distances[i] + 1, graph);
			}

			alreadyEvaluated.insert(start);
		}

		return graph;
	}

	void Explore(const Graph& graph, std::unordered_set<Path>& allPaths, Path currentPath)
	{
		Utilities::VerifyElseCrash(currentPath.time < timeBudget);

		for (auto [it, end] = boost::out_edges(currentPath.GetCurrentLocation(), graph); it != end; ++it)
		{
			auto edge = *it;
			auto source = static_cast<int>(boost::source(edge, graph));
			auto target = static_cast<int>(boost::target(edge, graph));
			auto weight = boost::get(boost::edge_weight, graph, edge);

			auto next = currentPath.GetCurrentLocation() == source ? target : source;
			if (currentPath.opened[next] || currentPath.time + weight > timeBudget)
			{
				continue;
			}

			auto nextPath = currentPath;
			nextPath.time += weight;
			nextPath.score += (weight * nextPath.flow);
			nextPath.flow += nodeToFlow[indexToNode[next]];
			nextPath.nodeIndices.emplace_back(next);
			nextPath.opened[next] = true;

			auto [_, bInserted] = allPaths.insert(nextPath);
			if (bInserted && nextPath.time < timeBudget)
			{
				Explore(graph, allPaths, nextPath);
			}
		}
	}

	std::vector<Path> FinalizePaths(const std::unordered_set<Path>& paths)
	{
		auto finalPaths = paths | std::ranges::to<std::vector>();
		for (auto& path : finalPaths)
		{
			if (path.time < timeBudget)
			{
				auto timeAdjustment = timeBudget - path.time;
				path.score += path.flow * timeAdjustment;
				path.time += timeAdjustment;
			}
		}
		std::ranges::sort(finalPaths);

		return finalPaths;
	}

	void PrintGraphviz(const Graph& graph)
	{
		auto vertexLabels = indexToNode;
		for (auto i = 0; i < vertexLabels.size(); ++i)
		{
			vertexLabels[i] += " - " + std::to_string(nodeToFlow[vertexLabels[i]]);
		}
		auto weights = boost::get(boost::edge_weight, graph);
		boost::write_graphviz(std::cout, graph, boost::make_label_writer(&vertexLabels[0]), boost::make_label_writer(weights));
	}

	void PrintSolution()
	{
		auto graph = BuildWeightedGraph(BuildGraphFromInput());

		auto currentPath = Path{};
		currentPath.nodeIndices.emplace_back(nodeToIndex[initialPosition]);
		currentPath.opened.resize(boost::num_vertices(graph));
		auto allPaths = std::unordered_set<Path>{ currentPath };
		Explore(graph, allPaths, currentPath);

		auto finalPaths = FinalizePaths(allPaths);
		std::cout << finalPaths.back().score;
	}
}