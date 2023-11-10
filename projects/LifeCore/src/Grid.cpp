#include "Grid.h"

#include <cassert>
#include <stdlib.h>
#include <cstring>

namespace LifeCore
{

	Grid::Grid(size_t width, size_t height) : m_width(width), m_height(height)
	{
		const size_t cellCount = MaxLinearIndex();
		m_buffer = static_cast<Cell*>(calloc(cellCount, sizeof(Cell)));
	}

	Grid::Grid(const Grid& other)
	{
		m_width = other.m_width;
		m_height = other.m_height;

		const size_t bufferSize = MaxLinearIndex() * sizeof(Cell);
		m_buffer = static_cast<Cell*>(malloc(bufferSize));
		assert(m_buffer != nullptr);

		{
			// Make sure our other grid isn't changing while we're copying it over
			const std::shared_lock<std::shared_mutex> lock(const_cast<Grid&>(other).m_mutex);
			memcpy(m_buffer, other.m_buffer, bufferSize);
		}
	}

	Grid::~Grid()
	{
		free(m_buffer);
	}

	void Grid::SetCell(const Position& position, bool value)
	{
		{
			std::unique_lock<std::shared_mutex> lock(m_mutex);
			assert(m_buffer != nullptr);
			m_buffer[ToLinearIndex(position)].Set(value);
		}
	}

	const Cell& Grid::ReadCell(const Position& position)
	{
		{
			std::shared_lock<std::shared_mutex> lock(m_mutex);
			assert(m_buffer != nullptr);
			return m_buffer[ToLinearIndex(position)];
		}
	}

	void Grid::Clear()
	{
		{
			std::unique_lock<std::shared_mutex> lock(m_mutex);
			m_buffer = static_cast<Cell*>(memset(m_buffer, 0, MaxLinearIndex() * sizeof(Cell)));
			assert(m_buffer != nullptr);
		}
	}

	size_t Grid::ToLinearIndex(const Position& position) const
	{
		size_t index = position.m_y * m_width + position.m_x;
		assert(index >= 0 && index < MaxLinearIndex());
		return index;
	}

	size_t Grid::MaxLinearIndex() const
	{
		return m_width * m_height;
	}
}