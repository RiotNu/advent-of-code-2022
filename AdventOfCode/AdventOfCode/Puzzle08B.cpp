#include "pch.h"
#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

namespace Puzzle08B
{
	void PrintSolution()
	{
		auto input = Utilities::ReadAllLinesInFile("Puzzle08.input");

		auto grid = Utilities::Grid2d<int>(input[0].size(), input.size());
		for (auto y = 0; y < input.size(); ++y)
		{
			const auto& line = input[y];
			for (auto x = 0; x < line.size(); ++x)
			{
				grid.at(x, y) = line[x] - '0';
			}
		}

		auto scenicScoresGrid = Utilities::Grid2d<int>(grid.Width(), grid.Height());
		for (auto y = 1; y < grid.Height() - 1; ++y)
		{
			for (auto x = 1; x < grid.Width() - 1; ++x)
			{
				auto current = grid.at(x, y);

				auto visibleLeft = 0;
				for (int i = x - 1; i >= 0; --i)
				{
					++visibleLeft;
					if (grid.at(i, y) >= current) { break; }
				}

				auto visibleRight = 0;
				for (int i = x + 1; i < grid.Width(); ++i)
				{
					++visibleRight;
					if (grid.at(i, y) >= current) { break; }
				}

				auto visibleTop = 0;
				for (int i = y - 1; i >= 0; --i)
				{
					++visibleTop;
					if (grid.at(x, i) >= current) { break; }
				}

				auto visibleBottom = 0;
				for (int i = y + 1; i < grid.Height(); ++i)
				{
					++visibleBottom;
					if (grid.at(x, i) >= current) { break; }
				}

				scenicScoresGrid.at(x, y) = visibleLeft * visibleRight * visibleTop * visibleBottom;
			}
		}

		std::cout << std::ranges::max(scenicScoresGrid);
	}
}