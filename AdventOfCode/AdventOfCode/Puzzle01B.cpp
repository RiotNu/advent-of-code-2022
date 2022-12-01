#include "pch.h"
#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

namespace Puzzle01B
{
	void PrintSolution()
	{
		auto input = Utilities::ReadAllLinesInFile("Puzzle01.input");

		auto calorieCounts = std::vector<int>{};
		for (auto i = 0; i < input.size(); ++i)
		{
			auto currentCalories = 0;
			while (i != input.size() && input[i].size() != 0)
			{
				currentCalories += std::stoi(input[i++]);
			}

			calorieCounts.emplace_back(currentCalories);
		}

		std::sort(calorieCounts.begin(), calorieCounts.end());
		
		auto sumOfTopThreeCalories =
			calorieCounts[calorieCounts.size() - 1]
			+ calorieCounts[calorieCounts.size() - 2]
			+ calorieCounts[calorieCounts.size() - 3];

		std::cout << sumOfTopThreeCalories;
	}
}