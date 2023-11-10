#include "LifeCore.h"


namespace LifeCore
{

	void Game::Initialize()
	{
		m_gridA = new Grid(m_width, m_height);
		m_gridB = new Grid(m_width, m_height);
		m_currentGrid = m_gridA;
	}

	void Game::Step()
	{
		printf("Stepping\n");
	}

	void Game::Reset()
	{

	}

	bool Game::LoadGrid(std::string path)
	{
		return false;
	}
}