#pragma once

#include <shared_mutex>
#include <thread>
#include <functional>
#include <algorithm>

namespace LifeCore
{
	typedef struct Cell
	{
	private:
		char m_data;

	public:
		inline bool IsAlive() const { return static_cast<bool>(m_data); }
		inline void Set(bool isAlive) { m_data = static_cast<char>(isAlive); }

	} Cell;

	typedef struct Position
	{
		int m_x, m_y;

		Position(int x, int y) : m_x(x), m_y(y) {};

		inline friend Position operator+(Position lhs, const Position& rhs) {
			lhs.m_x += rhs.m_x;
			lhs.m_y += rhs.m_y;
			return lhs;
		}

	} Position;

	typedef struct BoundedRect
	{
		int m_xMin, m_yMin, m_xMax, m_yMax;

		BoundedRect() : m_xMin(0), m_xMax(0), m_yMin(0), m_yMax(0), m_buffer(1) {
			m_xBoundMin = m_xMin;
			m_xBoundMax = m_xMax;
			m_yBoundMin = m_yMin;
			m_yBoundMax = m_yMax;
		};

		BoundedRect(int xMin, int yMin, int xMax, int yMax, int buffer) :
			m_xMin(xMin),
			m_yMin(yMin),
			m_xMax(xMax),
			m_yMax(yMax),
			m_buffer(buffer) {
			m_xBoundMin = m_xMin;
			m_xBoundMax = m_xMax;
			m_yBoundMin = m_yMin;
			m_yBoundMax = m_yMax;
		};

		void Encompass(int x, int y)
		{
			if (x - m_buffer < m_xMin) m_xMin = x - m_buffer;
			if (x + m_buffer > m_xMax) m_xMax = x + m_buffer;
			if (y - m_buffer < m_yMin) m_yMin = y - m_buffer;
			if (y + m_buffer > m_yMax) m_yMax = y + m_buffer;
			ClampBounds();
		}

		void Encompass(const Position& position)
		{
			Encompass(position.m_x, position.m_y);
		}

		void Reset()
		{
			m_xMin = m_xBoundMax;
			m_xMax = m_xBoundMin;
			m_yMin = m_yBoundMax;
			m_yMax = m_yBoundMin;
		}

		void SetBuffer(int buffer)
		{
			m_buffer = buffer;
			Encompass(m_xMin, m_yMin);
			Encompass(m_xMax, m_yMax);
		};

		void SetBounds(int xMin, int yMin, int xMax, int yMax)
		{
			m_xBoundMin = xMin;
			m_xBoundMax = xMax;
			m_yBoundMin = yMin;
			m_yBoundMax = yMax;
			ClampBounds();
		};

		bool Contains(int x, int y) const {
			return (x >= m_xMin &&
					x <= m_xMax &&
					y >= m_yMin &&
					y <= m_yMax);
		}

	private:
		int m_xBoundMin, m_yBoundMin, m_xBoundMax, m_yBoundMax;
		int m_buffer;

		void ClampBounds() {
			m_xMin = std::clamp(m_xMin, m_xBoundMin, m_xBoundMax);
			m_xMax = std::clamp(m_xMax, m_xBoundMin, m_xBoundMax);
			m_yMin = std::clamp(m_yMin, m_yBoundMin, m_yBoundMax);
			m_yMax = std::clamp(m_yMax, m_yBoundMin, m_yBoundMax);
		};
	} BoundedRect;

	class Grid
	{
	public:
		Grid() : Grid(5, 5) {};
		Grid(size_t width, size_t height);
		Grid(const Grid& other);
		~Grid();

		// Functions intended for non-threaded use
		void SetCell(int x, int y);
		void SetCell(int x, int y, bool value, bool markDirty);
		void SetCell(const Position& position, bool value, bool markDirty);
		Cell* GetAt(int x, int y);
		Cell* GetAt(const Position& position);
		bool IsCellAlive(int x, int y);
		bool IsCellAlive(const Position& position);

		void AddDirtyBounds(int x, int y);
		void ClearDirtyBounds();
		void IterateDirtyRect(std::function<void(int, int)> lambda);
		bool IsDirty(int x, int y);

		size_t GetNeighborCountOfCell(const Position& position);
		void IterateGrid(std::function<void(int, int)> lambda);

		void Clear();

		inline size_t GetWidth() const { return m_width; }
		inline size_t GetHeight() const { return m_height; }
	private:
		std::unique_ptr<Cell[]> m_buffer;

		size_t m_width;
		size_t m_height;

		BoundedRect m_dirtyRect;

		// mutable to preserve const correctness of accessors without threading issues
		mutable std::shared_mutex m_mutex;

		inline size_t ToLinearIndex(int x, int y) const;
		inline size_t ToLinearIndex(const Position& position) const;
		inline size_t MaxLinearIndex() const;
		inline bool IsBounded(const Position& position) const;
		inline bool IsBounded(int x, int y) const;
	};

	std::string GridToString(Grid& grid);
}