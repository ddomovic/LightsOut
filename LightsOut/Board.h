#ifndef _BOARD_H
#define _BOARD_H

#include <vector>

#define NOTHING 0
#define START_POINT 1
#define END_POINT 2
#define LINE_SEGMENT 3
#define WALL 4

class Board {
private:
	std::vector<std::vector<int> > data;
	short height, width;
public:
	Board();
	Board(short height_and_width);
	Board(short height, short width);
	Board(std::vector<std::vector<int> > &data);
	std::vector<std::vector<int> > get_data();
	short get_height();
	short get_width();
	void set_data(std::vector<std::vector<int> > &data);
	void set_specific_tile(int value, short h, short w);
};

#endif