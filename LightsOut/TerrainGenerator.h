#ifndef _TERRAIN_GENERATOR_H
#define _TERRAIN_GENERATOR_H

#include <vector>
#include "EnumDirection.h"

#define WALLS_PERCENTAGE 0.7
#define PERCENTAGE2_MAIN 40
#define PERCENTAGE2_OTHER 10
#define PERCENTAGE1_MAIN 70
#define PERCENTAGE1_OTHER 10

class TerrainGenerator {
private:
	Board *game_board;
	BoardRenderer *renderer;
	std::vector<Direction> choose_direction(int current_height, int current_width, int end_height, int end_width);
	int is_forbidden(std::vector<std::vector<int> > &field, int vertical, int horizontal);
public:
	TerrainGenerator(Board *game_board, BoardRenderer *renderer);
	bool start_generating();
	bool determine_start_and_end();
	bool determine_path();
	bool place_walls();
	Board* get_game_board();
	void set_game_board(Board *game_board);
	BoardRenderer* get_renderer();
	void set_renderer(BoardRenderer *renderer);
};

#endif

