#include "pch.h"
#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

namespace Puzzle19A
{
	enum class Resource { Ore, Clay, Obsidian, Geode, Count };

	using RobotCost = std::array<int, static_cast<int>(Resource::Count)>;

	struct Blueprint
	{
		int name = 0;

		// Cost of each robot.
		std::array<RobotCost, static_cast<int>(Resource::Count)> robotCosts;

		// Max number of each robot to build. We can only build one robot per turn, so we never need
		// to collect more than the max cost of each individual robot per turn.
		std::array<int, static_cast<int>(Resource::Count)> maxRobots;
	};

	struct Snapshot
	{
		// How many minutes have elapsed.
		int minute = 0;

		// Counts of each resource.
		std::array<int, static_cast<int>(Resource::Count)> resources;

		// Counts of each robot.
		std::array<int, static_cast<int>(Resource::Count)> robots;

		// Excludes minutes and declined robots state. This is safe since we work forwards in time. (Later duplicates can be dropped.)
		bool operator==(const Snapshot& other) const { return resources == other.resources && robots == other.robots; }
	};

	int GetMaxGeodes(const Blueprint& blueprint)
	{
		// Snapshot after minute 1.
		constexpr auto firstMinute = Snapshot{ 1, {1, 0, 0, 0}, {1, 0, 0, 0} };
		constexpr auto maxMinutes = 24;

		auto bestSnapshot = firstMinute;
		auto snapshotsToProcess = std::stack<Snapshot>{};
		snapshotsToProcess.push(firstMinute);
		while (!snapshotsToProcess.empty())
		{
			auto previousSnapshot = snapshotsToProcess.top();
			snapshotsToProcess.pop();

			// Assume a free geode robot every remaining turn. Is it possible to beat the best snapshot?
			auto additionalGeodeRobots = 0;
			auto additionalGeodes = 0;
			for (auto i = previousSnapshot.minute; i <= maxMinutes; ++i)
			{
				additionalGeodes += previousSnapshot.robots[static_cast<int>(Resource::Geode)] + additionalGeodeRobots++;
			}
			if (previousSnapshot.resources[static_cast<int>(Resource::Geode)] + additionalGeodes <= bestSnapshot.resources[static_cast<int>(Resource::Geode)])
			{
				continue;
			}

			// Continue forward with each robot as next purchase state.
			for (auto iRobot = 0; iRobot < static_cast<int>(Resource::Count); ++iRobot)
			{
				// Is there any point to make more of this robot?
				if (previousSnapshot.robots[iRobot] == blueprint.maxRobots[iRobot])
				{
					continue;
				}

				// Can we create this robot with current gathering robots?
				auto bCanBuildRobot = true;
				auto bCanAffordRobot = true;
				const auto& robotCost = blueprint.robotCosts[iRobot];
				for (auto iResource = 0; iResource < static_cast<int>(Resource::Count); ++iResource)
				{
					if (previousSnapshot.robots[iResource] == 0 && robotCost[iResource] != 0)
					{
						bCanBuildRobot = false;
						bCanAffordRobot = false;
						break;
					}

					if (previousSnapshot.resources[iResource] < robotCost[iResource])
					{
						bCanAffordRobot = false;
					}
				}

				if (!bCanBuildRobot)
				{
					continue;
				}

				// Move forward in time until the robot can be created.
				auto nextSnapshot = previousSnapshot;
				auto bOutOfTime = nextSnapshot.minute == maxMinutes;
				while (!bCanAffordRobot && !bOutOfTime)
				{
					nextSnapshot.minute++;

					bCanAffordRobot = true;
					for (auto iResource = 0; iResource < static_cast<int>(Resource::Count); ++iResource)
					{
						nextSnapshot.resources[iResource] += nextSnapshot.robots[iResource];
						if (nextSnapshot.resources[iResource] < robotCost[iResource])
						{
							bCanAffordRobot = false;
						}
					}

					bOutOfTime = nextSnapshot.minute == maxMinutes;
				}

				if (bOutOfTime)
				{
					if (nextSnapshot.resources[static_cast<int>(Resource::Geode)] > bestSnapshot.resources[static_cast<int>(Resource::Geode)])
					{
						bestSnapshot = nextSnapshot;
					}
					continue;
				}

				// Build the robot and collect resources.
				nextSnapshot.minute++;
				for (auto iResource = 0; iResource < static_cast<int>(Resource::Count); ++iResource)
				{
					nextSnapshot.resources[iResource] += nextSnapshot.robots[iResource];
					nextSnapshot.resources[iResource] -= robotCost[iResource];
				}
				nextSnapshot.robots[iRobot]++;

				if (nextSnapshot.minute == maxMinutes)
				{
					if (nextSnapshot.resources[static_cast<int>(Resource::Geode)] > bestSnapshot.resources[static_cast<int>(Resource::Geode)])
					{
						bestSnapshot = nextSnapshot;
					}
					continue;
				}

				// Enqueue snapshot for further processing if it's new.
				snapshotsToProcess.push(nextSnapshot);
			}
		}

		return bestSnapshot.resources[static_cast<int>(Resource::Geode)];
	}

	void PrintSolution()
	{
		auto blueprints = std::vector<Blueprint>{};
		for (const auto& line : Utilities::ReadAllLinesInFile("Puzzle19.input"))
		{
			auto [_, name, oreOre, clayOre, obsidianOre, obsidianClay, geodeOre, geodeObsidian] = scn::scan_tuple<int, int, int, int, int, int, int>(
				line,
				"Blueprint {}: Each ore robot costs {} ore. Each clay robot costs {} ore. Each obsidian robot costs {} ore and {} clay. Each geode robot costs {} ore and {} obsidian.");

			auto maxRobots = std::array<int, static_cast<int>(Resource::Count)>{ std::ranges::max({oreOre, clayOre, obsidianOre, geodeOre}), obsidianClay, geodeObsidian, -1 };
			auto costs = std::array<RobotCost, static_cast<int>(Resource::Count)>
			{
				RobotCost{ oreOre }, RobotCost{ clayOre }, RobotCost{ obsidianOre, obsidianClay }, RobotCost{ geodeOre, 0, geodeObsidian }
			};
			blueprints.emplace_back(name, costs, maxRobots);
		}

		std::cout << ranges::accumulate(blueprints, 0, [](int sum, const Blueprint& blueprint) { return sum + blueprint.name * GetMaxGeodes(blueprint); });
	}
}