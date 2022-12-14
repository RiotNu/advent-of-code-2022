#include "pch.h"
#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

namespace Puzzle09A
{
	struct Point
	{
		int x = 0;
		int y = 0;

		Point operator-(const Point& other) const { return Point{ x - other.x, y - other.y }; };
		auto operator<=>(const Point&) const = default;
	};
}

IMPLEMENT_STD_HASH(Puzzle09A::Point, value.x, value.y);

namespace Puzzle09A
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

		auto head = Point{ 0, 0 };
		auto tail = Point{ 0, 0 };
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

				tail = GetNextTailPosition(head, tail);
				tailVisited.insert(tail);
			}
		}

		std::cout << tailVisited.size();
	}
}