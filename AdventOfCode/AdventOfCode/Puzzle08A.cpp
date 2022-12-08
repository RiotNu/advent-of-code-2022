#include "pch.h"
#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

namespace Puzzle08A
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

		auto countVisible = grid.Width() * 2 + (grid.Height() - 2) * 2;
		for (auto y = 1; y < grid.Height() - 1; ++y)
		{
			for (auto x = 1; x < grid.Width() - 1; ++x)
			{
				auto current = grid.at(x, y);

				const auto checkRow = [&grid, y, current](int i) { return grid.at(i, y) < current; };
				auto visibleFromLeft = std::ranges::all_of(std::ranges::views::iota(0, x), checkRow);
				auto visibleFromRight = std::ranges::all_of(std::ranges::views::iota(x + 1, static_cast<int>(grid.Width())), checkRow);

				const auto checkColumn = [&grid, x, current](int i) { return grid.at(x, i) < current; };
				auto visibleFromTop = std::ranges::all_of(std::ranges::views::iota(0, y), checkColumn);
				auto visibleFromBottom = std::ranges::all_of(std::ranges::views::iota(y + 1, static_cast<int>(grid.Height())), checkColumn);

				if (visibleFromLeft || visibleFromRight || visibleFromTop || visibleFromBottom)
				{
					++countVisible;
				}
			}
		}

		std::cout << countVisible;
	}
}