#pragma once

namespace Utilities
{
	// Wraps a std::vector and for easier two-dimensional addressing.
	// Grid dimensions cannot be changed after construction.
	template <typename T>
	class Grid2d
	{
	public:
		// Initializes grid to specified width and height. Default initializes.
		Grid2d(size_t width, size_t height)
			: m_width(width), m_height(height), m_data(m_width * m_height)
		{
		}

		// Returns a mutable row-by-row view of the grid.
		auto GetRows() { return m_data | ranges::views::chunk(m_width); }

		// Returns a constant row-by-row view of the grid.
		auto GetRows() const { return m_data | ranges::views::chunk(m_width); }

		// Width of the grid.
		size_t Width() const { return m_width; }
		
		// Height of the grid.
		size_t Height() const { return m_height; }

		// Returns string representation of the grid.
		std::string ToString() const
		{
			auto stream = std::stringstream{};
			stream << *this;
			return stream.str();
		}

		// Retrieves reference to the value stored at the provided coordinates.
		T& at(size_t x, size_t y)
		{
			Utilities::VerifyElseCrash(x < m_width);
			Utilities::VerifyElseCrash(y < m_height);
			return m_data.at(y * m_width + x);
		}

		// Retrieves constant reference to the value stored at the provided coordinates.
		const T& at(size_t x, size_t y) const
		{
			Utilities::VerifyElseCrash(x < m_width);
			Utilities::VerifyElseCrash(y < m_height);
			return m_data.at(y * m_width + x);
		}

		// Standard iterator overloads.
		std::vector<T>::iterator begin() noexcept { return m_data.begin(); }
		std::vector<T>::iterator end() noexcept { return m_data.end(); }
		std::vector<T>::const_iterator begin() const noexcept { return m_data.begin(); }
		std::vector<T>::const_iterator end() const noexcept { return m_data.end(); }
		std::vector<T>::const_iterator cbegin() const noexcept { return m_data.cbegin(); }
		std::vector<T>::const_iterator cend() const noexcept { return m_data.cend(); }
		std::vector<T>::reverse_iterator rbegin() noexcept { return m_data.rbegin(); }
		std::vector<T>::reverse_iterator rend() noexcept { return m_data.rend(); }
		std::vector<T>::const_reverse_iterator rbegin() const noexcept { return m_data.rbegin(); }
		std::vector<T>::const_reverse_iterator rend() const noexcept { return m_data.rend(); }
		std::vector<T>::const_reverse_iterator crbegin() const noexcept { return m_data.crbegin(); }
		std::vector<T>::const_reverse_iterator crend() const noexcept { return m_data.crend(); }

		// Spaceship operator overload for comparison operators.
		auto operator<=>(const Grid2d<T>&) const = default;
		
		// Insertion operator overload.
		friend std::ostream& operator<<(std::ostream& stream, const Grid2d<T>& grid)
		{
			auto bFirstRow = true;
			for (const auto& row : grid.GetRows())
			{
				if (bFirstRow)
				{
					bFirstRow = false;
				}
				else
				{
					stream << '\n';
				}

				auto bFirstCell = true;
				for (const auto& cell : row)
				{
					if (bFirstCell)
					{
						bFirstCell = false;
						stream << cell;
					}
					else
					{
						stream << ' ' << cell;
					}
				}
			}

			return stream;
		}

	private:
		size_t m_width = 0;
		size_t m_height = 0;
		std::vector<T> m_data;
	};
}
