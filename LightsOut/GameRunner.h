#ifndef _GAME_RUNNER_H
#define _GAME_RUNNER_H

class GameRunner {
private:
	Board *original_board;
	Board *player_board;
	Board *optimal_path_board;
	BoardRenderer *original_board_renderer;
	BoardRenderer *player_board_renderer;
	BoardRenderer *optimal_path_board_renderer;
	int start_height, start_width, end_width, end_height;
	bool is_available(std::vector<std::vector<int> > &field, Direction direction, int curr_h, int curr_w);
	bool check_path(std::vector<std::pair<int, int> > &path);
public:
	GameRunner(Board *original_board);
	void print_starting_message();
	void start_game();
	Board* get_original_board();
	Board* get_player_board();
	void set_original_board(Board *original_board);
	void set_player_board(Board *player_board);
	void calculate_optimal_path(std::vector<std::pair<int, int> > &optimal_path);
};

#endif
