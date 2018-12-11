#include "pch.h"
#include <iostream>
#include <vector>
#include "Board.h"
#include "BoardRenderer.h"

BoardRenderer::BoardRenderer(Board *game_board) {
	this->game_board = game_board;
}

// draw the board, draw nothing on the line segment tiles if draw_lines == false
void BoardRenderer::draw_board(bool draw_lines) {
	
	std::vector<std::vector<int> > field = this->game_board->get_data();

	std::cout << "+ ";
	for (int i = 0; i < field[0].size(); i++) {
		std::cout << "- ";
	}
	std::cout << "+\n";

	for (std::vector<std::vector<int> >::iterator it = field.begin(); it != field.end(); ++it) {
		std::cout << "| ";
		for (std::vector<int>::iterator it2 = it->begin(); it2 != it->end(); ++it2) {
			if (*it2 == NOTHING) {
				std::cout << "  ";
			}
			else if (*it2 == LINE_SEGMENT) {
				if (draw_lines) {
					std::cout << "+ ";
				}
				else {
					std::cout << "  ";
				}
			}
			else if (*it2 == WALL) {
				std::cout << "# ";
			}
			else {
				std::cout << *it2 << " ";
			}
		}
		std::cout << "|\n";
	}

	std::cout << "+ ";
	for (int i = 0; i < field[0].size(); i++) {
		std::cout << "- ";
	}
	std::cout << "+\n";
}

void BoardRenderer::set_game_board(Board *game_board) {
	this->game_board = game_board;
}

Board* BoardRenderer::get_game_board() {
	return this->game_board;
}