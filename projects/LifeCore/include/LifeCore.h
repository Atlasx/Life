#pragma once

#include <string>

#include "Grid.h"

namespace LifeCore
{

	class Game
	{
	public:
		Game();
		~Game();

		void Initialize();
		void Step();
		void Reset();

		void Randomize();
		bool LoadGrid(std::string path);

		void LogGrid();
	private:
		// Keep two grid states, one to read from the previous frame and another to write to the next frame
		std::shared_ptr<Grid> m_gridA;
		std::shared_ptr<Grid> m_gridB;
		std::shared_ptr<Grid> m_currentGrid;
		std::shared_ptr<Grid> m_nextGrid;

		size_t m_width;
		size_t m_height;

		bool m_loggingEnabled = false;
		size_t m_threadedThreshold = 128;

		inline size_t GetCellCount() const { return m_width * m_height; }

		void ApplyRulesetThreaded();
		void ApplyRuleset();
	};

}