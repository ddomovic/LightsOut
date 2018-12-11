#ifndef _BOARD_RENDERER_H
#define _BOARD_RENDERER_H

#include <vector>

// This class will most likey be entirely removed/changed in the later iterations
class BoardRenderer {
private:
	Board *game_board;
public:
	BoardRenderer(Board *game_board);
	// draw the board, draw nothing on the line segment tiles if draw_lines == false
	void draw_board(bool draw_lines = true);
	Board* get_game_board();
	void set_game_board(Board *game_board);
};

#endif
