#include "pch.h"
#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

namespace Puzzle25A
{
	int64_t SnafuToDecimal(const std::string& snafuNumber)
	{
		auto result = 0ll;
		auto currentPower = 1ll;
		for (auto c : snafuNumber | std::ranges::views::reverse)
		{
			switch (c)
			{
			case '0':
				break;
			case '1':
				result += currentPower;
				break;
			case '2':
				result += 2 * currentPower;
				break;
			case '-':
				result += -1 * currentPower;
				break;
			case '=':
				result += -2 * currentPower;
				break;
			}

			currentPower *= 5;
		}

		return result;
	}

	const auto digitsToMin = std::vector<int64_t>
	{
		SnafuToDecimal("="),
		SnafuToDecimal("=="),
		SnafuToDecimal("==="),
		SnafuToDecimal("===="),
		SnafuToDecimal("====="),
		SnafuToDecimal("======"),
		SnafuToDecimal("======="),
		SnafuToDecimal("========"),
		SnafuToDecimal("========="),
		SnafuToDecimal("=========="),
		SnafuToDecimal("==========="),
		SnafuToDecimal("============"),
		SnafuToDecimal("============="),
		SnafuToDecimal("=============="),
		SnafuToDecimal("==============="),
		SnafuToDecimal("================"),
		SnafuToDecimal("================="),
		SnafuToDecimal("=================="),
		SnafuToDecimal("==================="),
		SnafuToDecimal("===================="),
		SnafuToDecimal("====================="),
		SnafuToDecimal("======================"),
		SnafuToDecimal("======================="),
		SnafuToDecimal("========================"),
		SnafuToDecimal("========================="),
		SnafuToDecimal("=========================="),
		SnafuToDecimal("==========================="),
	};

	const auto digitsToMax = std::vector<int64_t>
	{
		SnafuToDecimal("2"),
		SnafuToDecimal("22"),
		SnafuToDecimal("222"),
		SnafuToDecimal("2222"),
		SnafuToDecimal("22222"),
		SnafuToDecimal("222222"),
		SnafuToDecimal("2222222"),
		SnafuToDecimal("22222222"),
		SnafuToDecimal("222222222"),
		SnafuToDecimal("2222222222"),
		SnafuToDecimal("22222222222"),
		SnafuToDecimal("222222222222"),
		SnafuToDecimal("2222222222222"),
		SnafuToDecimal("22222222222222"),
		SnafuToDecimal("222222222222222"),
		SnafuToDecimal("2222222222222222"),
		SnafuToDecimal("22222222222222222"),
		SnafuToDecimal("222222222222222222"),
		SnafuToDecimal("2222222222222222222"),
		SnafuToDecimal("22222222222222222222"),
		SnafuToDecimal("222222222222222222222"),
		SnafuToDecimal("2222222222222222222222"),
		SnafuToDecimal("22222222222222222222222"),
		SnafuToDecimal("222222222222222222222222"),
		SnafuToDecimal("2222222222222222222222222"),
		SnafuToDecimal("22222222222222222222222222"),
		SnafuToDecimal("222222222222222222222222222"),
	};

	std::string DecimalToSnafu(int64_t number)
	{
		auto digits = 0;
		while (digits < digitsToMax.size() && digitsToMax[digits] < number)
		{
			++digits;
		}
		Utilities::VerifyElseCrash(digits < digitsToMax.size());

		auto snafu = std::stringstream{};
		auto remaining = number;
		while (digits > 0)
		{
			auto currentPower = static_cast<int64_t>(std::pow(5, digits));
			if (currentPower == 1)
			{
				break;
			}

			auto nextDigitsMin = digitsToMin[digits - 1];
			if (remaining > 0)
			{
				if (remaining >= 2 * currentPower + nextDigitsMin)
				{
					remaining -= 2 * currentPower;
					snafu << '2';
				}
				else if (remaining >= currentPower + nextDigitsMin)
				{
					remaining -= currentPower;
					snafu << '1';
				}
				else
				{
					snafu << '0';
				}
			}
			else
			{
				if (remaining * -1 >= 2 * currentPower + nextDigitsMin)
				{
					remaining += 2 * currentPower;
					snafu << '=';
				}
				else if (remaining * -1 >= currentPower + nextDigitsMin)
				{
					remaining += currentPower;
					snafu << '-';
				}
				else
				{
					snafu << '0';
				}
			}

			--digits;
		}

		switch (remaining)
		{
		case 2:
			snafu << '2';
			break;
		case 1:
			snafu << '1';
			break;
		case 0:
			snafu << '0';
			break;
		case -1:
			snafu << '-';
			break;
		case -2:
			snafu << '=';
			break;
		}

		return snafu.str();
	}

	void PrintSolution()
	{
		auto sum = 0ll;
		for (const auto& line : Utilities::ReadAllLinesInFile("Puzzle25.input"))
		{
			sum += SnafuToDecimal(line);
		}

		std::cout << DecimalToSnafu(sum);
	}
}