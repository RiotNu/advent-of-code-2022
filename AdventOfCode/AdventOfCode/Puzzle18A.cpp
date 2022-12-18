#include "pch.h"
#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

namespace Puzzle18A
{
	void PrintSolution()
	{
		auto grid = Utilities::Grid3d<int>{ 20, 20, 20 };
		for (const auto& line : Utilities::ReadAllLinesInFile("Puzzle18.input"))
		{
			auto coordinates = Utilities::SplitCommaSeparatedInt32s(line);
			grid.at(coordinates[0], coordinates[1], coordinates[2]) = 1;
		}

		auto exposedFaces = 0;
		for (auto i = 0; i < grid.size(); ++i)
		{
			if (grid.at(i) == 1)
			{
				auto currentFaces = 6;
				auto [x, y, z] = grid.GetCoordinatesFromIndex(i);
				currentFaces -= x != 0 ? grid.at(x - 1, y, z) : 0;
				currentFaces -= x + 1 < grid.Width() ? grid.at(x + 1, y, z) : 0;
				currentFaces -= y != 0 ? grid.at(x, y - 1, z) : 0;
				currentFaces -= y + 1 < grid.Height() ? grid.at(x, y + 1, z) : 0;
				currentFaces -= z != 0 ? grid.at(x, y, z - 1) : 0;
				currentFaces -= z + 1 < grid.Depth() ? grid.at(x, y, z + 1) : 0;
				exposedFaces += currentFaces;
			}
		}

		std::cout << exposedFaces;
	}
}