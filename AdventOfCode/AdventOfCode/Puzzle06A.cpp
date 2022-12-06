#include "pch.h"
#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

namespace Puzzle06A
{
    void PrintSolution()
    {
        auto input = Utilities::ReadAllLinesInFile("Puzzle06.input");
        const auto& line = input[0];

        constexpr int desiredPacketSize = 4;
        auto packetEndPosition = -1;
        for (auto i = 0; i < line.size() - desiredPacketSize; ++i)
        {
            auto packet = line.substr(i, desiredPacketSize);
            std::ranges::sort(packet);
            auto [first, last] = std::ranges::unique(packet);
            packet.erase(first, last);
            if (packet.size() == desiredPacketSize)
            {
                packetEndPosition = i + desiredPacketSize;
                break;
            }
        }

        std::cout << packetEndPosition;
    }
}