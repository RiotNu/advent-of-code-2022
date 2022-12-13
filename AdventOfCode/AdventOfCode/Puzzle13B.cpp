#include "pch.h"
#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

namespace Puzzle13B
{
	enum class EntryType
	{
		Int,
		List,
	};

	struct Entry
	{
		EntryType entryType = EntryType::Int;
		int value = 0;
		std::vector<Entry> values;

		bool operator==(const Entry& other) const = default;
		auto operator<=>(const Entry& other) const
		{
			auto result = CompareEntries(*this, other);
			if (result < 0)
			{
				return std::strong_ordering::less;
			}

			if (result > 0)
			{
				return std::strong_ordering::greater;
			}

			return std::strong_ordering::equal;
		}

	private:
		static int CompareListEntries(const Entry& left, const Entry& right)
		{
			Utilities::VerifyElseCrash(left.entryType == EntryType::List && right.entryType == EntryType::List);

			auto minListSize = std::min(left.values.size(), right.values.size());
			for (auto i = 0; i < minListSize; ++i)
			{
				auto result = CompareEntries(left.values[i], right.values[i]);
				if (result != 0)
				{
					return result;
				}
			}

			return static_cast<int>(left.values.size()) - static_cast<int>(right.values.size());
		}

		static int CompareEntries(const Entry& left, const Entry& right)
		{
			if (left.entryType == EntryType::Int && right.entryType == EntryType::Int)
			{
				return left.value - right.value;
			}

			if (left.entryType == EntryType::Int)
			{
				auto leftReplacement = Entry{ EntryType::List };
				leftReplacement.values.emplace_back(EntryType::Int, left.value);
				return CompareListEntries(leftReplacement, right);
			}

			if (right.entryType == EntryType::Int)
			{
				auto rightReplacement = Entry{ EntryType::List };
				rightReplacement.values.emplace_back(EntryType::Int, right.value);
				return CompareListEntries(left, rightReplacement);
			}

			return CompareListEntries(left, right);
		}
	};

	Entry ParseEntry(const std::string& str, size_t& offset)
	{
		auto result = Entry{};
		if (str[offset] != '[')
		{
			result.entryType = EntryType::Int;
			auto nextComma = str.find(',', offset + 1);
			auto nextBracket = str.find(']', offset + 1);
			auto end = std::min(nextComma, nextBracket);
			result.value = std::stoi(str.substr(offset, end - offset));
			offset = end;
			return result;
		}

		result.entryType = EntryType::List;
		if (str[offset + 1] == ']')
		{
			// Empty list
			offset += 2;
			return result;
		}

		while (str[offset] != ']')
		{
			result.values.emplace_back(ParseEntry(str, ++offset));
		}

		++offset;
		return result;
	}

	Entry ParseEntry(const std::string& str)
	{
		auto offset = 0ull;
		return ParseEntry(str, offset);
	}

	void PrintSolution()
	{
		auto input = Utilities::ReadAllLinesInFile("Puzzle13.input");

		auto startDivider = ParseEntry("[[2]]");
		auto endDivider = ParseEntry("[[6]]");
		auto packets = std::vector<Entry>{ startDivider, endDivider };
		for (const auto& line : input)
		{
			if (line.size() == 0)
			{
				continue;
			}

			packets.emplace_back(ParseEntry(line));
		}

		std::ranges::sort(packets, std::less<Entry>{});
		auto start = std::ranges::find(packets, startDivider);
		auto end = std::ranges::find(packets, endDivider);
		auto result = (std::distance(packets.begin(), start) + 1) * (std::distance(packets.begin(), end) + 1);

		std::cout << result;
	}
}