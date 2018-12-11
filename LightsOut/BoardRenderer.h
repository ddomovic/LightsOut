#ifndef _BOARD_RENDERER_H
#define _BOARD_RENDERER_H

#include <vector>

class BoardRenderer {
private:
	Board *game_board;
public:
	BoardRenderer(Board *game_board);
	void draw_board(bool draw_lines = true);
	Board* get_game_board();
	void set_game_board(Board *game_board);
};

#endif
