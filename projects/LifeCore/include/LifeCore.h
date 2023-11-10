#pragma once

#include <string>

#include "Grid.h"

namespace LifeCore
{

	class Game
	{
	public:
		void Initialize();
		void Step();
		void Reset();

		bool LoadGrid(std::string path);
	private:
		// Keep two grid states, one to read from the previous frame and another to write to the next frame
		Grid* m_gridA;
		Grid* m_gridB;
		Grid* m_currentGrid;

		size_t m_width;
		size_t m_height;
		
	};

}