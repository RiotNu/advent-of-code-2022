#include "pch.h"
#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

namespace Puzzle22B
{
	struct Vec2
	{
		int x = 0;
		int y = 0;

		Vec2 operator+(const Vec2& other) const { return Vec2{ x + other.x, y + other.y }; }
		auto operator<=>(const Vec2&) const = default;
	};

	constexpr auto left  = Vec2{ -1,  0 };
	constexpr auto right = Vec2{ 1,  0 };
	constexpr auto up    = Vec2{ 0, -1 };
	constexpr auto down  = Vec2{ 0,  1 };

	enum class CubeFace { A, B, C, D, E, F };

	struct FaceDirectionPair
	{
		CubeFace face = CubeFace::A;
		Vec2 direction = right;

		auto operator<=>(const FaceDirectionPair&) const = default;
	};
}

IMPLEMENT_STD_HASH(Puzzle22B::Vec2, value.x, value.y);
IMPLEMENT_STD_HASH(Puzzle22B::FaceDirectionPair, value.face, value.direction);

namespace Puzzle22B
{
	// Faces for sample input
	//                 A A A A        
	//                 A A A A        
	//                 A A A A        
	//                 A A A A        
	// B B B B C C C C D D D D        
	// B B B B C C C C D D D D        
	// B B B B C C C C D D D D        
	// B B B B C C C C D D D D        
	//                 E E E E F F F F
	//                 E E E E F F F F
	//                 E E E E F F F F
	//                 E E E E F F F F
	const auto sampleMapping = std::unordered_map<FaceDirectionPair, FaceDirectionPair>
	{
		{ FaceDirectionPair{CubeFace::A, left},  FaceDirectionPair{CubeFace::C, down}  },
		{ FaceDirectionPair{CubeFace::A, right}, FaceDirectionPair{CubeFace::F, left}  },
		{ FaceDirectionPair{CubeFace::A, up},    FaceDirectionPair{CubeFace::B, down}  },
		{ FaceDirectionPair{CubeFace::A, down},  FaceDirectionPair{CubeFace::D, down}  },

		{ FaceDirectionPair{CubeFace::B, left},  FaceDirectionPair{CubeFace::F, up}    },
		{ FaceDirectionPair{CubeFace::B, right}, FaceDirectionPair{CubeFace::C, right} },
		{ FaceDirectionPair{CubeFace::B, up},    FaceDirectionPair{CubeFace::A, down}  },
		{ FaceDirectionPair{CubeFace::B, down},  FaceDirectionPair{CubeFace::E, up}    },

		{ FaceDirectionPair{CubeFace::C, left},  FaceDirectionPair{CubeFace::B, left}  },
		{ FaceDirectionPair{CubeFace::C, right}, FaceDirectionPair{CubeFace::D, right} },
		{ FaceDirectionPair{CubeFace::C, up},    FaceDirectionPair{CubeFace::A, right} },
		{ FaceDirectionPair{CubeFace::C, down},  FaceDirectionPair{CubeFace::E, right} },

		{ FaceDirectionPair{CubeFace::D, left},  FaceDirectionPair{CubeFace::C, left}  },
		{ FaceDirectionPair{CubeFace::D, right}, FaceDirectionPair{CubeFace::F, down}  },
		{ FaceDirectionPair{CubeFace::D, up},    FaceDirectionPair{CubeFace::A, up}    },
		{ FaceDirectionPair{CubeFace::D, down},  FaceDirectionPair{CubeFace::E, down}  },

		{ FaceDirectionPair{CubeFace::E, left},  FaceDirectionPair{CubeFace::C, up}    },
		{ FaceDirectionPair{CubeFace::E, right}, FaceDirectionPair{CubeFace::F, right} },
		{ FaceDirectionPair{CubeFace::E, up},    FaceDirectionPair{CubeFace::D, up}    },
		{ FaceDirectionPair{CubeFace::E, down},  FaceDirectionPair{CubeFace::B, up}    },

		{ FaceDirectionPair{CubeFace::F, left},  FaceDirectionPair{CubeFace::E, left}  },
		{ FaceDirectionPair{CubeFace::F, right}, FaceDirectionPair{CubeFace::A, left}  },
		{ FaceDirectionPair{CubeFace::F, up},    FaceDirectionPair{CubeFace::D, left}  },
		{ FaceDirectionPair{CubeFace::F, down},  FaceDirectionPair{CubeFace::B, right} },
	};
	constexpr auto sampleDimensions = 4;
	constexpr auto sampleXShifts = std::array<int, 6>{ 2 * sampleDimensions, 0, sampleDimensions, 2 * sampleDimensions, 2 * sampleDimensions, 3 * sampleDimensions };
	constexpr auto sampleYShifts = std::array<int, 6>{ 0, sampleDimensions, sampleDimensions, sampleDimensions, 2 * sampleDimensions, 2 * sampleDimensions         };


