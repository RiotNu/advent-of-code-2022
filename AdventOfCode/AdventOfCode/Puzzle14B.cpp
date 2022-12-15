#include "pch.h"
#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

namespace Puzzle14B
{
	struct Point
	{
		int x = 0;
		int y = 0;

		auto operator<=>(const Point&) const = default;
	};
}

IMPLEMENT_STD_HASH(Puzzle14B::Point, value.x, value.y);

namespace Puzzle14B
{
	void PrintSolution()
	{
		auto input = Utilities::ReadAllLinesInFile("Puzzle14.input");

		auto maxY = 0;
		auto allRocks = std::unordered_set<Point>{};
		for (const auto& line : input)
		{
			auto tokens = Utilities::SplitString(line, " -> ");
			auto points = std::vector<Point>{};
			for (const auto& token : tokens)
			{
				auto [_, x, y] = scn::scan_tuple<int, int>(token, "{},{}");
				points.emplace_back(x, y);
				maxY = std::max(y, maxY);
			}

			for (auto i = 0; i < points.size() - 1; ++i)
			{
				auto begin = points[i];
				auto end = points[i + 1];
				if (begin.x == end.x)
				{
					if (begin.y > end.y)
					{
						std::swap(begin, end);
					}

					for (auto y : std::ranges::views::iota(begin.y, end.y + 1))
					{
						allRocks.insert({ begin.x, y });
					}
				}
				else
				{
					if (begin.x > end.x)
					{
						std::swap(begin, end);
					}

					for (auto x : std::ranges::views::iota(begin.x, end.x + 1))
					{
						allRocks.insert({ x, begin.y });
					}
				}
			}
		}

		// Build a really big floor. Not actually infinite, but probably good enough.
		for (auto x : std::ranges::views::iota(0, 1000))
		{
			allRocks.insert({ x, maxY + 2 });
		}

		constexpr auto startPosition = Point{ 500, 0 };
		auto allSand = std::unordered_set<Point>{};
		while (!allSand.contains(startPosition))
		{
			auto position = startPosition;
			auto bSandMoved = true;
			while (bSandMoved)
			{
				bSandMoved = false;

				auto nextPosition = Point{ position.x, position.y + 1 };
				if (!allRocks.contains(nextPosition) && !allSand.contains(nextPosition))
				{
					bSandMoved = true;
					position = nextPosition;
					continue;
				}

				nextPosition = Point{ position.x - 1, position.y + 1 };
				if (!allRocks.contains(nextPosition) && !allSand.contains(nextPosition))
				{
					bSandMoved = true;
					position = nextPosition;
					continue;
				}

				nextPosition = Point{ position.x + 1, position.y + 1 };
				if (!allRocks.contains(nextPosition) && !allSand.contains(nextPosition))
				{
					bSandMoved = true;
					position = nextPosition;
					continue;
				}
			}

			allSand.insert(position);
		}

		std::cout << allSand.size();
	}
}