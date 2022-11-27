#pragma once

namespace Utilities
{
	// Crashes the program if the assertion is false.
	void VerifyElseCrash(bool bAssert);

	// Crashes the program if executed. Use to annotate code locations expected to be unreachable.
	void VerifyNotReached();

	// Parses the input string into tokens separated by the provided delimiter and applies the supplied
	// transform to each token.
	template<typename T>
	std::vector<T> SplitStringAndTransform(
		const std::string& input,
		const std::string& delimiter,
		std::function<T(const std::string&)> transform)
	{
		constexpr auto subrangeToString =
			[](const auto& subrange)
			{
				return std::string{ std::string_view{ subrange.begin(), subrange.end() }};
			};

		auto tokens = std::vector<T>{};
		for (const auto& token : std::views::split(input, delimiter)
			| std::views::transform(subrangeToString)
			| std::views::transform(transform))
		{
			tokens.emplace_back(token);
		}

		return tokens;
	}

	// Parses the input string into tokens separated by the provided delimiter.
	std::vector<std::string> SplitString(const std::string& input, const std::string& delimiter);

	// Parses comma-separated 32-bit integers.
	std::vector<int> ReadCommaSeparatedInt32s(const std::string& input);

	// Parses comma-separated 64-bit integers.
	std::vector<int64_t> ReadCommaSeparatedInt64s(const std::string& input);
}