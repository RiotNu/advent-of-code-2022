#include "pch.h"
#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

namespace Puzzle11A
{
	class Monkey
	{
	public:
		Monkey(const std::vector<int>& items, std::function<int(int)> operation, std::function<int(int)> getThrowTarget)
			: m_items(items), m_operation(operation), m_getThrowTarget(getThrowTarget)
		{
		}

		int GetTotalInspects() const { return m_totalInspects; }

		void Tick(std::vector<Monkey>& monkeys)
		{
			for (auto item : m_items)
			{
				++m_totalInspects;
				auto worry = m_operation(item);
				worry /= 3;
				auto target = m_getThrowTarget(worry);
				monkeys[target].m_items.emplace_back(worry);
			}

			m_items.clear();
		}

		friend std::ostream& operator<<(std::ostream& stream, const Monkey& monkey)
		{
			auto bFirst = true;
			for (auto i : monkey.m_items)
			{
				if (bFirst)
				{
					bFirst = false;
				}
				else
				{
					stream << ", ";
				}

				stream << i;
			}

			return stream;
		}

	private:
		std::vector<int> m_items;
		std::function<int(int)> m_operation;
		std::function<int(int)> m_getThrowTarget;
		int m_totalInspects = 0;
	};

	void PrintSolution()
	{
		auto input = Utilities::ReadAllLinesInFile("Puzzle11.input");

		auto monkeys = std::vector<Monkey>{};
		for (auto monkeyInput : input | ranges::views::chunk(7))
		{
			auto items = Utilities::SplitStringAndTransform<int>(monkeyInput[1].substr(17), ", ", [](const std::string& item) { return std::stoi(item); });
			auto [unusedA, op, arg] = scn::scan_tuple<char, std::string>(monkeyInput[2], "  Operation: new = old {} {}");
			auto [unusedB, div] = scn::scan_tuple<int>(monkeyInput[3], "  Test: divisible by {}");
			auto [unusedC, trueMonkey] = scn::scan_tuple<int>(monkeyInput[4], "    If true: throw to monkey {}");
			auto [unusedD, falseMonkey] = scn::scan_tuple<int>(monkeyInput[5], "    If false: throw to monkey {}");

			std::function<int(int)> operation;
			if (arg == "old")
			{
				operation = op == '*'
					? std::function<int(int)>{ [](int item) { return item * item; } }
					: std::function<int(int)>{ [](int item) { return item + item; } };
			}
			else
			{
				auto argInt = std::stoi(arg);
				operation = op == '*'
					? std::function<int(int)>{ [argInt](int item) { return item * argInt; } }
					: std::function<int(int)>{ [argInt](int item) { return item + argInt; } };
			}

			monkeys.emplace_back(items, operation, [div, trueMonkey, falseMonkey](int item) { return item % div == 0 ? trueMonkey : falseMonkey; });
		}

		for (auto i = 0; i < 20; ++i)
		{
			for (auto& monkey : monkeys)
			{
				monkey.Tick(monkeys);
			}
		}

		std::ranges::sort(monkeys, std::greater<int>{}, [](const Monkey& monkey) { return monkey.GetTotalInspects(); });
		std::cout << monkeys[0].GetTotalInspects() * monkeys[1].GetTotalInspects();
	}
}