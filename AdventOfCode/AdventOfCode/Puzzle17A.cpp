#include "pch.h"
#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

namespace Puzzle17A
{
	struct Point
	{
		int x = 0;
		int y = 0;

		Point& operator+=(const Point& other) { x += other.x; y += other.y; return *this; }
		Point operator+(const Point& other) const { return Point{ x + other.x, y + other.y }; }
	};

	struct Rock
	{
		// Piece offsets are relative to a bottom-left corner anchor
		Point anchor;
		int pieces;
		std::array<Point, 5> pieceOffsets;

		bool MoveLeft(const Utilities::Grid2d<char>& grid) { return Move(grid, Point{ -1, 0 }); }
		bool MoveRight(const Utilities::Grid2d<char>& grid) { return Move(grid, Point{ 1, 0 }); }
		bool MoveDown(const Utilities::Grid2d<char>& grid) { return Move(grid, Point{ 0, 1 }); }
		bool MoveUp(const Utilities::Grid2d<char>& grid) { return Move(grid, Point{ 0, -1 }); }

		void WriteToGrid(Utilities::Grid2d<char>& grid, char character)
		{
			for (const auto& pieceOffset : pieceOffsets)
			{
				auto piecePosition = anchor + pieceOffset;
				grid.at(piecePosition.x, piecePosition.y) = character;
			}
		}

		bool Move(const Utilities::Grid2d<char>& grid, const Point& offset)
		{
			for (const auto& pieceOffset : pieceOffsets)
			{
				auto updatedPiecePosition = anchor + pieceOffset + offset;
				if (grid.at(updatedPiecePosition.x, updatedPiecePosition.y) != '.')
				{
					return false;
				}
			}

			anchor += offset;
			return true;
		}

		Point GetHighestPiece() const
		{
			auto highestPiece = anchor + pieceOffsets[0];
			for (const auto& pieceOffset : pieceOffsets)
			{
				auto piecePosition = anchor + pieceOffset;
				if (piecePosition.y < highestPiece.y)
				{
					highestPiece = piecePosition;
				}
			}
			return highestPiece;
		}
	};

	// ####
	constexpr auto rockHorizontal = Rock{ Point{0, 0}, 4, {Point{0, 0}, Point{1, 0}, Point{2, 0}, Point{3, 0}} };

	// #
	// #
	// #
	// #
	constexpr auto rockVertical = Rock{ Point{0, 3}, 4, {Point{0, -3}, Point{0, -2}, Point{0, -1}, Point{0, 0}} };

	// .#.
	// ###
	// .#.
	constexpr auto rockPlus = Rock{ Point{0, 2}, 5, {Point{1, -2}, Point{0, -1}, Point{1, -1}, Point{2, -1}, Point{1, 0}} };

	// ..#
	// ..#
	// ###
	constexpr auto rockMirrorL = Rock{ Point{0, 2}, 5, {Point{2, -2}, Point{2, -1}, Point{0, 0}, Point{1, 0}, Point{2, 0}} };

	// ##
	// ##
	constexpr auto rockSquare = Rock{ Point{0, 1}, 4, {Point{0, -1}, Point{1, -1}, Point{0, 0}, Point{1, 0}} };

	struct RockSpawner
	{
		Rock GetNextRock(const Point& position)
		{
			auto rock = m_rocks[m_currentRock++];
			rock.anchor = position;
			if (m_currentRock == m_rocks.size()) { m_currentRock = 0; }
			return rock;
		}

	private:
		int m_currentRock = 0;
		std::array<Rock, 5> m_rocks = { rockHorizontal, rockPlus, rockMirrorL, rockVertical, rockSquare };
	};

	struct JetPattern
	{
		JetPattern(const std::string& jetPattern) : m_jetPattern(jetPattern) {}

		char GetNextJet()
		{
			auto jet = m_jetPattern[m_currentJet++];
			if (m_currentJet == m_jetPattern.size()) { m_currentJet = 0; }
			return jet;
		}

	private:
		int m_currentJet = 0;
		std::string m_jetPattern;
	};

	Utilities::Grid2d<char> CreateWorld(int width, int height)
	{
		auto grid = Utilities::Grid2d<char>(width, height);

		std::ranges::fill(grid, '.');
		grid.at(0, grid.Height() - 1) = '+';
		grid.at(grid.Width() - 1, grid.Height() - 1) = '+';

		for (auto y = 0; y < grid.Height() - 1; ++y)
		{
			grid.at(0, y) = '|';
			grid.at(grid.Width() - 1, y) = '|';
		}

		for (auto x = 1; x < grid.Width() - 1; ++x)
		{
			grid.at(x, grid.Height() - 1) = '-';
		}

		return grid;
	}

	void PrintSolution()
	{
		auto input = Utilities::ReadAllLinesInFile("Puzzle17.input");

		constexpr auto totalRocksToSimulate = 2022;
		constexpr auto maxRockHeight = 4;
		auto grid = CreateWorld(9, totalRocksToSimulate * maxRockHeight + 1);
	
		auto spawner = RockSpawner();
		auto jetPattern = JetPattern(input[0]);
		auto maxHeight = static_cast<int>(grid.Height()) - 1;
		for (auto i = 0; i < totalRocksToSimulate; ++i)
		{
			auto rock = spawner.GetNextRock(Point{3, maxHeight - 4});
			auto bRockMoving = true;
			while (bRockMoving)
			{
				auto jet = jetPattern.GetNextJet();
				if (jet == '<')
				{
					rock.MoveLeft(grid);
				}
				else
				{
					rock.MoveRight(grid);
				}

				bRockMoving = rock.MoveDown(grid);
			}

			auto highestPiece = rock.GetHighestPiece();
			maxHeight = std::min(maxHeight, highestPiece.y);
			rock.WriteToGrid(grid, '#');
		}

		std::cout << grid.Height() - 1 - maxHeight;
	}
}