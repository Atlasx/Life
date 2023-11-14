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

		m_dirtyRect = BoundedRect(0, 0, width-1, height-1, 1);
	}

	Grid::Grid(const Grid& other)
	{
		m_width = other.m_width;
		m_height = other.m_height;

		const size_t cellCount = MaxLinearIndex();
		m_buffer = std::make_unique<Cell[]>(cellCount);
		assert(m_buffer != nullptr);

		{
			// Make sure our other grid isn't changing while we're copying it over
			const std::shared_lock<std::shared_mutex> lock(const_cast<Grid&>(other).m_mutex);
			memcpy(m_buffer.get(), other.m_buffer.get(), cellCount * sizeof(Cell));
		}

	}

	Grid::~Grid()
	{
		// Buffer is freed by out of scope unique ptr
	}

	void Grid::SetCell(int x, int y)
	{
		SetCell(x, y, true, true);
	}

	void Grid::SetCell(const Position& position, bool value, bool markDirty)
	{
		SetCell(position.m_x, position.m_y, value, markDirty);
	}

	void Grid::SetCell(int x, int y, bool value, bool markDirty)
	{
		if (!IsBounded(x, y)) {
			return;
		}
		{
			std::unique_lock<std::shared_mutex> lock(m_mutex);
			assert(m_buffer != nullptr);
			Cell& cell = m_buffer[ToLinearIndex(x, y)];

			cell.Set(value);
			if (markDirty) m_dirtyRect.Encompass(x, y);
		}
	}

	Cell* Grid::GetAt(const Position& position)
	{
		return GetAt(position.m_x, position.m_y);
	}

	Cell* Grid::GetAt(int x, int y)
	{
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
		int index = y * m_width + x;
		assert(index >= 0 && index < MaxLinearIndex());
		return index;
	}

	size_t Grid::ToLinearIndex(const Position& position) const
	{
		return ToLinearIndex(position.m_x, position.m_y);
	}

	size_t Grid::MaxLinearIndex() const
	{
		return m_width * m_height;
	}

	bool Grid::IsBounded(const Position& position) const
	{
		return IsBounded(position.m_x, position.m_y);
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

		// removed to favor lightweight C array for hot-path
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
		for (int i = 0; i < 16; i += 2) {
			if (IsCellAlive(deltas[i] + position.m_x, deltas[i + 1] + position.m_y)) {
				count++;
			}
		}

		return count;
	}

	std::string GridToString(Grid& grid)
	{
		std::string output;
		int prevX = grid.GetWidth();

		auto printer = [&](int i, int j) {
			if (i <= prevX) {
				output.append("\n");
				prevX = i;
			}

			Cell* curCell = grid.GetAt(i, j);
			if (curCell != nullptr) {
				if (curCell->IsAlive()) {
					output.append("X");
				}
				else {
					output.append("O");
				}
			}
			if (grid.IsDirty(i, j)) {
				output.append(".");
			}
			else {
				output.append(" ");
			}
		};

		grid.IterateGrid(printer);
		output.append("\n");

		return output;
	}

	void Grid::AddDirtyBounds(int x, int y)
	{
		assert(IsBounded(x, y));
		m_dirtyRect.Encompass(x, y);
	}

	void Grid::ClearDirtyBounds()
	{
		m_dirtyRect.Reset();
	}

	bool Grid::IsDirty(int x, int y)
	{
		return m_dirtyRect.Contains(x, y);
	}

	void Grid::IterateDirtyRect(std::function<void(int, int)> lambda)
	{
		// Small error prone problem here:
		// dirtyRect is inclusive max while m_width and m_height are exclusive max
		for (int j = m_dirtyRect.m_yMin; j <= m_dirtyRect.m_yMax; j++) {
			for (int i = m_dirtyRect.m_xMin; i <= m_dirtyRect.m_xMax; i++) {
				lambda(i, j);
			}
		}
	}

	void Grid::IterateGrid(std::function<void(int, int)> lambda)
	{
		for (int j = 0; j < m_height; j++) {
			for (int i = 0; i < m_width; i++) {
				lambda(i, j);
			}
		}
	}
}