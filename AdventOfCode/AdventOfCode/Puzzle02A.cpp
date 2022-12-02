#include "pch.h"
#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

namespace Puzzle02A
{
	enum class Move { Rock, Paper, Scissors };
	enum class Result { Loss, Draw, Win };

	const std::unordered_map<Move, int> moveToPoints = { { Move::Rock, 1 }, { Move::Paper, 2 }, { Move::Scissors, 3 } };
	const std::unordered_map<Result, int> resultToPoints = { { Result::Loss, 0 }, { Result::Draw, 3 }, { Result::Win, 6 } };

	struct MovePair
	{
		Move m_playerMove;
		Move m_opponentMove;

		bool operator==(const MovePair& other) const
		{
			return m_playerMove == other.m_playerMove && m_opponentMove == other.m_opponentMove;
		}
	};
}

IMPLEMENT_STD_HASH(Puzzle02A::MovePair, value.m_playerMove, value.m_opponentMove);

namespace Puzzle02A
{
	const std::unordered_map<MovePair, Result> movesToResult =
	{
		{ { Move::Rock, Move::Paper }, Result::Loss },
		{ { Move::Rock, Move::Rock }, Result::Draw },
		{ { Move::Rock, Move::Scissors }, Result::Win },
		{ { Move::Paper, Move::Scissors }, Result::Loss },
		{ { Move::Paper, Move::Paper }, Result::Draw },
		{ { Move::Paper, Move::Rock }, Result::Win },
		{ { Move::Scissors, Move::Rock }, Result::Loss },
		{ { Move::Scissors, Move::Scissors }, Result::Draw },
		{ { Move::Scissors, Move::Paper }, Result::Win },
	};

	void PrintSolution()
	{
		auto input = Utilities::ReadAllLinesInFile("Puzzle02.input");

		auto score = 0;
		for (const auto& line : input)
		{
			auto opponentMove = static_cast<Move>(line[0] - 'A');
			auto playerMove = static_cast<Move>(line[2] - 'X');

			score += moveToPoints.at(playerMove);
			auto result = movesToResult.at(MovePair{ playerMove, opponentMove });
			score += resultToPoints.at(result);
		}

		std::cout << score;
	}
}