	// Faces for real input
	//         A A A A B B B B
	//         A A A A B B B B
	//         A A A A B B B B
	//         A A A A B B B B
	//         C C C C         
	//         C C C C         
	//         C C C C         
	//         C C C C         
	// D D D D E E E E         
	// D D D D E E E E         
	// D D D D E E E E         
	// D D D D E E E E         
	// F F F F                 
	// F F F F                 
	// F F F F                 
	// F F F F                 
	const auto actualMapping = std::unordered_map<FaceDirectionPair, FaceDirectionPair>
	{
		{ FaceDirectionPair{CubeFace::A, left},  FaceDirectionPair{CubeFace::D, right} },
		{ FaceDirectionPair{CubeFace::A, right}, FaceDirectionPair{CubeFace::B, right} },
		{ FaceDirectionPair{CubeFace::A, up},    FaceDirectionPair{CubeFace::F, right} },
		{ FaceDirectionPair{CubeFace::A, down},  FaceDirectionPair{CubeFace::C, down}  },

		{ FaceDirectionPair{CubeFace::B, left},  FaceDirectionPair{CubeFace::A, left}  },
		{ FaceDirectionPair{CubeFace::B, right}, FaceDirectionPair{CubeFace::E, left}  },
		{ FaceDirectionPair{CubeFace::B, up},    FaceDirectionPair{CubeFace::F, up}    },
		{ FaceDirectionPair{CubeFace::B, down},  FaceDirectionPair{CubeFace::C, left}  },

		{ FaceDirectionPair{CubeFace::C, left},  FaceDirectionPair{CubeFace::D, down}  },
		{ FaceDirectionPair{CubeFace::C, right}, FaceDirectionPair{CubeFace::B, up}    },
		{ FaceDirectionPair{CubeFace::C, up},    FaceDirectionPair{CubeFace::A, up}    },
		{ FaceDirectionPair{CubeFace::C, down},  FaceDirectionPair{CubeFace::E, down}  },
 
		{ FaceDirectionPair{CubeFace::D, left},  FaceDirectionPair{CubeFace::A, right} },
		{ FaceDirectionPair{CubeFace::D, right}, FaceDirectionPair{CubeFace::E, right} },
		{ FaceDirectionPair{CubeFace::D, up},    FaceDirectionPair{CubeFace::C, right} },
		{ FaceDirectionPair{CubeFace::D, down},  FaceDirectionPair{CubeFace::F, down}  },

		{ FaceDirectionPair{CubeFace::E, left},  FaceDirectionPair{CubeFace::D, left}  },
		{ FaceDirectionPair{CubeFace::E, right}, FaceDirectionPair{CubeFace::B, left}  },
		{ FaceDirectionPair{CubeFace::E, up},    FaceDirectionPair{CubeFace::C, up}    },
		{ FaceDirectionPair{CubeFace::E, down},  FaceDirectionPair{CubeFace::F, left}  },

		{ FaceDirectionPair{CubeFace::F, left},  FaceDirectionPair{CubeFace::A, down}  },
		{ FaceDirectionPair{CubeFace::F, right}, FaceDirectionPair{CubeFace::E, up}    },
		{ FaceDirectionPair{CubeFace::F, up},    FaceDirectionPair{CubeFace::D, up}    },
		{ FaceDirectionPair{CubeFace::F, down},  FaceDirectionPair{CubeFace::B, down}  },
	};
	constexpr auto actualDimensions = 50;
	constexpr auto actualXShifts = std::array<int, 6>{ actualDimensions, 2 * actualDimensions, actualDimensions, 0, actualDimensions, 0 };
	constexpr auto actualYShifts = std::array<int, 6>{ 0, 0, actualDimensions, 2 * actualDimensions, 2 * actualDimensions, 3 * actualDimensions };

	// Select whether running against sample input or real input.
	constexpr auto& mapping = actualMapping;
	constexpr auto& dim = actualDimensions;
	constexpr auto& xShifts = actualXShifts;
	constexpr auto& yShifts = actualYShifts;

	struct Maze
	{
		Maze(const auto& inputView)
		{
			for (auto i = 0; i < 6; ++i)
			{
				auto face = Utilities::Grid2d<char>(dim, dim);
				auto shiftX = xShifts[i];
				auto shiftY = yShifts[i];
				for (auto y = 0; y < face.Height(); ++y)
				{
					const auto& line = inputView[y + shiftY];
					for (auto x = 0; x < face.Width(); ++x)
					{
						face.at(x, y) = line[x + shiftX];
					}
				}
				m_faces.emplace_back(face);
			}
		}

		void TurnLeft()
		{
			if (m_direction == left)       { m_direction = down; }
			else if (m_direction == right) { m_direction = up; }
			else if (m_direction == up)    { m_direction = left; }
			else if (m_direction == down)  { m_direction = right; }
			else { Utilities::VerifyNotReached(); }
		}

		void TurnRight()
		{
			if (m_direction == left)       { m_direction = up; }
			else if (m_direction == right) { m_direction = down; }
			else if (m_direction == up)    { m_direction = right; }
			else if (m_direction == down)  { m_direction = left; }
			else { Utilities::VerifyNotReached(); }
		}

