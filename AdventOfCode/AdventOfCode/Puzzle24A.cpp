#include "pch.h"
#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

namespace Puzzle24A
{
	struct Vec2
	{
		int x = 0;
		int y = 0;

		Vec2 operator+(const Vec2& other) const { return Vec2{ x + other.x, y + other.y }; }
		auto operator<=>(const Vec2&) const = default;
	};
}

IMPLEMENT_STD_HASH(Puzzle24A::Vec2, value.x, value.y);

namespace Puzzle24A
{
	struct Blizzard
	{
		char label;
		Vec2 position;
		Vec2 direction;

		void Tick(Utilities::Grid2d<char>& grid)
		{
			auto nextPosition = position + direction;

			// Handle case where blizzard hits wall.
			if (nextPosition.x == 0) { nextPosition.x = static_cast<int>(grid.Width()) - 2; }
			else if (nextPosition.x == grid.Width() - 1) { nextPosition.x = 1; }
			else if (nextPosition.y == 0) { nextPosition.y = static_cast<int>(grid.Height()) - 2; }
			else if (nextPosition.y == grid.Height() - 1) { nextPosition.y = 1; }

			position = nextPosition;

			// Write the label to the grid.
			auto& currentLabel = grid.at(position.x, position.y);
			if (currentLabel == '.')
			{
				currentLabel = label;
			}
			else if (currentLabel >= '0' && currentLabel <= '8')
			{
				currentLabel++;
			}
			else
			{
				currentLabel = '2';
			}
		}
	};

	auto ReadInput(const std::vector<std::string>& input)
	{
		auto blizzards = std::vector<Blizzard>{};
		auto initialState = Utilities::Grid2d<char>(input[0].size(), input.size());
		auto emptyGrid = Utilities::Grid2d<char>(input[0].size(), input.size());
		std::ranges::fill(emptyGrid, '.');

		for (auto y : std::ranges::views::iota(0, static_cast<int>(input.size())))
		{
			const auto& line = input[y];
			for (auto x : std::ranges::views::iota(0, static_cast<int>(line.size())))
			{
				initialState.at(x, y) = line[x];
				switch (line[x])
				{
				default:
					emptyGrid.at(x, y) = line[x];
					break;
				case '>':
					blizzards.emplace_back(line[x], Vec2(x, y), Vec2(1, 0));
					break;
				case '<':
					blizzards.emplace_back(line[x], Vec2(x, y), Vec2(-1, 0));
					break;
				case '^':
					blizzards.emplace_back(line[x], Vec2(x, y), Vec2(0, -1));
					break;
				case 'v':
					blizzards.emplace_back(line[x], Vec2(x, y), Vec2(0, 1));
					break;
				}
			}
		}

		auto target = Vec2(0, static_cast<int>(emptyGrid.Height()) - 1);
		for (auto x : std::ranges::views::iota(0, static_cast<int>(emptyGrid.Width())))
		{
			if (emptyGrid.at(x, target.y) == '.')
			{
				target.x = x;
				break;
			}
		}

		return std::make_tuple(blizzards, initialState, emptyGrid, target);
	}

	auto CalculateBlizzardStates(const auto& initialState, const auto& emptyGrid, auto& blizzards)
	{
		// Cycle length is least common multiple of the inner boundary of the grid, where blizzards move.
		auto cycleLength = std::lcm(emptyGrid.Width() - 2, emptyGrid.Height() - 2);
		auto blizzardStates = std::vector<Utilities::Grid2d<char>>{ initialState };
		for (auto i : std::ranges::views::iota(0, static_cast<int>(cycleLength)))
		{
			auto state = emptyGrid;
			for (auto& blizzard : blizzards)
			{
				blizzard.Tick(state);
			}
			blizzardStates.emplace_back(state);
		}

		return blizzardStates;
	}

	auto BreadthFirstSearch(const auto& blizzardStates, const auto& source, const auto& target)
	{
		struct QueueEntry
		{
			Vec2 position;
			std::vector<Vec2> history;
		};

		auto alreadyEvaluated = std::vector<std::unordered_set<Vec2>>{};
		auto explorationQueue = std::queue<QueueEntry>();
		explorationQueue.emplace(source, std::vector<Vec2>{ source });
		while (explorationQueue.size() > 0)
		{
			auto entry = explorationQueue.front();
			explorationQueue.pop();

			// Check if we've found the target.
			const auto& position = entry.position;
			if (position == target)
			{
				return entry.history;
			}

			// Keep a bucket for each minute so that we don't evaluate the same position and time repeatedly.
			auto minute = entry.history.size();
			while (alreadyEvaluated.size() < minute + 1)
			{
				alreadyEvaluated.emplace_back();
			}

			// Evaluate possible movements and enqueue any that are valid.
			auto enqueue =
				[&alreadyEvaluated, &explorationQueue, minute](const auto& nextPosition, auto nextHistory)
			{
				if (!alreadyEvaluated[minute].contains(nextPosition))
				{
					alreadyEvaluated[minute].insert(nextPosition);
					nextHistory.emplace_back(nextPosition);
					explorationQueue.emplace(nextPosition, std::move(nextHistory));
				}
			};

			const auto& nextBlizzardState = blizzardStates[minute % blizzardStates.size()];
			if (position.x > 0 && nextBlizzardState.at(position.x - 1, position.y) == '.')
			{
				enqueue(Vec2(position.x - 1, position.y), entry.history);
			}
			if (position.x < nextBlizzardState.Width() - 1 && nextBlizzardState.at(position.x + 1, position.y) == '.')
			{
				enqueue(Vec2(position.x + 1, position.y), entry.history);
			}
			if (position.y > 0 && nextBlizzardState.at(position.x, position.y - 1) == '.')
			{
				enqueue(Vec2(position.x, position.y - 1), entry.history);
			}
			if (position.y < nextBlizzardState.Height() - 1 && nextBlizzardState.at(position.x, position.y + 1) == '.')
			{
				enqueue(Vec2(position.x, position.y + 1), entry.history);
			}
			if (nextBlizzardState.at(position.x, position.y) == '.')
			{
				enqueue(Vec2(position.x, position.y), entry.history);
			}
		}

		// No path found.
		Utilities::VerifyNotReached();
		return std::vector<Vec2>{};
	}

	void PrintSolution()
	{
		auto [blizzards, initialState, emptyGrid, target] = ReadInput(Utilities::ReadAllLinesInFile("Puzzle24.input"));
		auto blizzardStates = CalculateBlizzardStates(initialState, emptyGrid, blizzards);
		auto source = Vec2(1, 0);
		auto bestPath = BreadthFirstSearch(blizzardStates, source, target);
		std::cout << bestPath.size() - 1;
	}
}