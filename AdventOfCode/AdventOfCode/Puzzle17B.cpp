#include "pch.h"
#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

namespace Puzzle17B
{
	struct CacheKey
	{
		static constexpr auto rowsInCacheKey = 500;

		int rocksOffset = 0;
		int jetPatternOffset = 0;
		std::string gridState;

		auto operator<=>(const CacheKey&) const = default;
	};

	struct CacheValue
	{
		size_t totalRocks = 0;
		size_t totalHeight = 0;
	};
}

IMPLEMENT_STD_HASH(Puzzle17B::CacheKey, value.rocksOffset, value.jetPatternOffset, value.gridState)

namespace Puzzle17B
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

		int GetRockOffset() const { return m_currentRock; }

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

		int GetJetPatternOffset() const { return m_currentJet; }

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

	std::string SerializeGridState(const Utilities::Grid2d<char>& grid, size_t maxHeight)
	{
		auto builder = std::stringstream{};
		for (auto y = maxHeight; y < CacheKey::rowsInCacheKey; ++y)
		{
			for (auto x = 1; x < grid.Width() - 1; ++x)
			{
				builder << grid.at(x, y);
			}
		}
		return builder.str();
	}

	void PrintSolution()
	{
		auto input = Utilities::ReadAllLinesInFile("Puzzle17.input");

		constexpr auto totalRocksToSimulate = 1000000000000;

		auto grid = CreateWorld(9, 20000);

		auto spawner = RockSpawner();
		auto jetPattern = JetPattern(input[0]);

		auto cache = std::unordered_map<CacheKey, CacheValue>{};
		auto bUsedCache = false;

		// Grid inverts y coordinates (downwards has increasing values of y).
		// maxHeightY is in grid coordinates and is distance from the bottom of the grid.
		// heightFromCache and totalHeight increase with height.
		auto maxHeightY = grid.Height() - 1;
		auto heightFromCache = 0ull;
		auto totalHeight = grid.Height() - 1 - maxHeightY + heightFromCache;

		// Simulate rocks until we have at least rowsInCacheKey in the grid. We need at least this many rows to start caching.
		//
		// Then start caching keyed on the most recent rowsInCacheKey rows of the grid, the current rock offset, and current jet offset.
		// In the cache store the current rocks and current height. When we successfully have a cache hit, compute the difference in 
		// current rocks and current height since then. Then repeatedly add to the rock count and height to get as close to
		// totalRocksToSimulate as possible. This doesn't change the simulation state, as the grid, rock offset, and jet offset are the same.
		//
		// Finally, simulate the remaining rocks.
		for (auto i = 0ull; i < totalRocksToSimulate; ++i)
		{
			if (!bUsedCache && maxHeightY + CacheKey::rowsInCacheKey < grid.Height() - 1)
			{
				// We have enough grid state to begin caching and doing cache lookups.
				auto cacheKey = CacheKey{ spawner.GetRockOffset(), jetPattern.GetJetPatternOffset(), SerializeGridState(grid, maxHeightY) };
				if (cache.contains(cacheKey))
				{
					const auto& cacheValue = cache.at(cacheKey);
					auto deltaRocks = i - cacheValue.totalRocks;
					auto deltaHeight = totalHeight - cacheValue.totalHeight;

					// Gotta go fast.
					constexpr auto goFastMultiplier = 100000;
					while (i + deltaRocks * goFastMultiplier < totalRocksToSimulate)
					{
						i += deltaRocks * goFastMultiplier;
						heightFromCache += deltaHeight * goFastMultiplier;
					}

					// Slow down and get as close as possible.
					while (i + deltaRocks < totalRocksToSimulate)
					{
						i += deltaRocks;
						heightFromCache += deltaHeight;
					}

					bUsedCache = true;
				}
				else
				{
					// Keep caching until we have our first hit.
					cache[cacheKey] = CacheValue{ i, totalHeight };
				}
			}

			// Simulate individual rocks.
			auto rock = spawner.GetNextRock(Point{ 3, static_cast<int>(maxHeightY) - 4 });
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
			maxHeightY = std::min(maxHeightY, static_cast<size_t>(highestPiece.y));
			totalHeight = grid.Height() - 1 - maxHeightY + heightFromCache;
			rock.WriteToGrid(grid, '#');
		}

		std::cout << totalHeight;
	}
}