		void Move(int distance)
		{
			while (distance-- > 0)
			{
				auto* currentFace = &m_faces[static_cast<int>(m_faceLabel)];
				auto* nextFace = currentFace;
				auto nextFaceLabel = m_faceLabel;
				auto next = m_position + m_direction;
				auto nextDirection = m_direction;

				// Check if we need to transfer to a different face of the cube.
				if (m_direction == left && next.x < 0
					|| m_direction == right && next.x >= currentFace->Width()
					|| m_direction == up && next.y < 0
					|| m_direction == down && next.y >= currentFace->Height())
				{
					auto sourceFace = FaceDirectionPair{ m_faceLabel, m_direction };
					auto target = mapping.at(sourceFace);
					nextFaceLabel = target.face;
					nextFace = &m_faces[static_cast<int>(nextFaceLabel)];
					nextDirection = target.direction;
					next = TransferFaces(m_position, m_direction, nextDirection);
				}

				// Trace path for debugging.
				if (m_direction == right)     { currentFace->at(m_position.x, m_position.y) = '>'; }
				else if (m_direction == left) { currentFace->at(m_position.x, m_position.y) = '<'; }
				else if (m_direction == up)   { currentFace->at(m_position.x, m_position.y) = '^'; }
				else if (m_direction == down) { currentFace->at(m_position.x, m_position.y) = 'v'; }

				// Stop if we hit a wall.
				if (nextFace->at(next.x, next.y) == '#')
				{
					return;
				}

				// If clear, continue to next position.
				m_faceLabel = nextFaceLabel;
				m_direction = nextDirection;
				m_position = next;
			}
		}

		Vec2 TransferFaces(const Vec2& position, const Vec2& currentDirection, const Vec2& nextDirection)
		{
			if (currentDirection == left)
			{
				Utilities::VerifyElseCrash(position.x == 0);
				if (nextDirection == left) { return Vec2(dim - 1, position.y); }
				if (nextDirection == right) { return Vec2(0, dim - 1 - position.y); }
				if (nextDirection == up) { return Vec2(dim - 1 - position.y, dim - 1); }
				if (nextDirection == down) { return Vec2(position.y, 0); }
			}

			if (currentDirection == right)
			{
				Utilities::VerifyElseCrash(position.x == dim - 1);
				if (nextDirection == left) { return Vec2(dim - 1, dim - 1 - position.y); }
				if (nextDirection == right) { return Vec2(0, position.y); }
				if (nextDirection == up) { return Vec2(position.y, dim - 1); }
				if (nextDirection == down) { return Vec2(dim - 1 - position.y, 0); }
			}

			if (currentDirection == up)
			{
				Utilities::VerifyElseCrash(position.y == 0);
				if (nextDirection == left) { return Vec2(dim - 1, dim - 1 - position.x); }
				if (nextDirection == right) { return Vec2(0, position.x); }
				if (nextDirection == up) { return Vec2(position.x, dim - 1); }
				if (nextDirection == down) { return Vec2(dim - 1 - position.x, 0); }
			}

			if (currentDirection == down)
			{
				Utilities::VerifyElseCrash(position.y == dim - 1);
				if (nextDirection == left) { return Vec2(dim - 1, position.x); }
				if (nextDirection == right) { return Vec2(0, dim - 1 - position.x); }
				if (nextDirection == up) { return Vec2(dim - 1 - position.x, dim - 1); }
				if (nextDirection == down) { return Vec2(position.x, 0); }
			}

			Utilities::VerifyNotReached();
			return position;
		}

		int GetPassword()
		{
			auto columnScore = 1000 * (m_position.y + yShifts[static_cast<int>(m_faceLabel)] + 1);
			auto rowScore = 4 * (m_position.x + xShifts[static_cast<int>(m_faceLabel)] + 1);
			auto facingScore = 0;
			if (m_direction == right)     { facingScore = 0; }
			else if (m_direction == left) { facingScore = 2; }
			else if (m_direction == up)   { facingScore = 3; }
			else if (m_direction == down) { facingScore = 1; }
			else { Utilities::VerifyNotReached(); }
			return columnScore + rowScore + facingScore;
		}

		std::vector<Utilities::Grid2d<char>> m_faces;
		Vec2 m_direction = right;
		Vec2 m_position = Vec2{ 0, 0 };
		CubeFace m_faceLabel = CubeFace::A;
	};

	void PrintSolution()
	{
		auto input = Utilities::ReadAllLinesInFile("Puzzle22.input");
		auto maze = Maze{ input | std::ranges::views::take(input.size() - 2) };

		const auto& directions = input[input.size() - 1];
		for (auto i = 0; i < directions.size(); ++i)
		{
			if (directions[i] == 'L')
			{
				maze.TurnLeft();
			}
			else if (directions[i] == 'R')
			{
				maze.TurnRight();
			}
			else
			{
				int j = i;
				while (j++ < directions.size() && directions[j] >= '0' && directions[j] <= '9')
				{
				}

				auto distance = std::stoi(directions.substr(i, j - i));
				maze.Move(distance);
				i = j - 1;
			}
		}

		std::cout << maze.GetPassword();
	}
}