#include "pch.h"
#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

namespace Puzzle23A
{
	struct Point
	{
		int x = 0;
		int y = 0;

		auto operator<=>(const Point&) const = default;
	};
}

IMPLEMENT_STD_HASH(Puzzle23A::Point, value.x, value.y);

namespace Puzzle23A
{
	bool AnyElfAdjacent(const std::unordered_set<Point>& elfPositions, const Point& elfPosition)
	{
		return elfPositions.contains(Point{ elfPosition.x - 1, elfPosition.y - 1 })
			|| elfPositions.contains(Point{ elfPosition.x,     elfPosition.y - 1 })
			|| elfPositions.contains(Point{ elfPosition.x + 1, elfPosition.y - 1 })
			|| elfPositions.contains(Point{ elfPosition.x - 1, elfPosition.y })
			|| elfPositions.contains(Point{ elfPosition.x + 1, elfPosition.y })
			|| elfPositions.contains(Point{ elfPosition.x - 1, elfPosition.y + 1 })
			|| elfPositions.contains(Point{ elfPosition.x    , elfPosition.y + 1 })
			|| elfPositions.contains(Point{ elfPosition.x + 1, elfPosition.y + 1 });
	}

	bool AnyElfNorth(const std::unordered_set<Point>& elfPositions, const Point& elfPosition)
	{
		return elfPositions.contains(Point{ elfPosition.x - 1, elfPosition.y - 1 })
			|| elfPositions.contains(Point{ elfPosition.x,     elfPosition.y - 1 })
			|| elfPositions.contains(Point{ elfPosition.x + 1, elfPosition.y - 1 });
	}

	bool AnyElfSouth(const std::unordered_set<Point>& elfPositions, const Point& elfPosition)
	{
		return elfPositions.contains(Point{ elfPosition.x - 1, elfPosition.y + 1 })
			|| elfPositions.contains(Point{ elfPosition.x,     elfPosition.y + 1 })
			|| elfPositions.contains(Point{ elfPosition.x + 1, elfPosition.y + 1 });
	}

	bool AnyElfWest(const std::unordered_set<Point>& elfPositions, const Point& elfPosition)
	{
		return elfPositions.contains(Point{ elfPosition.x - 1, elfPosition.y - 1 })
			|| elfPositions.contains(Point{ elfPosition.x - 1, elfPosition.y })
			|| elfPositions.contains(Point{ elfPosition.x - 1, elfPosition.y + 1 });
	}

	bool AnyElfEast(const std::unordered_set<Point>& elfPositions, const Point& elfPosition)
	{
		return elfPositions.contains(Point{ elfPosition.x + 1, elfPosition.y - 1 })
			|| elfPositions.contains(Point{ elfPosition.x + 1, elfPosition.y })
			|| elfPositions.contains(Point{ elfPosition.x + 1, elfPosition.y + 1 });
	}

	// Debug helper.
	void Draw(const std::unordered_set<Point>& positions, Utilities::Grid2d<char>& grid)
	{
		std::ranges::fill(grid, '.');
		for (auto& position : positions)
		{
			grid.at(position.x, position.y) = '#';
		}
		std::cout << grid << "\n\n";
	}

	void PrintSolution()
	{
		constexpr auto numRounds = 10;

		// Load from input with enough padding for movement over the desired number of rounds.
		auto input = Utilities::ReadAllLinesInFile("Puzzle23.input");
		auto grid = Utilities::Grid2d<char>(input[0].size() + 2 * numRounds, input.size() + 2 * numRounds);
		auto positions = std::unordered_set<Point>{};
		for (auto y = 0; y < input.size(); ++y)
		{
			const auto& line = input[y];
			for (auto x = 0; x < line.size(); ++x)
			{
				if (line[x] == '#')
				{
					positions.insert(Point{ x + numRounds, y + numRounds });
				}
			}
		}

		auto directions = std::vector<char>{ 'N', 'S', 'W', 'E' };
		for (auto round = 1; round <= numRounds; ++round)
		{
			// First half of round. Propose movement.
			auto proposals = std::unordered_map<Point, Point>{};
			for (const auto& position : positions)
			{
				if (!AnyElfAdjacent(positions, position))
				{
					continue;
				}

				for (auto direction : directions)
				{
					switch (direction)
					{
					case 'N':
						if (!AnyElfNorth(positions, position)) { proposals[position] = Point(position.x, position.y - 1); }
						break;
					case 'S':
						if (!AnyElfSouth(positions, position)) { proposals[position] = Point(position.x, position.y + 1); }
						break;
					case 'W':
						if (!AnyElfWest(positions, position)) { proposals[position] = Point(position.x - 1, position.y); }
						break;
					case 'E':
						if (!AnyElfEast(positions, position)) { proposals[position] = Point(position.x + 1, position.y); }
						break;
					}

					if (proposals.contains(position)) { break; }
				}
			}

			// Second half of round. Identify conflicting proposals.
			auto seen = std::unordered_set<Point>{};
			auto conflicts = std::unordered_set<Point>{};
			for (const auto& proposal : proposals | std::ranges::views::values)
			{
				auto [it, result] = seen.insert(proposal);
				if (!result)
				{
					conflicts.insert(proposal);
				}
			}

			// Execute movement.
			for (const auto& proposal : proposals)
			{
				if (!conflicts.contains(proposal.second))
				{
					positions.erase(proposal.first);
					positions.insert(proposal.second);
				}
			}

			// End of round. Rotate directions.
			auto front = directions.front();
			directions = directions | std::ranges::views::drop(1) | std::ranges::to<std::vector>();
			directions.emplace_back(front);
		}

		auto minX = std::ranges::min(positions, std::less<int>{}, [](const Point& point) { return point.x; }).x;
		auto maxX = std::ranges::max(positions, std::less<int>{}, [](const Point& point) { return point.x; }).x;
		auto minY = std::ranges::min(positions, std::less<int>{}, [](const Point& point) { return point.y; }).y;
		auto maxY = std::ranges::max(positions, std::less<int>{}, [](const Point& point) { return point.y; }).y;

		auto width = maxX - minX + 1;
		auto height = maxY - minY + 1;
		auto area = width * height;
		auto emptyTiles = area - positions.size();

		std::cout << emptyTiles;
	}
}