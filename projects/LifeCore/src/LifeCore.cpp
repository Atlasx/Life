#include "LifeCore.h"

#include <iostream>
#include <random>
#include <cassert>

namespace LifeCore
{

	Game::Game() :
		m_width(5),
		m_height(5)
	{}

	Game::Game(size_t width, size_t height) :
		m_width(width),
		m_height(height)
	{}

	Game::~Game() {}

	void Game::Initialize(const GameConfig& config)
	{
		m_width = config.width;
		m_height = config.height;
		m_stepCount = 0;
		m_stepTarget = config.stepTarget;
		m_loggingEnabled = config.logging;

		m_gridA = std::make_shared<Grid>(m_width, m_height);
		m_gridB = std::make_shared<Grid>(m_width, m_height);
		m_currentGrid = m_gridA;
		m_nextGrid = m_gridB;

		m_initialized = true;

		switch (config.startingState)
		{
		case GameConfig::StartingState::Random:
		{
			Randomize();
			break;
		}
		case GameConfig::StartingState::Glider:
		{
			SetCell(2, 0);
			SetCell(0, 1);
			SetCell(2, 1);
			SetCell(1, 2);
			SetCell(2, 2);
			break;
		}
		case (GameConfig::StartingState::Patterns):
		{
			SetCell(10, 10);
			SetCell(12, 10);
			SetCell(13, 10);
			SetCell(10, 11);
			SetCell(13, 11);
			SetCell(14, 11);
			SetCell(14, 12);
			SetCell(10, 13);
			SetCell(10, 14);
			SetCell(14, 14);
			break;
		}
		case (GameConfig::StartingState::Custom):
			[[fallthrough]];
		default:
			break;
		}

		printf("Initialized State: \nWidth: %lld\nHeight: %lld\nLogging: %s\nIterations: %lld\nPreset: %s\n",
			m_width, m_height, m_loggingEnabled ? "true" : "false", m_stepTarget, GameConfig::GetPresetName(config.startingState).c_str());
	
	}

	void Game::Run()
	{
		assert(m_initialized);

		// assumes m_stepCount is incremented elsewhere, not a good plan
		while (m_stepCount <= m_stepTarget)
		{
			Step();
		}
	}

	void Game::Step()
	{
		ApplyRuleset();
		
		// swap current and next
		std::swap(m_currentGrid, m_nextGrid);

		m_stepCount++;

		if (m_loggingEnabled) {
			LogGrid();
		}
	}

	void Game::Reset()
	{
		m_gridA->Clear();
		m_gridB->Clear();
		m_currentGrid = m_gridA;
		m_stepCount = 0;
	}

	void Game::Randomize()
	{
		static std::mt19937 gen;
		static std::uniform_int_distribution<int> distribution(0,1);
		
		assert(m_initialized);

		for (int j = 0; j < m_height; j++)
		{
			for (int i = 0; i < m_width; i++)
			{
				const bool value = distribution(gen);
				m_currentGrid->SetCell(i, j, value, true);
				m_nextGrid->SetCell(i, j, value, true);
			}
		}
	}

	void Game::SetCell(int x, int y)
	{
		m_currentGrid->SetCell(x, y);
		m_nextGrid->SetCell(x, y);
	}

	bool Game::LoadGrid(std::string path)
	{
		return false;
	}

	void Game::LogGrid()
	{
		assert(m_initialized);

		printf("Step Count: %lld\n", m_stepCount);
		std::string gridString = GridToString(*m_currentGrid);
		printf(gridString.c_str());
	}

	void Game::ApplyRuleset()
	{
		m_currentGrid->IterateDirtyRect([&](int x, int y){
			size_t neighborCount = m_currentGrid->GetNeighborCountOfCell(Position(x, y));

			Cell* current = m_currentGrid->GetAt(Position(x, y));
			Cell* next = m_nextGrid->GetAt(Position(x, y));
			assert(current != nullptr);
			assert(next != nullptr);

			const bool currentIsAlive = current->IsAlive();
			bool nextIsAlive = currentIsAlive;

			if (currentIsAlive) {
				if (neighborCount < 2 || neighborCount > 3) {
					nextIsAlive = false;
				}
			}
			else {
				if (neighborCount == 3) {
					nextIsAlive = true;
				}
			}

			const bool markDirty = (currentIsAlive != nextIsAlive);
			m_nextGrid->SetCell(x, y, nextIsAlive, markDirty);
		});

		m_currentGrid->ClearDirtyBounds();
	}
}