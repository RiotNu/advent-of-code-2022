#include "pch.h"
#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

namespace Puzzle11B
{
	class Monkey
	{
	public:
		Monkey(const std::vector<int64_t>& items, std::function<int64_t(int64_t)> operation, int div, int trueMonkey, int falseMonkey)
			: m_items(items), m_operation(operation), m_div(div), m_trueMonkey(trueMonkey), m_falseMonkey(falseMonkey)
		{
		}

		int64_t GetTotalInspects() const { return m_totalInspects; }

		void SetMaxWorry(int64_t maxWorry) { m_maxWorry = maxWorry; }

		void Tick(std::vector<Monkey>& monkeys)
		{
			for (auto item : m_items)
			{
				++m_totalInspects;
				auto worry = m_operation(item);
				worry = worry % m_maxWorry;
				auto target = worry % m_div == 0 ? m_trueMonkey : m_falseMonkey;
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
		std::vector<int64_t> m_items;
		std::function<int64_t(int64_t)> m_operation;
		int64_t m_totalInspects = 0;
		int64_t m_maxWorry = 0;
		int m_div;
		int m_trueMonkey;
		int m_falseMonkey;
	};

	void PrintSolution()
	{
		auto input = Utilities::ReadAllLinesInFile("Puzzle11.input");

		auto divisors = std::vector<int>{};
		auto monkeys = std::vector<Monkey>{};
		for (auto monkeyInput : input | ranges::views::chunk(7))
		{
			auto items = Utilities::SplitStringAndTransform<int64_t>(monkeyInput[1].substr(17), ", ", [](const std::string& item) { return std::stoi(item); });
			auto [unusedA, op, arg] = scn::scan_tuple<char, std::string>(monkeyInput[2], "  Operation: new = old {} {}");
			auto [unusedB, div] = scn::scan_tuple<int>(monkeyInput[3], "  Test: divisible by {}");
			auto [unusedC, trueMonkey] = scn::scan_tuple<int>(monkeyInput[4], "    If true: throw to monkey {}");
			auto [unusedD, falseMonkey] = scn::scan_tuple<int>(monkeyInput[5], "    If false: throw to monkey {}");

			std::function<int64_t(int64_t)> operation;
			if (arg == "old")
			{
				operation = op == '*'
					? std::function<int64_t(int64_t)>{ [](int64_t item) { return item * item; } }
				: std::function<int64_t(int64_t)>{ [](int64_t item) { return item + item; } };
			}
			else
			{
				auto argInt = std::stoi(arg);
				operation = op == '*'
					? std::function<int64_t(int64_t)>{ [argInt](int64_t item) { return item * argInt; } }
				: std::function<int64_t(int64_t)>{ [argInt](int64_t item) { return item + argInt; } };
			}

			divisors.emplace_back(div);
			monkeys.emplace_back(items, operation, div, trueMonkey, falseMonkey);
		}

		auto maxWorry = ranges::accumulate(divisors, 1ull, [](int64_t total, int64_t val) { return total * val; });
		for (auto& monkey : monkeys)
		{
			monkey.SetMaxWorry(maxWorry);
		}

		for (auto i = 0; i < 10000; ++i)
		{
			for (auto& monkey : monkeys)
			{
				monkey.Tick(monkeys);
			}
		}

		std::ranges::sort(monkeys, std::greater<int64_t>{}, [](const Monkey& monkey) { return monkey.GetTotalInspects(); });
		std::cout << monkeys[0].GetTotalInspects() * monkeys[1].GetTotalInspects();
	}
}