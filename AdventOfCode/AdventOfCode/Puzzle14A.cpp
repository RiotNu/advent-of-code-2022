#include "pch.h"
#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

namespace Puzzle14A
{
    struct Point
    {
        int x = 0;
        int y = 0;

        auto operator<=>(const Point&) const = default;
    };
}

IMPLEMENT_STD_HASH(Puzzle14A::Point, value.x, value.y);

namespace Puzzle14A
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
                        allRocks.insert({begin.x, y});
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
                        allRocks.insert({x, begin.y});
                    }
                }
            }
        }

        auto allSand = std::unordered_set<Point>{};
        auto isSandFallingIntoAbyss = false;
        while (!isSandFallingIntoAbyss)
        {
            auto position = Point{500, 0};
            auto bSandMoved = true;
            while (bSandMoved)
            {
                bSandMoved = false;
                if (position.y > maxY)
                {
                    isSandFallingIntoAbyss = true;
                    break;
                }

                auto nextPosition = Point{ position.x, position.y + 1 };
                if (!allRocks.contains(nextPosition) && !allSand.contains(nextPosition))
                {
                    bSandMoved = true;
                    position = nextPosition;
                    continue;
                }

                nextPosition = Point{ position.x - 1, position.y + 1};
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

            if (!isSandFallingIntoAbyss)
            {
                allSand.insert(position);
            }
        }

        std::cout << allSand.size();
    }
}