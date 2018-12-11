#include "pch.h"
#include <vector>
#include <iostream>
#include "EnumDirection.h"
#include "Board.h"
#include "BoardRenderer.h"
#include "TerrainGenerator.h"

TerrainGenerator::TerrainGenerator(Board *game_board, BoardRenderer *renderer) : game_board(game_board), renderer(renderer) {
}

//begin the process of generating, return true if successful
//the sequence is:
//1. determine_start_and_end()
//2. determine_path()
//3. place_walls()
bool TerrainGenerator::start_generating() {

	this->renderer->draw_board();

	if (!determine_start_and_end()) {
		return false;
	}

	this->renderer->draw_board();

	if (!determine_path()) {
		return false;
	}

	this->renderer->draw_board();

	if (!place_walls()) {
		return false;
	}

	this->renderer->draw_board();

	return true;
}

//decide the positions of the start and end points
//start point will be randomly chosen in one of the corners and the end point will be in the opposite corner
//actual coordinate of the point can be a maximum distance of 0.25*board_dimension (height or width)
//from the actual corner (chosen randomly)
bool TerrainGenerator::determine_start_and_end() {
	
	if (this->game_board->get_height() * this->game_board->get_width() <= 1) {
		return false;
	}

	int width = this->game_board->get_width();
	int height = this->game_board->get_height();

	int aux = rand() % 2;
	int aux2 = rand() % 2;

	int width_quarter = (int)(floor(width * 0.25));
	int height_quarter = (int)(floor(height * 0.25));

	if (width_quarter == 0) {
		width_quarter++;
	}
	if (height_quarter == 0) {
		height_quarter++;
	}

	int chosen_width_index_start = width * aux;
	if (aux == 0) {
		chosen_width_index_start += rand() % width_quarter;
	}
	else {
		chosen_width_index_start -= 1;
		chosen_width_index_start -= rand() % width_quarter;
	}
	int chosen_height_index_start = height * aux2;
	if (aux2 == 0) {
		chosen_height_index_start += rand() % height_quarter;
	}
	else {
		chosen_height_index_start -= 1;
		chosen_height_index_start -= rand() % height_quarter;
	}

	int chosen_width_index_end = width - width * aux;
	if (aux == 0) {
		chosen_width_index_end -= 1;
		chosen_width_index_end -= rand() % width_quarter;
	}
	else {
		chosen_width_index_end += rand() % width_quarter;
	}
	int chosen_height_index_end = height - height * aux2;
	if (aux2 == 0) {
		chosen_height_index_end -= 1;
		chosen_height_index_end -= rand() % height_quarter;
	}
	else {
		chosen_height_index_end += rand() % height_quarter;
	}

	this->game_board->set_specific_tile(START_POINT, chosen_height_index_start, chosen_width_index_start);
	this->game_board->set_specific_tile(END_POINT, chosen_height_index_end, chosen_width_index_end);

	return true;
}

