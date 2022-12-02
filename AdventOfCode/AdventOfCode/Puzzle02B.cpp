#include "pch.h"
#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

namespace Puzzle02B
{
	enum class Move { Rock, Paper, Scissors };
	enum class Result { Loss, Draw, Win };

	const std::unordered_map<Move, int> moveToPoints = { { Move::Rock, 1 }, { Move::Paper, 2 }, { Move::Scissors, 3 } };
	const std::unordered_map<Result, int> resultToPoints = { { Result::Loss, 0 }, { Result::Draw, 3 }, { Result::Win, 6 } };
	const std::unordered_map<Move, Move> moveToWin = { { Move::Rock, Move::Paper }, { Move::Paper, Move::Scissors }, { Move::Scissors, Move::Rock } };
	const std::unordered_map<Move, Move> moveToLose = { { Move::Rock, Move::Scissors }, { Move::Paper, Move::Rock }, { Move::Scissors, Move::Paper } };

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

IMPLEMENT_STD_HASH(Puzzle02B::MovePair, value.m_playerMove, value.m_opponentMove);

namespace Puzzle02B
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
			auto desiredResult = static_cast<Result>(line[2] - 'X');

			auto playerMove = opponentMove;
			if (desiredResult == Result::Loss)
			{
				playerMove = moveToLose.at(opponentMove);
			}
			else if (desiredResult == Result::Win)
			{
				playerMove = moveToWin.at(opponentMove);
			}

			score += moveToPoints.at(playerMove);
			score += resultToPoints.at(desiredResult);
		}

		std::cout << score;
	}
}