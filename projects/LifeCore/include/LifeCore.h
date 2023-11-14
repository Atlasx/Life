#pragma once

#include <string>
#include <optional>

#include "Grid.h"

namespace LifeCore
{
	struct GameConfig
	{
		enum class StartingState {
			Custom,
			Random,
			Glider,
			Patterns
		};

		size_t width = 3;
		size_t height = 3;
		bool logging = false;
		size_t stepTarget = 10;
		StartingState startingState = StartingState::Custom;

		GameConfig() {};

		GameConfig(size_t w, size_t h, bool log, size_t sTarget, StartingState sState) :
			width(w), height(h), logging(log), stepTarget(sTarget), startingState(sState) {};


		static std::string GetPresetName(const StartingState& state) {
			switch (state)
			{
			case StartingState::Random:
				return "Random";
			case StartingState::Custom:
				return "Custom";
			case StartingState::Glider:
				return "Glider";
			case StartingState::Patterns:
				return "Patterns";
			}
			return "";
		}
	};

	class Game
	{
	public:
		Game();
		Game(size_t width, size_t height);
		~Game();

		void Initialize(const GameConfig& config);
		void Run();
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
		size_t m_stepTarget;

		bool m_initialized = false;

		inline size_t GetCellCount() const { return m_width * m_height; }

		//void ApplyRulesetThreaded(); TODO: threaded approach
		void ApplyRuleset();
	};

}