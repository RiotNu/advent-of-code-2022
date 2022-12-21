#include "pch.h"
#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

namespace Puzzle21B
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
		auto root = Operation{};
		for (const auto& line : input)
		{
			auto tokens = Utilities::SplitString(line, ": ");
			if (tokens[0] == "humn")
			{
				continue;
			}

			if (!tokens[1].contains(' '))
			{
				monkeyValues[tokens[0]] = std::stoi(tokens[1]);
			}
			else
			{
				auto [_, left, op, right] = scn::scan_tuple<std::string, char, std::string>(tokens[1], "{} {} {}");
				if (tokens[0] == "root")
				{
					root = Operation{ left, right, '=' };
					continue;
				}

				monkeyOps[tokens[0]] = Operation{ left, right, op };
				monkeyDepends.insert({ left, tokens[0] });
				monkeyDepends.insert({ right, tokens[0] });
			}
		}

		// Solve until we are blocked on the value of "humn"
		auto lastMonkeyOpsSize = monkeyOps.size();
		auto currentMonkeyOpsSize = lastMonkeyOpsSize + 1;
		while (currentMonkeyOpsSize != lastMonkeyOpsSize)
		{
			lastMonkeyOpsSize = monkeyOps.size();
			SolveMonkeyOperations(monkeyValues, monkeyDepends, monkeyOps);
			currentMonkeyOpsSize = monkeyOps.size();
		}

		// Simplifying assumption that one term in "root" is known. This is true in my input.
		Utilities::VerifyElseCrash(monkeyValues.contains(root.left) || monkeyValues.contains(root.right));

		// Get the value we need the result to hit from the known term on "root".
		auto targetValue = monkeyValues.contains(root.left) ? monkeyValues[root.left] : monkeyValues[root.right];

		// Traverse down the tree of operations reversing each operation until we hit "humn".
		auto currentOp = monkeyValues.contains(root.left) ? monkeyOps[root.right] : monkeyOps[root.left];
		auto result = 0ll;
		while (true)
		{
			if (monkeyValues.contains(currentOp.left))
			{
				auto leftValue = monkeyValues[currentOp.left];
				switch (currentOp.op)
				{
				case '+':
					targetValue -= leftValue;
					break;
				case '-':
					targetValue -= leftValue;
					targetValue *= -1;
					break;
				case '*':
					targetValue /= leftValue;
					break;
				case '/':
					targetValue = leftValue / targetValue;
					break;
				}

				if (currentOp.right == "humn")
				{
					result = targetValue;
					break;
				}

				currentOp = monkeyOps[currentOp.right];
			}
			else if (monkeyValues.contains(currentOp.right))
			{
				auto rightValue = monkeyValues[currentOp.right];
				switch (currentOp.op)
				{
				case '+':
					targetValue -= rightValue;
					break;
				case '-':
					targetValue += rightValue;
					break;
				case '*':
					targetValue /= rightValue;
					break;
				case '/':
					targetValue *= rightValue;
					break;
				}

				if (currentOp.left == "humn")
				{
					result = targetValue;
					break;
				}

				currentOp = monkeyOps[currentOp.left];
			}
			else
			{
				Utilities::VerifyNotReached();
			}
		}

		std::cout << result;
	}
}