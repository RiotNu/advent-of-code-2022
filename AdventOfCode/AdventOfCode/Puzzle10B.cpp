#include "pch.h"
#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

namespace Puzzle10B
{
	void PrintSolution()
	{
		auto input = Utilities::ReadAllLinesInFile("Puzzle10.input");

		auto currentValue = 1;
		auto allValues = std::vector<int>{};
		for (const auto& line : input)
		{
			if (line == "noop")
			{
				allValues.emplace_back(currentValue);
			}
			else
			{
				auto [_, arg] = scn::scan_tuple<int>(line, "addx {}");
				allValues.emplace_back(currentValue);
				allValues.emplace_back(currentValue);
				currentValue += arg;
			}
		}


		auto crt = Utilities::Grid2d<char>{ 40, 6 };
		std::ranges::fill(crt, '.');
		for (auto i = 0; i < crt.size(); ++i)
		{
			auto [pixel, _] = crt.GetCoordinatesFromIndex(i);
			auto x = allValues[i];
			if (x - 1 <= pixel && pixel <= x + 1)
			{
				crt.at(i) = '#';
			}
		}

		std::cout << crt;
	}
}