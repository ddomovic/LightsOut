#include "pch.h"
#include <iostream>
#include <vector>
#include <ctime>
#include <iterator>
#include "Board.h"
#include "BoardRenderer.h"
#include "EnumDirection.h"
#include "GameRunner.h"

GameRunner::GameRunner(Board *original_board) {
	this->original_board = original_board;
	std::vector<std::vector<int> > field_data = original_board->get_data(), temp;
	
	for (std::vector<std::vector<int> >::iterator it = field_data.begin(); it != field_data.end(); ++it) {
		std::vector<int> temp_data;
		for (std::vector<int>::iterator it2 = it->begin(); it2 != it->end(); ++it2) {
			if (*it2 == START_POINT) {
				temp_data.push_back(*it2);
				this->start_height = it - field_data.begin();
				this->start_width = it2 - it->begin();
			}
			else if (*it2 == END_POINT) {
				temp_data.push_back(*it2);
				this->end_height = it - field_data.begin();
				this->end_width = it2 - it->begin();
			}
			else {
				temp_data.push_back(NOTHING);
			}
		}
		temp.push_back(temp_data);
	}

	this->player_board = new Board(temp);
	this->original_board_renderer = new BoardRenderer(this->original_board);
	this->player_board_renderer = new BoardRenderer(this->player_board);
	
	this->optimal_path_board = new Board(field_data);
	for (int i = 0; i < this->optimal_path_board->get_data().size(); i++) {
		for (int j = 0; j < this->optimal_path_board->get_data()[0].size(); j++) {
			if (this->optimal_path_board->get_data()[i][j] == LINE_SEGMENT) {
				this->optimal_path_board->set_specific_tile(NOTHING, i, j);
			}
		}
	}
	this->optimal_path_board_renderer = new BoardRenderer(this->optimal_path_board);
}

void GameRunner::print_starting_message() {
	std::cout << "\nTerrain generated. Press any key to start playing..." << std::endl;
	getchar();
	std::cout << "Controls: WASD\n";
}

bool GameRunner::is_available(std::vector<std::vector<int> > &field, Direction direction, int curr_h, int curr_w) {
	if (direction == UP) {
		if (curr_h - 1 < 0) {
			return false;
		}
		if (field[curr_h - 1][curr_w] == LINE_SEGMENT || field[curr_h - 1][curr_w] == START_POINT) {
			return false;
		}
	}
	else if (direction == DOWN) {
		if (curr_h + 1 > field.size() - 1) {
			return false;
		}
		if (field[curr_h + 1][curr_w] == LINE_SEGMENT || field[curr_h + 1][curr_w] == START_POINT) {
			return false;
		}
	}
	else if (direction == LEFT) {
		if (curr_w - 1 < 0) {
			return false;
		}
		if (field[curr_h][curr_w - 1] == LINE_SEGMENT || field[curr_h][curr_w - 1] == START_POINT) {
			return false;
		}
	}
	else {
		if (curr_w + 1 > field[0].size() - 1) {
			return false;
		}
		if (field[curr_h][curr_w + 1] == LINE_SEGMENT || field[curr_h][curr_w + 1] == START_POINT) {
			return false;
		}
	}

	return true;
}

bool GameRunner::check_path(std::vector<std::pair<int, int> > &path) {
	
	std::vector<std::vector<int> > original_field = this->original_board->get_data();

	for (std::vector<std::pair<int, int> >::iterator it = path.begin(); it != path.end(); ++it) {
		if (original_field[it->first][it->second] == WALL) {
			return false;
		}
	}

	return true;
}

