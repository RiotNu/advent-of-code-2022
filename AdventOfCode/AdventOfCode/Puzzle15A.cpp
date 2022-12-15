#include "pch.h"
#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

namespace Puzzle15A
{
	struct Point
	{
		int x = 0;
		int y = 0;

		int ManhattanDistance(const Point& other) const { return std::abs(x - other.x) + std::abs(y - other.y); }

		auto operator<=>(const Point&) const = default;
	};
}

IMPLEMENT_STD_HASH(Puzzle15A::Point, value.x, value.y);

namespace Puzzle15A
{
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

		constexpr auto targetY = 2000000;
		auto coveredRanges = std::vector<std::pair<int, int>>{};
		for (const auto& [scanner, beacon] : scannerToClosestBeacon)
		{
			auto distanceToBeacon = scanner.ManhattanDistance(beacon);
			auto distanceToLine = scanner.ManhattanDistance(Point{ scanner.x, targetY });

			if (distanceToLine > distanceToBeacon)
			{
				continue;
			}

			if (distanceToLine == distanceToBeacon && beacon.y == targetY)
			{
				continue;
			}

			auto distanceX = distanceToBeacon - distanceToLine;
			auto begin = (beacon.y == targetY && beacon.x < scanner.x) ? beacon.x + 1 : scanner.x - distanceX;
			auto end = (beacon.y == targetY && beacon.x > scanner.x) ? beacon.x - 1 :scanner.x + distanceX;
			coveredRanges.emplace_back(begin, end);
		}

		std::ranges::sort(coveredRanges,
			[](const auto& left, const auto& right)
			{
				if (left.first < right.first) { return true; }
				if (left.first > right.first) { return false; }
				return left.second < right.second;
			});

		auto collapsedRanges = std::vector<std::pair<int, int>>{};
		for (auto i = 0; i < coveredRanges.size(); ++i)
		{
			auto current = coveredRanges[i];
			for (auto j = i + 1; j < coveredRanges.size(); ++j)
			{
				auto next = coveredRanges[j];
				if (next.first <= current.second)
				{
					// next starts within current; we'll handle next here
					i = j;

					if (next.second <= current.second)
					{
						// next already fully contained by current; discard next
						continue;
					}

					// next extends current; expand current
					current.second = next.second;
					continue;
				}
				else
				{
					// next is a new range; consider as next current
					i = j - 1;
					break;
				}
			}

			collapsedRanges.emplace_back(current);
		}

		auto size = ranges::accumulate(collapsedRanges, 0, [](int sum, const auto& range) { return sum + range.second - range.first + 1; });
		std::cout << size;
	}
}