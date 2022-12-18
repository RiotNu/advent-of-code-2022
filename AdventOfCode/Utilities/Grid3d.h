#pragma once

namespace Utilities
{
	// Wraps a std::vector and for easier three-dimensional addressing.
	// Grid dimensions cannot be changed after construction.
	template <typename T>
	class Grid3d
	{
	public:
		// Initializes grid to specified width and height. Default initializes.
		Grid3d(size_t width, size_t height, size_t depth)
			: m_width(width), m_height(height), m_depth(depth), m_data(m_width * m_height * m_depth)
		{
		}

		// Width of the grid.
		size_t Width() const { return m_width; }

		// Height of the grid.
		size_t Height() const { return m_height; }

		// Depth of the grid.
		size_t Depth() const { return m_depth; }

		// Returns the size of the underlying vector.
		size_t size() const { return m_data.size(); }

		// Converts an index for the underlying vector into (x, y, z) coordinates.
		auto GetCoordinatesFromIndex(size_t i) const
		{
			Utilities::VerifyElseCrash(i < m_data.size());
			auto z = i / (m_width * m_height);
			i -= z * m_width * m_height;
			auto y = i / m_width;
			auto x = i % m_width;
			return std::make_tuple(x, y, z);
		}

		// Converts (x, y, z) coordinates into an index for the underlying vector.
		size_t GetIndexFromCoordinates(size_t x, size_t y, size_t z) const
		{
			Utilities::VerifyElseCrash(x < m_width);
			Utilities::VerifyElseCrash(y < m_height);
			Utilities::VerifyElseCrash(z < m_depth);
			return (z * m_height + y) * m_width + x;
		}

		// Retrieves reference to the value stored at the provided coordinates.
		T& at(size_t x, size_t y, size_t z)
		{
			Utilities::VerifyElseCrash(x < m_width);
			Utilities::VerifyElseCrash(y < m_height);
			Utilities::VerifyElseCrash(z < m_depth);
			return m_data.at((z * m_height + y) * m_width + x);
		}

		// Retrieves constant reference to the value stored at the provided coordinates.
		const T& at(size_t x, size_t y, size_t z) const
		{
			Utilities::VerifyElseCrash(x < m_width);
			Utilities::VerifyElseCrash(y < m_height);
			Utilities::VerifyElseCrash(z < m_depth);
			return m_data.at((z * m_height + y) * m_width + x);
		}

		// Retrieves reference to the value stored at the provided index.
		T& at(size_t i)
		{
			return m_data.at(i);
		}

		// Retrieves constant reference to the value stored at the provided index.
		const T& at(size_t i) const
		{
			return m_data.at(i);
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
		auto operator<=>(const Grid3d<T>&) const = default;

	private:
		size_t m_width = 0;
		size_t m_height = 0;
		size_t m_depth = 0;
		std::vector<T> m_data;
	};
}
