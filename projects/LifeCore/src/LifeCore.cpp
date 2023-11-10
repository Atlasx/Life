#include "LifeCore.h"

#include <iostream>
#include <random>
#include <cassert>

namespace LifeCore
{

	Game::Game() :
		m_width(5),
		m_height(5),
		m_gridA(nullptr),
		m_gridB(nullptr),
		m_currentGrid(nullptr),
		m_nextGrid(nullptr)
	{}

	Game::~Game() {}

	void Game::Initialize()
	{
		m_gridA = std::make_shared<Grid>(m_width, m_height);
		m_gridB = std::make_shared<Grid>(m_width, m_height);
		m_currentGrid = m_gridA;
		m_nextGrid = m_gridB;
	}

	void Game::Step()
	{
		if (GetCellCount() > m_threadedThreshold) {
			ApplyRulesetThreaded();
		}
		else {
			ApplyRuleset();
		}
		
		// swap current and next
		std::swap(m_currentGrid, m_nextGrid);

		if (m_loggingEnabled) {
			LogGrid();
		}
	}

	void Game::Reset()
	{
		m_gridA->Clear();
		m_gridB->Clear();
		m_currentGrid = m_gridA;
	}

	void Game::Randomize()
	{
		static std::mt19937 gen;
		static std::uniform_int_distribution<int> distribution(0,1);

		for (size_t j = 0; j < m_height; j++)
		{
			for (size_t i = 0; i < m_width; i++)
			{
				m_currentGrid->SetCell(i, j, distribution(gen));
			}
		}
	}

	bool Game::LoadGrid(std::string path)
	{
		return false;
	}

	void Game::LogGrid()
	{
		std::string gridString = GridToString(*m_currentGrid);
		std::cout << gridString << std::endl;
	}

	void Game::ApplyRuleset()
	{
		for (size_t j = 0; j < m_height; j++)
		{
			for (size_t i = 0; i < m_width; i++)
			{
				size_t neighborCount = m_currentGrid->GetNeighborCountOfCell(Position(i, j));

				Cell* current = m_currentGrid->GetAt(Position(i, j));
				Cell* next = m_nextGrid->GetAt(Position(i, j));
				assert(current != nullptr);
				assert(next != nullptr);

				next->Set(current->IsAlive());

				if (current->IsAlive()) {
					if (neighborCount < 2 || neighborCount > 3) {
						next->Set(false);
					}
				}
				else {
					if (neighborCount == 3) {
						next->Set(true);
					}
				}
			}
		}
	}

	void Game::ApplyRulesetThreaded()
	{

	}
}