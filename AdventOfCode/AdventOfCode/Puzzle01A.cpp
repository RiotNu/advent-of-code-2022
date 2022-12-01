#include "pch.h"
#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

namespace Puzzle01A
{
	void PrintSolution()
	{
		auto input = Utilities::ReadAllLinesInFile("Puzzle01.input");

		// Add up the snack calories for each elf and find the highest total calories.
		constexpr auto sumSnackCaloriesForElf =
			[](const auto& currentElfInput)
		{
			auto elfSnackCalories = currentElfInput | std::views::transform([](const std::string& line) { return std::stoi(line); });
			return std::accumulate(elfSnackCalories.begin(), elfSnackCalories.end(), 0);
		};
		auto caloriesPerElfView = input | std::views::split("") | std::views::transform(sumSnackCaloriesForElf);
		auto maxCalories = std::ranges::max(caloriesPerElfView);

		std::cout << maxCalories;
	}
}