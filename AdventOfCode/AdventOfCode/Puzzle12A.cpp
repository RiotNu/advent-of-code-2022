#include "pch.h"
#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

namespace Puzzle12A
{
	using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property, boost::property<boost::edge_weight_t, int>>;

	bool HasEdge(const Utilities::Grid2d<char>& grid, int x1, int y1, int x2, int y2)
	{
		if (x2 < 0 || x2 >= grid.Width() || y2 < 0 || y2 >= grid.Height())
		{
			return false;
		}

		return grid.at(x2, y2) <= grid.at(x1, y1) + 1;
	}

	void PrintSolution()
	{
		auto input = Utilities::ReadAllLinesInFile("Puzzle12.input");
		auto grid = Utilities::Grid2d<char>(input[0].size(), input.size());
		auto start = 0ull;
		auto end = 0ull;
		for (auto y = 0; y < input.size(); ++y)
		{
			const auto& line = input[y];
			for (auto x = 0; x < line.size(); ++x)
			{
				auto val = line[x];
				if (val == 'S')
				{
					start = grid.GetIndexFromCoordinates(x, y);
					val = 'a';
				}
				else if (val == 'E')
				{
					end = grid.GetIndexFromCoordinates(x, y);
					val = 'z';
				}
				grid.at(x, y) = val;
			}
		}

		constexpr auto weight = 1;
		auto graph = Graph{};
		for (auto y = 0; y < grid.Height(); ++y)
		{
			for (auto x = 0; x < grid.Width(); ++x)
			{
				auto source = grid.GetIndexFromCoordinates(x, y);
				if (HasEdge(grid, x, y, x - 1, y))
				{
					auto target = grid.GetIndexFromCoordinates(x - 1, y);
					boost::add_edge(source, target, weight, graph);
				}

				if (HasEdge(grid, x, y, x + 1, y))
				{
					auto target = grid.GetIndexFromCoordinates(x + 1, y);
					boost::add_edge(source, target, weight, graph);
				}

				if (HasEdge(grid, x, y, x, y - 1))
				{
					auto target = grid.GetIndexFromCoordinates(x, y - 1);
					boost::add_edge(source, target, weight, graph);
				}

				if (HasEdge(grid, x, y, x, y + 1))
				{
					auto target = grid.GetIndexFromCoordinates(x, y + 1);
					boost::add_edge(source, target, weight, graph);
				}
			}
		}

		auto predecessors = std::vector<Graph::vertex_descriptor>(boost::num_vertices(graph));
		auto distances = std::vector<int>(boost::num_vertices(graph));
		boost::dijkstra_shortest_paths(graph, start, boost::distance_map(distances.data()).predecessor_map(predecessors.data()));

		std::cout << distances[end];
	}
}