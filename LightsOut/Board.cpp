#include "pch.h"
#include <vector>
#include "Board.h"

Board::Board() : height(3), width(3) {
	std::vector<int> temp(this->width, NOTHING);
	std::vector<std::vector<int> > temp2(this->height, temp);
	this->data = temp2;
}

Board::Board(short height_and_width) : height(height_and_width), width(height_and_width) {
	std::vector<int> temp(this->width, NOTHING);
	std::vector<std::vector<int> > temp2(this->height, temp);
	this->data = temp2;
}

Board::Board(short height, short width) : height(height), width(width) {
	std::vector<int> temp(this->width, NOTHING);
	std::vector<std::vector<int> > temp2(this->height, temp);
	this->data = temp2;
}

Board::Board(std::vector<std::vector<int> > &data) : data(data) {
	this->height = data.size();
	this->width = data[0].size();
}

std::vector<std::vector<int> > Board::get_data() {
	return this->data;
}

short Board::get_height() {
	return this->height;
}

short Board::get_width() {
	return this->width;
}

void Board::set_data(std::vector<std::vector<int> > &data) {
	this->data = data;
	this->height = data.size();
	this->width = data[0].size();
}

void Board::set_specific_tile(int value, short h, short w) {
	this->data[h][w] = value;
}