#include "pch.h"
#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

namespace Puzzle21A
{
	struct Operation
	{
		std::string left;
		std::string right;
		char op = '+';
	};

	// Solves monkey operations that are solvable with current values. Updates values with results.
	void SolveMonkeyOperations(
		std::unordered_map<std::string, int64_t>& monkeyValues,
		std::unordered_multimap<std::string, std::string>& monkeyDepends,
		std::unordered_map<std::string, Operation>& monkeyOps)
	{
		auto currentValues = monkeyValues;
		for (const auto& monkey : currentValues | std::ranges::views::keys)
		{
			if (!monkeyDepends.contains(monkey))
			{
				continue;
			}

			auto found = monkeyDepends.equal_range(monkey);
			for (auto it = found.first; it != found.second; ++it)
			{
				auto dependent = it->second;
				if (!monkeyOps.contains(dependent))
				{
					continue;
				}

				auto op = monkeyOps[dependent];
				if (currentValues.contains(op.left) && currentValues.contains(op.right))
				{
					auto left = currentValues[op.left];
					auto right = currentValues[op.right];
					auto result = 0ll;
					switch (op.op)
					{
					case '+':
						result = left + right;
						break;
					case '-':
						result = left - right;
						break;
					case '*':
						result = left * right;
						break;
					case '/':
						result = left / right;
						break;
					}

					monkeyValues[dependent] = result;
					monkeyOps.erase(dependent);
				}
			}
			monkeyDepends.erase(monkey);
		}
	}

	void PrintSolution()
	{
		auto input = Utilities::ReadAllLinesInFile("Puzzle21.input");
		auto monkeyValues = std::unordered_map<std::string, int64_t>{};
		auto monkeyOps = std::unordered_map<std::string, Operation>{};
		auto monkeyDepends = std::unordered_multimap<std::string, std::string>{};
		for (const auto& line : input)
		{
			auto tokens = Utilities::SplitString(line, ": ");
			if (!tokens[1].contains(' '))
			{
				monkeyValues[tokens[0]] = std::stoi(tokens[1]);
			}
			else
			{
				auto [_, left, op, right] = scn::scan_tuple<std::string, char, std::string>(tokens[1], "{} {} {}");
				monkeyOps[tokens[0]] = Operation{ left, right, op };
				monkeyDepends.insert({left, tokens[0]});
				monkeyDepends.insert({right, tokens[0]});
			}
		}

		while (monkeyOps.size() > 0)
		{
			SolveMonkeyOperations(monkeyValues, monkeyDepends, monkeyOps);
		}

		std::cout << monkeyValues["root"];
	}
}