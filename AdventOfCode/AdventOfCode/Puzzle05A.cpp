#include "pch.h"
#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

namespace Puzzle05A
{
    void PrintSolution()
    {
        auto input = Utilities::ReadAllLinesInFile("Puzzle05.input");

        // Find the empty line in our input.
        auto blankIndex = -1;
        for (auto i = 0; i < input.size(); ++i)
        {
            if (input[i].size() == 0)
            {
                blankIndex = i;
                break;
            }
        }

        // Setup initial state.
        auto stacks = std::vector<std::stack<char>>{};
        const auto& stackLabels = input[blankIndex - 1];
        for (auto i = 0; i < stackLabels.size(); ++i)
        {
            if (stackLabels[i] == ' ')
            {
                continue;
            }

            auto currentStack = std::stack<char>{};
            for (auto lineIndex = blankIndex - 2; lineIndex >= 0; --lineIndex)
            {
                const auto& line = input[lineIndex];
                const auto& crate = line[i];
                if (crate == ' ')
                {
                    break;
                }

                currentStack.push(crate);
            }

            stacks.emplace_back(currentStack);
        }

        // Process moves.
        for (auto i = blankIndex + 1; i < input.size(); ++i)
        {
            const auto& line = input[i];
            auto tokens = Utilities::SplitString(line, " ");
            auto count = std::stoi(tokens[1]);
            auto from = std::stoi(tokens[3]) - 1;
            auto to = std::stoi(tokens[5]) - 1;

            while (count-- > 0)
            {
                auto crate = stacks[from].top();
                stacks[from].pop();
                stacks[to].push(crate);
            }
        }

        // Build output string from top of each stack.
        auto output = std::stringstream{};
        for (const auto& currentStack : stacks)
        {
            output << currentStack.top();
        }

        std::cout << output.str();
    }
}