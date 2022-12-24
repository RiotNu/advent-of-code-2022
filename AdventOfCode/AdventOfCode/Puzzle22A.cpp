#include "pch.h"
#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

namespace Puzzle22A
{
	struct Vec2
	{
		int x = 0;
		int y = 0;

		Vec2 operator+(const Vec2& other) const { return Vec2{ x + other.x, y + other.y }; }
		auto operator<=>(const Vec2&) const = default;
	};

	constexpr auto left  = Vec2{ -1,  0 };
	constexpr auto right = Vec2{ 1,  0 };
	constexpr auto up    = Vec2{ 0, -1 };
	constexpr auto down  = Vec2{ 0,  1 };

	struct Maze
	{
		Maze(const auto& inputView)
			: m_grid(std::ranges::max(inputView, std::less<size_t>{}, [](const std::string& line) { return line.size(); }).size(), inputView.size())
		{
			// Load the grid from input.
			std::ranges::fill(m_grid, ' ');
			auto bSetStartingPosition = 0;
			auto y = 0;
			for (const auto& line : inputView)
			{
				for (auto x = 0; x < line.size(); ++x)
				{
					m_grid.at(x, y) = line[x];
					if (!bSetStartingPosition && line[x] != ' ')
					{
						m_position = Vec2{ x, y };
						bSetStartingPosition = true;
					}
				}
				++y;
			}

			// Keep track of the min and max valid positions on each row.
			for (auto y = 0; y < m_grid.Height(); ++y)
			{
				auto minX = -1;
				auto maxX = -1;
				for (auto x = 0; x < m_grid.Width(); ++x)
				{
					if (m_grid.at(x, y) == ' ')
					{
						continue;
					}

					if (minX < 0)
					{
						minX = x;
					}

					maxX = std::max(maxX, x);
				}

				m_rowMinMax.emplace_back(minX, maxX);
			}

			// Keep track of the min and max valid positions on each column.
			for (auto x = 0; x < m_grid.Width(); ++x)
			{
				auto minY = -1;
				auto maxY = -1;
				for (auto y = 0; y < m_grid.Height(); ++y)
				{
					if (m_grid.at(x, y) == ' ')
					{
						continue;
					}

					if (minY < 0)
					{
						minY = y;
					}

					maxY = std::max(maxY, y);
				}

				m_columnMinMax.emplace_back(minY, maxY);
			}
		}

		void TurnLeft()
		{
			if (m_direction == left) { m_direction = down; }
			else if (m_direction == right) { m_direction = up; }
			else if (m_direction == up) { m_direction = left; }
			else if (m_direction == down) { m_direction = right; }
			else { Utilities::VerifyNotReached(); }
		}

		void TurnRight()
		{
			if (m_direction == left) { m_direction = up; }
			else if (m_direction == right) { m_direction = down; }
			else if (m_direction == up) { m_direction = right; }
			else if (m_direction == down) { m_direction = left; }
			else { Utilities::VerifyNotReached(); }
		}

		void Move(int distance)
		{
			while (distance-- > 0)
			{
				auto next = m_position + m_direction;

				// If walking off the edge, teleport the other side.
				if (m_direction == left && (next.x < 0 || m_grid.at(next.x, next.y) == ' '))
				{
					next.x = m_rowMinMax[m_position.y].second;
				}
				else if (m_direction == right && (next.x >= m_grid.Width() || m_grid.at(next.x, next.y) == ' '))
				{
					next.x = m_rowMinMax[m_position.y].first;
				}
				else if (m_direction == up && (next.y < 0 || m_grid.at(next.x, next.y) == ' '))
				{
					next.y = m_columnMinMax[m_position.x].second;
				}
				else if (m_direction == down && (next.y >= m_grid.Height() || m_grid.at(next.x, next.y) == ' '))
				{
					next.y = m_columnMinMax[m_position.x].first;
				}

				// Trace path for debugging.
				if (m_direction == right)     { m_grid.at(m_position.x, m_position.y) = '>'; }
				else if (m_direction == left) { m_grid.at(m_position.x, m_position.y) = '<'; }
				else if (m_direction == up)   { m_grid.at(m_position.x, m_position.y) = '^'; }
				else if (m_direction == down) { m_grid.at(m_position.x, m_position.y) = 'v'; }

				// Stop if we hit a wall.
				if (m_grid.at(next.x, next.y) == '#')
				{
					return;
				}

				// If clear, continue to next position.
				m_position = next;
			}
		}

		int GetPassword()
		{
			auto facingScore = 0;
			if (m_direction == right)
			{
				facingScore = 0;
			}
			else if (m_direction == left)
			{
				facingScore = 2;
			}
			else if (m_direction == up)
			{
				facingScore = 3;
			}
			else if (m_direction == down)
			{
				facingScore = 1;
			}

			return 1000 * (m_position.y + 1) + 4 * (m_position.x + 1) + facingScore;
		}

		Utilities::Grid2d<char> m_grid;
		Vec2 m_direction = right;
		Vec2 m_position = Vec2{ 0, 0 };
		std::vector<std::pair<int, int>> m_rowMinMax;
		std::vector<std::pair<int, int>> m_columnMinMax;
	};

	void PrintSolution()
	{
		auto input = Utilities::ReadAllLinesInFile("Puzzle22.input");
		auto maze = Maze{ input | std::ranges::views::take(input.size() - 2) };

		const auto& directions = input[input.size() - 1];
		for (auto i = 0; i < directions.size(); ++i)
		{
			if (directions[i] == 'L')
			{
				maze.TurnLeft();
			}
			else if (directions[i] == 'R')
			{
				maze.TurnRight();
			}
			else
			{
				int j = i;
				while (j++ < directions.size() && directions[j] >= '0' && directions[j] <= '9')
				{
				}

				auto distance = std::stoi(directions.substr(i, j - i));
				maze.Move(distance);
				i = j - 1;
			}
		}

		std::cout << maze.GetPassword();
	}
}