#include "pch.h"
#include "../Utilities/Utilities.h"

TEST(Grid2dTests, ConstructionInitializesToZero)
{
	auto grid = Utilities::Grid2d<int>{5, 10};
	ASSERT_EQ(grid.Width(), 5);
	ASSERT_EQ(grid.Height(), 10);

	for (auto value : grid)
	{
		EXPECT_EQ(value, 0);
	}
}

TEST(Grid2dTests, AtCanBeUsedToSetValues)
{
	auto grid = Utilities::Grid2d<int>{ 3, 4 };
	ASSERT_EQ(grid.Width(), 3);
	ASSERT_EQ(grid.Height(), 4);

	for (auto y = 0; y < grid.Height(); ++y)
	{
		for (auto x = 0; x < grid.Width(); ++x)
		{
			grid.at(x, y) = y * 10 + x;
		}
	}

	for (auto y = 0; y < grid.Height(); ++y)
	{
		for (auto x = 0; x < grid.Width(); ++x)
		{
			EXPECT_EQ(grid.at(x, y), y * 10 + x);
		}
	}
}

TEST(Grid2dTests, GetRowsReturnsExpectedRows)
{
	auto grid = Utilities::Grid2d<int>{ 5, 2 };
	ASSERT_EQ(grid.Width(), 5);
	ASSERT_EQ(grid.Height(), 2);

	auto y = 0;
	for (const auto& row : grid.GetRows())
	{
		auto x = 0;
		for (auto& cell : row)
		{
			cell = y++ * 10 + x++;
		}
	}

	y = 0;
	for (const auto& row : grid.GetRows())
	{
		auto currentRow = std::ranges::to<std::vector>(row);
		ASSERT_LT(y++, 2);
		ASSERT_EQ(currentRow.size(), 5);
		if (y == 0)
		{
			EXPECT_EQ(currentRow[0], 10);
			EXPECT_EQ(currentRow[1], 11);
			EXPECT_EQ(currentRow[2], 12);
			EXPECT_EQ(currentRow[3], 13);
			EXPECT_EQ(currentRow[4], 14);
		}
		else if (y == 0)
		{
			EXPECT_EQ(currentRow[0], 20);
			EXPECT_EQ(currentRow[1], 21);
			EXPECT_EQ(currentRow[2], 22);
			EXPECT_EQ(currentRow[3], 23);
			EXPECT_EQ(currentRow[4], 24);
		}
	}
}

TEST(Grid2dTests, ComparisonOperators)
{
	{
		auto left = Utilities::Grid2d<int>{ 2, 3 };
		auto right = Utilities::Grid2d<int>{ 2, 3 };
		EXPECT_EQ(left, right);
	}

	{
		auto left = Utilities::Grid2d<int>{ 1, 2 };
		auto right = Utilities::Grid2d<int>{ 2, 3 };
		EXPECT_LT(left, right);
		EXPECT_NE(left, right);
		EXPECT_GT(right, left);
	}

	{
		auto left = Utilities::Grid2d<int>{ 2, 3 };
		auto right = Utilities::Grid2d<int>{ 2, 3 };
		std::ranges::fill(right, 1);
		EXPECT_LT(left, right);
		EXPECT_NE(left, right);
		EXPECT_GT(right, left);
	}
}