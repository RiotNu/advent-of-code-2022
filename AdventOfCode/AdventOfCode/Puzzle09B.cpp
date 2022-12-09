#include "pch.h"
#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

namespace Puzzle09B
{
	struct Point
	{
		int x = 0;
		int y = 0;

		Point operator-(const Point& other) const { return Point{ x - other.x, y - other.y }; };
		auto operator<=>(const Point&) const = default;
	};
}

IMPLEMENT_STD_HASH(Puzzle09B::Point, value.x, value.y);

namespace Puzzle09B
{
	Point GetNextTailPosition(const Point& head, const Point& tail)
	{
		auto relative = head - tail;
		if (std::abs(relative.x) <= 1 && std::abs(relative.y) <= 1)
		{
			return tail;
		}

		if (head.y == tail.y)
		{
			return Point{ head.x > tail.x ? head.x - 1 : head.x + 1, tail.y };
		}
		else if (head.x == tail.x)
		{
			return Point{ tail.x, head.y > tail.y ? head.y - 1 : tail.y - 1 };
		}
		else
		{
			return Point
			{
				head.x > tail.x ? tail.x + 1 : tail.x - 1,
				head.y > tail.y ? tail.y + 1 : tail.y - 1
			};
		}
	}

	void PrintSolution()
	{
		auto input = Utilities::ReadAllLinesInFile("Puzzle09.input");

		auto knots = std::array<Point, 10>{0};
		auto& head = knots[0];
		auto& tail = knots[knots.size() - 1];
		auto tailVisited = std::unordered_set<Point>{ tail };

		for (const auto& line : input)
		{
			auto direction = line[0];
			auto magnitude = std::stoi(line.substr(2));

			while (magnitude-- > 0)
			{
				switch (direction)
				{
				case 'L':
					head.x--;
					break;
				case 'R':
					head.x++;
					break;
				case 'U':
					head.y--;
					break;
				case 'D':
					head.y++;
					break;
				}

				for (auto iCurrent = 0, iNext = 1; iNext < knots.size(); ++iCurrent, ++iNext)
				{
					knots[iNext] = GetNextTailPosition(knots[iCurrent], knots[iNext]);
				}
				tailVisited.insert(tail);
			}
		}

		std::cout << tailVisited.size();
	}
}