//helper function that randomly chooses a direction
//directions pointing towards the end point have a higher chance of being selected
std::vector<Direction> TerrainGenerator::choose_direction(int current_height, int current_width, int end_height, int end_width) {
	bool height_added = false, width_added = false;
	std::vector<Direction> to_return;
	std::vector<Direction> to_decide;
	int height_direction = 0, width_direction = 0;

	if (current_height - end_height < 0) {
		height_direction = 1;
	}
	else if (current_height - end_height > 0) {
		height_direction = -1;
	}

	if (current_width - end_width < 0) {
		width_direction = 1;
	}
	else if (current_width - end_width > 0) {
		width_direction = -1;
	}


	if (height_direction != 0 && width_direction != 0) {
		if (height_direction == 1) {
			for (int i = 0; i < PERCENTAGE2_MAIN; i++) {
				to_decide.push_back(DOWN);
			}
			for (int i = 0; i < PERCENTAGE2_OTHER; i++) {
				to_decide.push_back(UP);
			}
		}
		else {
			for (int i = 0; i < PERCENTAGE2_MAIN; i++) {
				to_decide.push_back(UP);
			}
			for (int i = 0; i < PERCENTAGE2_OTHER; i++) {
				to_decide.push_back(DOWN);
			}
		}

		if (width_direction == 1) {
			for (int i = 0; i < PERCENTAGE2_MAIN; i++) {
				to_decide.push_back(RIGHT);
			}
			for (int i = 0; i < PERCENTAGE2_OTHER; i++) {
				to_decide.push_back(LEFT);
			}
		}
		else {
			for (int i = 0; i < PERCENTAGE2_MAIN; i++) {
				to_decide.push_back(LEFT);
			}
			for (int i = 0; i < PERCENTAGE2_OTHER; i++) {
				to_decide.push_back(RIGHT);
			}
		}
	}
	else if (height_direction != 0) {
		if (height_direction == 1) {
			for (int i = 0; i < PERCENTAGE1_MAIN; i++) {
				to_decide.push_back(DOWN);
			}
			for (int i = 0; i < PERCENTAGE1_OTHER; i++) {
				to_decide.push_back(UP);
				to_decide.push_back(LEFT);
				to_decide.push_back(RIGHT);
			}
		}
		else {
			for (int i = 0; i < PERCENTAGE1_MAIN; i++) {
				to_decide.push_back(UP);
			}
			for (int i = 0; i < PERCENTAGE1_OTHER; i++) {
				to_decide.push_back(DOWN);
				to_decide.push_back(LEFT);
				to_decide.push_back(RIGHT);
			}
		}
	}
	else if (width_direction != 0) {
		if (width_direction == 1) {
			for (int i = 0; i < PERCENTAGE1_MAIN; i++) {
				to_decide.push_back(RIGHT);
			}
			for (int i = 0; i < PERCENTAGE1_OTHER; i++) {
				to_decide.push_back(UP);
				to_decide.push_back(LEFT);
				to_decide.push_back(DOWN);
			}
		}
		else {
			for (int i = 0; i < PERCENTAGE1_MAIN; i++) {
				to_decide.push_back(LEFT);
			}
			for (int i = 0; i < PERCENTAGE1_OTHER; i++) {
				to_decide.push_back(DOWN);
				to_decide.push_back(UP);
				to_decide.push_back(RIGHT);
			}
		}
	}
	else {
		return to_return;
	}

	for (int i = 0; i < 4; i++) {
		Direction temp = to_decide[rand() % to_decide.size()];
		to_return.push_back(temp);
		std::vector<Direction> aux_vec;
		for (int j = 0; j < to_decide.size(); j++) {
			if (to_decide[j] != temp) {
				aux_vec.push_back(to_decide[j]);
			}
		}
		to_decide = aux_vec;
	}

	return to_return;
}

//helper function that checks if the tile is a starting point or a line segment
int TerrainGenerator::is_forbidden(std::vector<std::vector<int> > &field, int vertical, int horizontal) {

	if ((field[vertical][horizontal] == START_POINT) || (field[vertical][horizontal] == LINE_SEGMENT)) {
		return 1;
	}

	return 0;
}

