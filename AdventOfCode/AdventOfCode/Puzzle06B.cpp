#include "pch.h"
#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

namespace Puzzle06B
{
	void PrintSolution()
	{
		auto input = Utilities::ReadAllLinesInFile("Puzzle06.input");
		const auto& line = input[0];

		constexpr int desiredHeaderSize = 14;
		auto headerEndIndex = -1;
		for (auto i = 0; i < line.size() - desiredHeaderSize; ++i)
		{
			auto header = line.substr(i, desiredHeaderSize);
			std::ranges::sort(header);

			auto [first, last] = std::ranges::unique(header);
			header.erase(first, last);

			if (header.size() == desiredHeaderSize)
			{
				headerEndIndex = i + desiredHeaderSize;
				break;
			}
		}

		std::cout << headerEndIndex;
	}
}