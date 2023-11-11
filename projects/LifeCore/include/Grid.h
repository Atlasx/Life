#pragma once

#include <shared_mutex>
#include <thread>

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

	class Grid
	{
	public:
		Grid() : Grid(5, 5) {};
		Grid(size_t width, size_t height);
		Grid(const Grid& other);
		~Grid();

		// Functions intended for non-threaded use
		void SetCell(int x, int y, bool value);
		void SetCell(const Position& position, bool value);
		Cell* GetAt(int x, int y);
		Cell* GetAt(const Position& position);
		bool IsCellAlive(int x, int y);
		bool IsCellAlive(const Position& position);

		size_t GetNeighborCountOfCell(const Position& position);

		void Clear();

		inline size_t GetWidth() const { return m_width; }
		inline size_t GetHeight() const { return m_height; }
	private:
		std::unique_ptr<Cell[]> m_buffer;

		size_t m_width;
		size_t m_height;

		// mutable to preserve const correctness of accessors without threading issues
		mutable std::shared_mutex m_mutex;

		inline size_t ToLinearIndex(int x, int y) const;
		inline size_t ToLinearIndex(const Position& position) const;
		inline size_t MaxLinearIndex() const;
		inline bool IsBounded(const Position& position) const;
		inline bool IsBounded(int x, int y) const;
	};

	std::string GridToString(const Grid& grid);
}