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
	//helper function that randomly chooses a direction
	//directions pointing towards the end point have a higher chance of being selected
	std::vector<Direction> choose_direction(int current_height, int current_width, int end_height, int end_width);
	//helper function that checks if the tile is a starting point or a line segment
	int is_forbidden(std::vector<std::vector<int> > &field, int vertical, int horizontal);
public:
	TerrainGenerator(Board *game_board, BoardRenderer *renderer);
	//begin the process of generating, return true if successful
	//the sequence is:
	//1. determine_start_and_end()
	//2. determine_path()
	//3. place_walls()
	bool start_generating();
	//decide the positions of the start and end points
	//start point will be randomly chosen in one of the corners and the end point will be in the opposite corner
	//actual coordinate of the point can be a maximum distance of 0.25*board_dimension (height or width)
	//from the actual corner (chosen randomly)
	bool determine_start_and_end();
	//generate a pseudo-random path from start to finish
	bool determine_path();
	//place walls within the board, on WALL_PERCETANGE of free spaces (no start/end points or line segments)
	bool place_walls();
	Board* get_game_board();
	void set_game_board(Board *game_board);
	BoardRenderer* get_renderer();
	void set_renderer(BoardRenderer *renderer);
};

#endif

