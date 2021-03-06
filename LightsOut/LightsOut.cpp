#include "pch.h"
#include <ctime>
#include "Game.h"
#include<iostream>

int main(int argc, char** argv) {
	srand(time(NULL));
	//srand(5);
/*
	Board *board = new Board(6, 6);
	BoardRenderer *renderer = new BoardRenderer(board);
	TerrainGenerator *generator = new TerrainGenerator(board, renderer);

	if (generator->start_generating()) {
		std::cout << "Terrain successfully generated!\n" ;
	}
	else {
		std::cout << "Terrain generation failed!\n" ;
	}

	GameRunner *game = new GameRunner(board);
	game->print_starting_message();
	game->start_game();
	*/

	std::cout << "Please enter the difficulty level: " << std::endl;
	std::cout << "Enter 1 for EASY" << std::endl;
	std::cout << "Enter 2 for NORMAL" << std::endl;
	std::cout << "Enter 3 for HARD" << std::endl;
	
	int difficulty;

	std::cin >> difficulty;

	Game::Difficulty eDifficulty;

	switch (difficulty)
	{
	case 1:
		eDifficulty = Game::EEasy;
		break;
	case 2:
		eDifficulty = Game::ENormal;
		break;
	case 3:
		eDifficulty = Game::EHard;
		break;
	default:
		break;
	}

	Game game(eDifficulty);
	bool success = game.Initialize();
	if (success)
	{
		game.RunLoop();
	}
	game.Shutdown();

	return 0;
}
