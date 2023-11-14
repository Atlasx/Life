#pragma once

#include <string>

#include "Grid.h"

namespace LifeCore
{
	struct GameConfig
	{
		size_t width;
		size_t height;

		bool logging;
		size_t stepCount;
	};

	class Game
	{
	public:
		Game();
		Game(size_t width, size_t height);
		~Game();

		void Initialize(const GameConfig& config);
		void Step();
		void Reset();

		void Randomize();
		void SetCell(int x, int y);
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

		size_t m_stepCount;

		inline size_t GetCellCount() const { return m_width * m_height; }

		void ApplyRulesetThreaded();
		void ApplyRuleset();
	};

}