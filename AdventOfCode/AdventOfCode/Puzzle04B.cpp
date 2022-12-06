#include "pch.h"
#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

namespace Puzzle04B
{
	void PrintSolution()
	{
		std::cout << ranges::accumulate(
			Utilities::ReadAllLinesInFile("Puzzle04.input"),
			0,
			[](int sum, const std::string& line)
			{
				auto [_, leftBegin, leftEnd, rightBegin, rightEnd] = scn::scan_tuple<int, int, int, int>(line, "{}-{},{}-{}");
				return (leftBegin <= rightBegin && rightBegin <= leftEnd || rightBegin <= leftBegin && leftBegin <= rightEnd)
					? sum + 1
					: sum;
			});
	}
}