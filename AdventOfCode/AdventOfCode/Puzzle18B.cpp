#include "pch.h"
#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

namespace Puzzle18B
{
	enum class Material { Air, Lava, Steam };

	void ExpandSteam(Utilities::Grid3d<Material>& grid, int x, int y, int z)
	{
		auto toVisit = std::queue<size_t>{};
		toVisit.push(grid.GetIndexFromCoordinates(x, y, z));
		while (!toVisit.empty())
		{
			auto next = toVisit.front();
			toVisit.pop();

			if (grid.at(next) != Material::Air)
			{
				continue;
			}

			grid.at(next) = Material::Steam;

			auto [x, y, z] = grid.GetCoordinatesFromIndex(next);
			if (x != 0)                { toVisit.push(grid.GetIndexFromCoordinates(x - 1, y,     z)); }
			if (x + 1 < grid.Width())  { toVisit.push(grid.GetIndexFromCoordinates(x + 1, y,     z)); }
			if (y != 0)                { toVisit.push(grid.GetIndexFromCoordinates(x,     y - 1, z)); }
			if (y + 1 < grid.Height()) { toVisit.push(grid.GetIndexFromCoordinates(x,     y + 1, z)); }
			if (z != 0)                { toVisit.push(grid.GetIndexFromCoordinates(x,     y,     z - 1)); }
			if (z + 1 < grid.Depth())  { toVisit.push(grid.GetIndexFromCoordinates(x,     y,     z + 1)); }
		}
	}

	void PrintSolution()
	{
		auto grid = Utilities::Grid3d<Material>{ 21, 21, 21 };
		for (const auto& line : Utilities::ReadAllLinesInFile("Puzzle18.input"))
		{
			auto coordinates = Utilities::SplitCommaSeparatedInt32s(line);

			// Translate input by (1, 1, 1) to guarantee we can create first steam at (0, 0, 0).
			grid.at(coordinates[0] + 1, coordinates[1] + 1, coordinates[2] + 1) = Material::Lava;
		}

		// Expand steam through all reachable air.
		ExpandSteam(grid, 0, 0, 0);

		// Convert any remaining air into lava.
		for (auto i = 0; i < grid.size(); ++i)
		{
			if (grid.at(i) == Material::Air)
			{
				grid.at(i) = Material::Lava;
			}
		}

		// Count remaining exposed faces.
		auto exposedFaces = 0;
		for (auto i = 0; i < grid.size(); ++i)
		{
			if (grid.at(i) == Material::Lava)
			{
				auto currentFaces = 6;
				auto [x, y, z] = grid.GetCoordinatesFromIndex(i);
				if (grid.at(x - 1, y, z) == Material::Lava)                          { currentFaces--; }
				if (x + 1 < grid.Width() && grid.at(x + 1, y, z) == Material::Lava)  { currentFaces--; }
				if (grid.at(x, y - 1, z) == Material::Lava)                          { currentFaces--; }
				if (y + 1 < grid.Height() && grid.at(x, y + 1, z) == Material::Lava) { currentFaces--; }
				if (grid.at(x, y, z - 1) == Material::Lava)                          { currentFaces--; }
				if (z + 1 < grid.Depth() && grid.at(x, y, z + 1) == Material::Lava)  { currentFaces--; }
				exposedFaces += currentFaces;
			}
		}

		std::cout << exposedFaces;
	}
}