#include "pch.h"
#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

namespace Puzzle01A
{
	void PrintSolution()
	{
		auto input = Utilities::ReadAllLinesInFile("Puzzle01.input");

		auto currentMaxCalories = 0;
		for (auto i = 0; i < input.size(); ++i)
		{
			auto currentCalories = 0;
			while (i != input.size() && input[i].size() != 0)
			{
				currentCalories += std::stoi(input[i++]);
			}

			currentMaxCalories = std::max(currentCalories, currentMaxCalories);
		}

		std::cout << currentMaxCalories;
	}
}