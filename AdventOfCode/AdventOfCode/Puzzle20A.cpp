#include "pch.h"
#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

namespace Puzzle20A
{
	struct Entry
	{
		int uniqueIndex = 0;
		int number = 0;

		auto operator<=>(const Entry&) const = default;

		friend std::ostream& operator<<(std::ostream& stream, const Entry& entry)
		{
			stream << entry.number;
			return stream;
		}
	};

	void PrintSolution()
	{
		auto original = std::vector<Entry>{};
		auto uniqueIndex = 0;
		auto zeroEntry = Entry{};
		for (const auto& line : Utilities::ReadAllLinesInFile("Puzzle20.input"))
		{
			auto entry = Entry{ uniqueIndex++, std::stoi(line) };
			if (entry.number == 0)
			{
				zeroEntry = entry;
			}
			original.emplace_back(entry);
		}

		auto current = original;
		for (auto i = 0; i < original.size(); ++i)
		{
			// Find the current index.
			auto currentIndex = std::distance(current.begin(), std::ranges::find(current, original[i]));
			auto number = original[i].number;

			// Swap forward or backwards until number is in the right position.
			if (number > 0)
			{
				auto swaps = number % (current.size() - 1);
				for (auto j = swaps; j > 0; --j)
				{
					auto nextIndex = currentIndex + 1;
					if (nextIndex == current.size())
					{
						nextIndex = 0;
					}
					std::swap(current[currentIndex], current[nextIndex]);
					currentIndex = nextIndex;
				}
			}
			else
			{
				auto swaps = number % (static_cast<int>(current.size()) - 1);
				if (swaps < 0)
				{
					swaps *= -1;
				}
				for (auto j = swaps; j > 0; --j)
				{
					auto nextIndex = currentIndex - 1;
					if (nextIndex == -1)
					{
						nextIndex = current.size() - 1;
					}
					std::swap(current[currentIndex], current[nextIndex]);
					currentIndex = nextIndex;
				}
			}
		}

		auto zeroIndex = std::distance(current.begin(), std::ranges::find(current, zeroEntry));
		auto a = current[(zeroIndex + 1000) % current.size()];
		auto b = current[(zeroIndex + 2000) % current.size()];
		auto c = current[(zeroIndex + 3000) % current.size()];
		std::cout << a.number + b.number + c.number;
	}
}