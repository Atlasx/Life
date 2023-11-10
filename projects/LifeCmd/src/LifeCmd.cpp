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


int main()
{
	std::cout << "Hello World!\n";

	LifeCore::Game myGame;
	myGame.Initialize();
	myGame.LogGrid();
	myGame.Randomize();
	myGame.LogGrid();
	myGame.Randomize();
	myGame.LogGrid();

	std::chrono::high_resolution_clock clock;
	auto startTime = clock.now();

	int iterations = 50000;
	for (int i = 0; i < iterations; i++)
	{
		myGame.Step();
		if (i < 10) {
			myGame.LogGrid();
		}
	}

	myGame.LogGrid();

	auto endTime = clock.now();

	printf("%d iterations in %f s\n", iterations, std::chrono::duration<double>(endTime - startTime));
}