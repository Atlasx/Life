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

	} Position;

	class Grid
	{
	public:
		Grid() : Grid(5, 5) {};
		Grid(size_t width, size_t height);
		Grid(const Grid& other);
		~Grid();

		// Functions intended for non-threaded use
		void SetCell(const Position& position, bool value);
		const Cell& ReadCell(const Position& position);

		void Clear();

	private:
		Cell* m_buffer;

		size_t m_width;
		size_t m_height;

		std::shared_mutex m_mutex;


		inline size_t ToLinearIndex(const Position& position) const;
		inline size_t MaxLinearIndex() const;
	};
}