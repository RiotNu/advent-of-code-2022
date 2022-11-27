#include "pch.h"
#include "Utilities.h"

namespace Utilities
{

void VerifyElseCrash(bool bAssert)
{
	assert(bAssert);
	if (!bAssert)
	{
		std::terminate();
	}
}

void VerifyNotReached()
{
	VerifyElseCrash(false);
}

std::vector<std::string> SplitString(const std::string& input, const std::string& delimiter)
{
	return Utilities::SplitStringAndTransform<std::string>(
		input,
		delimiter,
		[](const std::string& token) { return token; });
}

std::vector<int> ReadCommaSeparatedInt32s(const std::string& input)
{
	return Utilities::SplitStringAndTransform<int>(
		input,
		",",
		[](const std::string& token) { return std::stoi(token); });
}

std::vector<int64_t> ReadCommaSeparatedInt64s(const std::string& input)
{
	return Utilities::SplitStringAndTransform<int64_t>(
		input,
		",",
		[](const std::string& token) { return std::stoll(token); });
}

}
