// BRIEF:
// Write a program that simulates the Planter Box Simulation. (Conway's game of life)
//
// INPUT: 
// Starting board state - Please provide format so that we can try different board states.
// How many days to simulate
//
// OUTPUT:
// The planter box status after simulation.
// 
// DESCRIPTION:
// There is a planter box with 5 rows and 5 columns. Either slot on the planter box either has a plant or it does not have a plant.
//
// RULES:
//  - If a plant has fewer than two neighbors, it will die due to under-population
//  - If a plant has between 2 or 3 neighbors, it will survive until the next day
//  - If a plant has more than 3 neighbors, it will die due to overpopulation
//  - If an empty slot has exactly 3 neighbors, it will become a plant the next day
//  - Diagonals count as neighbors
//


#include <iostream>
#include <chrono>
#include "LifeCore.h"


class ScopeTimer
{
public:
	ScopeTimer() {
		m_start = m_clock.now();
	}

	~ScopeTimer() {
		m_end = m_clock.now();
		auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(m_end - m_start);

		printf("Duration: %lld ms\n", dur.count());
	}

private:
	std::chrono::high_resolution_clock m_clock;

	std::chrono::steady_clock::time_point m_start;
	std::chrono::steady_clock::time_point m_end;
};

int main()
{
	std::cout << "Hello World!\n";

	{
		// Some time estimates here:
		// 
		// 50x50 grid of cells: 2500 cells
		// 
		// each cell needs to read 9 neighboring cells + set result 
		// 2500 * 10 = 25000 cell reads
		// 
		// 50,000 iterations of the game
		// 25000 * 50000 = 1250000000 = 1.25 billion
		// 
		// CPU hertz on test machine: 3.7 Ghz
		// single threaded timing: ~2500ms
		// 
		// 3.7 * 1.25 = 4.675 avg cycles per operation
		//


		ScopeTimer timer;
		LifeCore::Game myGame(50, 50);
		myGame.Initialize();

		int iterations = 50000;
		for (int i = 0; i < iterations; i++)
		{
			myGame.Step();
			if (i % 100 == 0) {
				myGame.Randomize();
			}
		}

		myGame.LogGrid();
	}
}