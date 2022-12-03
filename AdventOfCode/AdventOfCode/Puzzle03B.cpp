#include "pch.h"
#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

namespace Puzzle03B
{
    void PrintSolution()
    {
		auto input = Utilities::ReadAllLinesInFile("Puzzle03.input");

		auto totalScore = 0;
		for (auto i = 0; i < input.size(); i += 3)
		{
			auto elfA = input[i];
			auto elfB = input[i + 1];
			auto elfC = input[i + 2];

			std::ranges::sort(elfA);
			std::ranges::sort(elfB);
			std::ranges::sort(elfC);

			auto commonBetweenAB = std::string{};
			std::ranges::set_intersection(elfA, elfB, std::back_inserter(commonBetweenAB));

			auto commonBetweenABC = std::string{};
			std::ranges::set_intersection(commonBetweenAB, elfC, std::back_inserter(commonBetweenABC));

			const auto [eraseFirst, eraseLast] = std::ranges::unique(commonBetweenABC);
			commonBetweenABC.erase(eraseFirst, eraseLast);

			totalScore += std::accumulate(commonBetweenABC.begin(), commonBetweenABC.end(), 0,
				[](int sum, char c)
				{
					return sum + ('a' <= c && c <= 'z' ? 1 + c - 'a' : 27 + c - 'A');
				});
		}

		std::cout << totalScore;
    }
}