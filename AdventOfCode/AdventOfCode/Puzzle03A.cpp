#include "pch.h"
#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

namespace Puzzle03A
{
    void PrintSolution()
    {
        auto input = Utilities::ReadAllLinesInFile("Puzzle03.input");

        auto totalScore = 0;
        for (const auto& line : input)
        {
            auto halfLineSize = line.size() / 2;
            auto left = line.substr(0, halfLineSize);
            auto right = line.substr(halfLineSize, halfLineSize);

            std::ranges::sort(left);
            std::ranges::sort(right);

            auto common = std::string{};
            std::ranges::set_intersection(left, right, std::back_inserter(common));

            const auto [eraseFirst, eraseLast] = std::ranges::unique(common);
            common.erase(eraseFirst, eraseLast);

            totalScore += std::accumulate(common.begin(), common.end(), 0, 
                [](int sum, char c)
                {
                    return sum + ('a' <= c && c <= 'z' ? 1 + c - 'a' : 27 + c - 'A' );
                });
        }

        std::cout << totalScore;
    }
}