#include "pch.h"
#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

namespace Puzzle10A
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

		auto signalStrength = 0;
		for (auto i = 19; i <= 219; i += 40)
		{
			signalStrength += (i + 1) * allValues[i];
		}

		std::cout << signalStrength;
	}
}