void GameRunner::calculate_optimal_path(std::vector<std::pair<int, int> > &optimal_path) {
	std::vector<std::pair<int, int> > visited_nodes;
	std::vector<std::pair<int, int> > predecessor_of_visited_nodes;
	visited_nodes.push_back(std::make_pair(this->start_height, this->start_width));
	predecessor_of_visited_nodes.push_back(std::make_pair(0, 0));
	std::vector<std::pair<int, int> > working_nodes;
	working_nodes.push_back(std::make_pair(this->start_height, this->start_width));

	while (!working_nodes.empty()) {
		std::pair<int, int> w = working_nodes.front();
		working_nodes.erase(working_nodes.begin());
		if (w.first - 1 >= 0) {
			if (find(visited_nodes.begin(), visited_nodes.end(), std::make_pair(w.first - 1, w.second)) == visited_nodes.end()) {
				int d = this->optimal_path_board->get_data()[w.first - 1][w.second];
				if (d == END_POINT) {
					visited_nodes.push_back(std::make_pair(w.first - 1, w.second));
					predecessor_of_visited_nodes.push_back(std::make_pair(w.first, w.second));
					break;
				}
				else if (d != WALL) {
					visited_nodes.push_back(std::make_pair(w.first - 1, w.second));
					predecessor_of_visited_nodes.push_back(std::make_pair(w.first, w.second));
					working_nodes.push_back(std::make_pair(w.first - 1, w.second));
				}
			}
		}
		if (w.first + 1 < this->optimal_path_board->get_height()) {
			if (find(visited_nodes.begin(), visited_nodes.end(), std::make_pair(w.first + 1, w.second)) == visited_nodes.end()) {
				int d = this->optimal_path_board->get_data()[w.first + 1][w.second];
				if (d == END_POINT) {
					visited_nodes.push_back(std::make_pair(w.first + 1, w.second));
					predecessor_of_visited_nodes.push_back(std::make_pair(w.first, w.second));
					break;
				}
				else if (d != WALL) {
					visited_nodes.push_back(std::make_pair(w.first + 1, w.second));
					predecessor_of_visited_nodes.push_back(std::make_pair(w.first, w.second));
					working_nodes.push_back(std::make_pair(w.first + 1, w.second));
				}
			}
		}
		if (w.second - 1 >= 0) {
			if (find(visited_nodes.begin(), visited_nodes.end(), std::make_pair(w.first, w.second - 1)) == visited_nodes.end()) {
				int d = this->optimal_path_board->get_data()[w.first][w.second - 1];
				if (d == END_POINT) {
					visited_nodes.push_back(std::make_pair(w.first, w.second - 1));
					predecessor_of_visited_nodes.push_back(std::make_pair(w.first, w.second));
					break;
				}
				else if (d != WALL) {
					visited_nodes.push_back(std::make_pair(w.first, w.second - 1));
					predecessor_of_visited_nodes.push_back(std::make_pair(w.first, w.second));
					working_nodes.push_back(std::make_pair(w.first, w.second - 1));
				}
			}
		}
		if (w.second + 1 < this->optimal_path_board->get_width()) {
			if (find(visited_nodes.begin(), visited_nodes.end(), std::make_pair(w.first, w.second + 1)) == visited_nodes.end()) {
				int d = this->optimal_path_board->get_data()[w.first][w.second + 1];
				if (d == END_POINT) {
					visited_nodes.push_back(std::make_pair(w.first, w.second + 1));
					predecessor_of_visited_nodes.push_back(std::make_pair(w.first, w.second));
					break;
				}
				else if (d != WALL) {
					visited_nodes.push_back(std::make_pair(w.first, w.second + 1));
					predecessor_of_visited_nodes.push_back(std::make_pair(w.first, w.second));
					working_nodes.push_back(std::make_pair(w.first, w.second + 1));
				}
			}
		}
	}

	int index = visited_nodes.size() - 1;
	while (index > 0) {
		index = std::distance(visited_nodes.begin(), find(visited_nodes.begin(), 
			visited_nodes.end(), predecessor_of_visited_nodes[index]));
		optimal_path.insert(optimal_path.begin(), visited_nodes[index]);
	}
}

void GameRunner::start_game() {

	this->original_board_renderer->draw_board(false);
	time_t start_time = time(NULL);
	time_t current_time = time(NULL);

	while (current_time - start_time < 3) {
		current_time = time(NULL);
	}

	char key_input;
	int current_height = start_height;
	int current_width = start_width;
	Direction direction;
	std::vector<std::pair<int, int>> path_vector;
	while (1) {

		std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
		this->player_board_renderer->draw_board();
		std::cin >> key_input;

		std::vector<std::vector<int> > player_field = this->player_board->get_data();

		if (key_input == 'w') {
			if (is_available(player_field, UP, current_height, current_width)) {
				--current_height;
			}
			else {
				std::cout << "Unavailable input!\n";
				continue;
			}
		}
		else if (key_input == 's') {
			if (is_available(player_field, DOWN, current_height, current_width)) {
				++current_height;
			}
			else {
				std::cout << "Unavailable input!\n";
				continue;
			}
		}
		else if (key_input == 'a') {
			if (is_available(player_field, LEFT, current_height, current_width)) {
				--current_width;
			}
			else {
				std::cout << "Unavailable input!\n";
				continue;
			}
		}
		else if (key_input == 'd') {
			if (is_available(player_field, RIGHT, current_height, current_width)) {
				++current_width;
			}
			else {
				std::cout << "Unavailable input!\n";
				continue;
			}
		}
		else {
			std::cout << "Invalid input!\n";
			continue;
		}


		if (current_height == end_height && current_width == end_width) {
			break;
		}
		else {
			this->player_board->set_specific_tile(LINE_SEGMENT, current_height, current_width);
			path_vector.push_back(std::make_pair(current_height, current_width));
		}
	}

	if (!check_path(path_vector)) {
		std::cout << "YOU SUCK!\n";
	}
	else {
		std::cout << "GOOD JOB!\n";
	}
	std::cout << "ORIGINAL BOARD:\n";
	this->original_board_renderer->draw_board();
	std::cout << std::endl;
	std::cout << "PLAYER'S BOARD:\n";
	this->player_board_renderer->draw_board();
	std::cout << std::endl;
	std::vector<std::pair<int, int> > optimal_path;
	this->calculate_optimal_path(optimal_path);
	for (int i = 0; i < optimal_path.size(); i++) {
		this->optimal_path_board->set_specific_tile(LINE_SEGMENT, optimal_path[i].first, optimal_path[i].second);
	}
	std::cout << "OPTIMAL PATH BOARD:\n";
	this->optimal_path_board_renderer->draw_board();
	std::cout << "YOUR PATH SIZE: " << path_vector.size() + 1 << std::endl;
	std::cout << "OPTIMAL PATH SIZE: " << optimal_path.size() << std::endl;
}

Board* GameRunner::get_original_board() {
	return this->original_board;
}

Board* GameRunner::get_player_board() {
	return this->player_board;
}

void GameRunner::set_original_board(Board *original_board) {
	this->original_board = original_board;
}

void GameRunner::set_player_board(Board *player_board) {
	this->player_board = player_board;
}