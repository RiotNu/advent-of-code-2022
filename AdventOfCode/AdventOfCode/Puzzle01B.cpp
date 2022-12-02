#include "pch.h"
#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

namespace Puzzle01B
{
	void PrintSolution()
	{
		auto input = Utilities::ReadAllLinesInFile("Puzzle01.input");

		// Track the total snack calories for each elf.
		constexpr auto sumSnackCaloriesForElf =
			[](const auto& currentElfInput)
			{
				auto elfSnackCalories = currentElfInput | std::views::transform([](const std::string& line) { return std::stoi(line); });
				return std::accumulate(elfSnackCalories.begin(), elfSnackCalories.end(), 0);
			};
		auto caloriesPerElf = input | std::views::split("") | std::views::transform(sumSnackCaloriesForElf) | std::ranges::to<std::vector>();

		// Sum the total calories from the three elves carrying the most snacks.
		std::ranges::sort(caloriesPerElf, std::ranges::greater());
		auto topThreeCaloriesView = caloriesPerElf | std::ranges::views::take(3);
		auto sumOfTopThreeCalories = std::accumulate(topThreeCaloriesView.begin(), topThreeCaloriesView.end(), 0);

		std::cout << sumOfTopThreeCalories;
	}
}