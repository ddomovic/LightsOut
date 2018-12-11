#include "pch.h"
#include <iostream>
#include <ctime>
#include "Board.h"
#include "BoardRenderer.h"
#include "TerrainGenerator.h"
#include "GameRunner.h"

int main() {
	srand(time(NULL));

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

	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