//generate a pseudo-random path from start to finish
bool TerrainGenerator::determine_path() {
	std::vector<Direction> directions;
	std::vector<std::vector<int> > temp_field = this->game_board->get_data();
	std::vector<std::vector<int> > field = this->game_board->get_data();

	int start_height, start_width, end_height, end_width;

	for (std::vector<std::vector<int> >::iterator it = field.begin(); it != field.end(); ++it) {
		for (std::vector<int>::iterator it2 = it->begin(); it2 != it->end(); ++it2) {
			if (*it2 == START_POINT) {
				start_width = std::distance(it->begin(), it2);
				start_height = std::distance(field.begin(), it);
			}
			else if (*it2 == END_POINT) {
				end_width = std::distance(it->begin(), it2);
				end_height = std::distance(field.begin(), it);
			}
		}
	}

	int current_height = start_height;
	int current_width = start_width;

	while (1) {

		std::vector<Direction> dir_vec = choose_direction(current_height, current_width, end_height, end_width);
		int horizontal_direction;
		int vertical_direction;
		bool success = false;

		for (int i = 0; i < 4; i++) {

			if (dir_vec[i] == UP) {
				horizontal_direction = 0;
				vertical_direction = -1;
			}
			else if (dir_vec[i] == DOWN) {
				horizontal_direction = 0;
				vertical_direction = 1;
			}
			else if (dir_vec[i] == RIGHT) {
				horizontal_direction = 1;
				vertical_direction = 0;
			}
			else if (dir_vec[i] == LEFT) {
				horizontal_direction = -1;
				vertical_direction = 0;
			}
			else {
				horizontal_direction = 0;
				vertical_direction = 0;
			}

			if ((current_height + vertical_direction < 0) || (current_height + vertical_direction >= field.size())) {
				continue;
			}
			if ((current_width + horizontal_direction < 0) || (current_width + horizontal_direction >= field[0].size())) {
				continue;
			}
			if (is_forbidden(temp_field, current_height + vertical_direction, current_width + horizontal_direction)) {
				continue;
			}

			current_height += vertical_direction;
			current_width += horizontal_direction;
			success = true;
			break;
		}

		if (!success) {
			current_height = start_height;
			current_width = start_width;
			temp_field = field;
			directions.clear();
			std::cout << "RESETING!\n";
			continue;
		}

		if ((current_height == end_height) && (current_width == end_width)) {
			break;
		}

		temp_field[current_height][current_width] = LINE_SEGMENT;

		if (horizontal_direction == 1) {
			directions.push_back(RIGHT);
		}
		else if (horizontal_direction == -1) {
			directions.push_back(LEFT);
		}
		else if (vertical_direction == 1) {
			directions.push_back(DOWN);
		}
		else if (vertical_direction == -1) {
			directions.push_back(UP);
		}
	}

	for (int i = 0; i < directions.size(); i++) {
		if (directions[i] == UP) {
			std::cout << "UP, ";
		}
		else if (directions[i] == DOWN) {
			std::cout << "DOWN, ";
		}
		else if (directions[i] == LEFT) {
			std::cout << "LEFT, ";
		}
		else if (directions[i] == RIGHT) {
			std::cout << "RIGHT, ";
		}
	}

	this->game_board->set_data(temp_field);

	return true;
}

//place walls within the board, on WALL_PERCETANGE of free spaces (no start/end points or line segments)
bool TerrainGenerator::place_walls() {
	
	int free_spaces = 0;
	std::vector<std::pair<int, int> > free_spaces_vec;
	std::vector<std::vector<int> > field = this->game_board->get_data();

	for (std::vector<std::vector<int> >::iterator it = field.begin(); it != field.end(); ++it) {
		for (std::vector<int>::iterator it2 = it->begin(); it2 != it->end(); ++it2) {
			if (*it2 == NOTHING) {
				free_spaces++;
				free_spaces_vec.push_back(std::make_pair(std::distance(field.begin(), it), std::distance(it->begin(), it2)));
			}
		}
	}

	int temp_size = free_spaces_vec.size();
	for (int i = 0; i < (int)(free_spaces*WALLS_PERCENTAGE); i++) {
		int index = rand() % temp_size;
		std::pair<int, int> temp_pair = free_spaces_vec[index];
		temp_size--;
		this->game_board->set_specific_tile(WALL, temp_pair.first, temp_pair.second);
		free_spaces_vec.erase(free_spaces_vec.begin() + index);
	}

	return true;
}

Board* TerrainGenerator::get_game_board() {
	return this->game_board;
}

void TerrainGenerator::set_game_board(Board *game_board) {
	this->game_board = game_board;
}

BoardRenderer* TerrainGenerator::get_renderer() {
	return this->renderer;
}

void TerrainGenerator::set_renderer(BoardRenderer *renderer) {
	this->renderer = renderer;
}