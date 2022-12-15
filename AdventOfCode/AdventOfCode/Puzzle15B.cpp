#include "pch.h"
#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

namespace Puzzle15B
{
	struct Point
	{
		int x = 0;
		int y = 0;

		int ManhattanDistance(const Point& other) const { return std::abs(x - other.x) + std::abs(y - other.y); }

		auto operator<=>(const Point&) const = default;
	};
}

IMPLEMENT_STD_HASH(Puzzle15B::Point, value.x, value.y);

namespace Puzzle15B
{
	std::vector<std::pair<int, int>> FindCoveredRangesOnLine(const std::unordered_map<Point, Point>& scannerToClosestBeacon, int targetY)
	{
		auto coveredRanges = std::vector<std::pair<int, int>>{ scannerToClosestBeacon.size()  };
		for (const auto& [scanner, beacon] : scannerToClosestBeacon)
		{
			auto distanceToBeacon = scanner.ManhattanDistance(beacon);
			auto distanceToLine = scanner.ManhattanDistance(Point{ scanner.x, targetY });
			if (distanceToLine > distanceToBeacon)
			{
				continue;
			}

			auto distanceX = distanceToBeacon - distanceToLine;
			auto begin = scanner.x - distanceX;
			auto end = scanner.x + distanceX;
			coveredRanges.emplace_back(begin, end);
		}

		std::ranges::sort(coveredRanges, std::less<int>{}, [](const auto& pair) { return pair.first; });

		return coveredRanges;
	}

	void PrintSolution()
	{
		auto input = Utilities::ReadAllLinesInFile("Puzzle15.input");

		auto scannerToClosestBeacon = std::unordered_map<Point, Point>{};
		for (const auto& line : input)
		{
			constexpr auto format = "Sensor at x={}, y={}: closest beacon is at x={}, y={}";
			auto [_, scannerX, scannerY, beaconX, beaconY] = scn::scan_tuple<int, int, int, int>(line, format);
			auto scanner = Point{ scannerX, scannerY };
			auto beacon = Point{ beaconX, beaconY };
			scannerToClosestBeacon[scanner] = beacon;
		}

		constexpr auto maxX = 4000000;
		constexpr auto maxY = 4000000;
		auto distressBeacon = Point{ -1, -1 };
		for (auto y : std::ranges::views::iota(0, maxY + 1))
		{
			auto ranges = FindCoveredRangesOnLine(scannerToClosestBeacon, y);

			auto candidateX = 0;
			for (const auto& range : ranges)
			{
				if (range.first <= candidateX && candidateX <= range.second)
				{
					candidateX = range.second + 1;
				}
			}

			if (candidateX <= maxX)
			{
				distressBeacon.x = candidateX;
				distressBeacon.y = y;
				break;
			}
		}

		std::cout << static_cast<uint64_t>(distressBeacon.x) * 4000000 + distressBeacon.y;
	}
}