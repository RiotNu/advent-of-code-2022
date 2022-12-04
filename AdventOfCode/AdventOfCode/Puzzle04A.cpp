#include "pch.h"
#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

namespace Puzzle04A
{
    void PrintSolution()
    {
        auto input = Utilities::ReadAllLinesInFile("Puzzle04.input");

        auto fullyContained = 0;
        for (const auto& line : input)
        {
            auto elves = Utilities::SplitString(line, ",");

            constexpr auto transform = [](const std::string& str) { return std::stoi(str); };
            auto left = Utilities::SplitStringAndTransform<int>(elves[0], "-", transform);
            auto right = Utilities::SplitStringAndTransform<int>(elves[1], "-", transform);

            if (left[0] <= right[0] && right[1] <= left[1] || right[0] <= left[0] && left[1] <= right[1])
            {
                ++fullyContained;
            }
        }

        std::cout << fullyContained;
    }
}