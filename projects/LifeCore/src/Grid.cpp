#include "Grid.h"

#include <cassert>
#include <stdlib.h>
#include <cstring>
#include <vector>

namespace LifeCore
{

	Grid::Grid(size_t width, size_t height) : m_width(width), m_height(height)
	{
		const size_t cellCount = MaxLinearIndex();
		m_buffer = std::make_unique<Cell[]>(cellCount);
		assert(m_buffer != nullptr);
	}

	Grid::Grid(const Grid& other)
	{
		m_width = other.m_width;
		m_height = other.m_height;

		/*
		* switched to unique ptr and was getting in the weeds with memcpy and malloc.
		* will revisit this later
		* 
		

		const size_t bufferSize = MaxLinearIndex() * sizeof(Cell);
		//m_buffer = std::make_unique<Cell[]>(malloc(bufferSize));
		//assert(m_buffer != nullptr);

		{
			// Make sure our other grid isn't changing while we're copying it over
			//const std::shared_lock<std::shared_mutex> lock(const_cast<Grid&>(other).m_mutex);
			//memcpy(m_buffer.get(), other.m_buffer.get(), bufferSize);
		}
		*/
	}

	Grid::~Grid()
	{
		// Buffer is freed by out of scope unique ptr
	}

	void Grid::SetCell(const Position& position, bool value)
	{
		if (!IsBounded(position)) {
			return;
		}
		{
			std::unique_lock<std::shared_mutex> lock(m_mutex);
			assert(m_buffer != nullptr);
			m_buffer[ToLinearIndex(position)].Set(value);
		}
	}

	void Grid::SetCell(int x, int y, bool value)
	{
		SetCell(Position(x, y), value);
	}

	Cell* Grid::GetAt(const Position& position)
	{
		if (!IsBounded(position)) {
			return nullptr;
		}

		//std::shared_lock<std::shared_mutex> lock(m_mutex);
		//assert(m_buffer != nullptr);
		return &m_buffer[ToLinearIndex(position)];
	}

	Cell* Grid::GetAt(int x, int y)
	{
		//return GetAt(Position(x, y));
		if (!IsBounded(x, y)) {
			return nullptr;
		}

		return &m_buffer[ToLinearIndex(x, y)];
	}

	void Grid::Clear()
	{
		std::unique_lock<std::shared_mutex> lock(m_mutex);
		const size_t cellCount = MaxLinearIndex();
		m_buffer = std::make_unique<Cell[]>(cellCount);
		assert(m_buffer != nullptr);
	}
	
	size_t Grid::ToLinearIndex(int x, int y) const
	{
		return y * m_width + x;
	}

	size_t Grid::ToLinearIndex(const Position& position) const
	{
		const size_t index = position.m_y * m_width + position.m_x;
		assert(index >= 0 && index < MaxLinearIndex());
		return index;
	}

	size_t Grid::MaxLinearIndex() const
	{
		return m_width * m_height;
	}

	bool Grid::IsBounded(const Position& position) const
	{
		return (position.m_x >= 0		&&
				position.m_x < m_width	&&
				position.m_y >= 0		&&
				position.m_y < m_height
			);
	}

	bool Grid::IsBounded(int x, int y) const
	{
		return (x >= 0 &&
			x < m_width &&
			y >= 0 &&
			y < m_height
			);
	}

	bool Grid::IsCellAlive(int x, int y)
	{

		const Cell* cell = GetAt(x, y);
		if (cell == nullptr) { return false; }

		return cell->IsAlive();
	}

	bool Grid::IsCellAlive(const Position& position)
	{
		const Cell* cell = GetAt(position);
		if (cell == nullptr) { return false; }

		return cell->IsAlive();
	}

	size_t Grid::GetNeighborCountOfCell(const Position& position)
	{
		std::shared_lock<std::shared_mutex> lock(m_mutex);
		assert(m_buffer != nullptr);

		// removed to favor lightweight C array for hotpath
		/*static std::vector<Position> neighborDeltas = {
			Position(-1, -1),
			Position(-1, 0),
			Position(-1, 1),
			Position(0, -1),
			Position(0, 1),
			Position(1, -1),
			Position(1, 0),
			Position(1, 1)
		};*/

		static const int deltas[] = {
			-1, -1,
			-1, 0,
			-1, 1,
			0, -1,
			0, 1,
			1, -1,
			1, 0,
			1, 1
		};

		size_t count = 0;

		//for (auto delta : neighborDeltas) {
		for (int i = 0; i < 16; i += 2) {
			/*const Position checkPos = Position(delta.m_x + position.m_x, delta.m_y + position.m_y);
			if (IsCellAlive(delta.m_x + position.m_x, delta.m_y + position.m_y)) {
				count++;
			}*/
			if (IsCellAlive(deltas[i] + position.m_x, deltas[i + 1] + position.m_y)) {
				count++;
			}
		}

		return count;
	}

	std::string GridToString(const Grid& grid)
	{
		const size_t w = grid.GetWidth();
		const size_t h = grid.GetHeight();

		std::string output;

		for (int j = 0; j < h; j++) {
			for (int i = 0; i < w; i++) {
				// stripping const here is bad, todo fix. but don't want to remove const from param
				Cell* curCell = const_cast<Grid&>(grid).GetAt(i, j);
				if (curCell != nullptr) {
					if (curCell->IsAlive()) {
						output.append("X");
					}
					else {
						output.append("O");
					}
				}
			}
			output.append("\n");
		}

		return output;
	}
}