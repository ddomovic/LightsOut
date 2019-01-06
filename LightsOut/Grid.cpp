#include "pch.h"
#include "Grid.h"
#include "Tile.h"
#include <algorithm>
#include <iostream>
#include "EnumDirection.h"
#include "Game.h"

Grid::Grid(class Game* game, const size_t rows, const size_t cols) : Actor(game), NumRows(rows), NumCols(cols)
{
	//resize the 2d vector to fit the exact number of rows and columns (7 rows and 7 columns)
	mTiles.resize(NumRows);
	for (size_t i = 0; i < mTiles.size(); ++i)
	{
		mTiles[i].resize(NumCols);
	}

	//create tile grid
	for (size_t i = 0; i < NumRows; ++i)
	{
		for (size_t j = 0; j < NumCols; ++j)
		{
			mTiles[i][j] = new Tile(GetGame());
			mTiles[i][j]->SetPosition(Vector2(StartX + TileSize / 2.0f + j * TileSize, StartY + i * TileSize));
		}
	}

	//set start and end tiles
	if (DetermineStartAndEndTile())
	{
		GetStartTile()->SetTileState(Tile::EStart);
		GetEndTile()->SetTileState(Tile::EEnd);
	}

	//adding adjacent tiles to the vector
	for (size_t i = 0; i < NumRows; ++i)
	{
		for (size_t j = 0; j < NumCols; ++j)
		{
			if (i > 0)
			{
				mTiles[i][j]->mAdjacent.push_back(mTiles[i - 1][j]);
			}
			if (i < NumRows - 1)
			{
				mTiles[i][j]->mAdjacent.push_back(mTiles[i + 1][j]);
			}
			if (j > 0)
			{
				mTiles[i][j]->mAdjacent.push_back(mTiles[i][j - 1]);
			}
			if (j < NumCols - 1)
			{
				mTiles[i][j]->mAdjacent.push_back(mTiles[i][j + 1]);
			}
		}
	}

	
	DeterminePath();
	PlaceWalls();
	ClearPath();
	//reverse so we dont have to backtrack to find the path
	FindPath(GetEndTile(), GetStartTile());
	UpdatePathTiles(GetStartTile());
}

void Grid::SelectTile(size_t row, size_t col)
{
	auto iter = std::find(mSelectedTiles.begin(), mSelectedTiles.end(), mTiles[row][col]);
	if (iter == mSelectedTiles.end())
	{
		mSelectedTiles.push_back(mTiles[row][col]);
		mTiles[row][col]->ToggleSelect();
	}	
}

void Grid::SetHidden()
{
	for (size_t i = 0; i < NumRows; ++i)
	{
		for (size_t j = 0; j < NumCols; ++j)
		{
			mTiles[i][j]->ToggleHidden();
		}
	}
}

void Grid::ProcessClickDown(int x, int y)
{
	x -= static_cast<int>(StartX);
	y -= static_cast<int>(StartY - TileSize / 2);
	if (y >= 0 && x >= 0)
	{
		x /= static_cast<int>(TileSize);
		y /= static_cast<int>(TileSize);
		if (x >= 0 && x < static_cast<int>(NumCols) && y >= 0 && y < static_cast<int>(NumRows))
		{
			SelectTile(y, x);
		}
	}
}

void Grid::ProcessClickUp()
{
	if (!mSelectedTiles.empty())
	{
		CheckWin();
		if (!CheckWin())
		{
			for (std::vector<Tile*>::iterator it = mSelectedTiles.begin(); it != mSelectedTiles.end(); ++it)
			{
				(*it)->ToggleSelect();
			}
			mSelectedTiles.clear();
		}
	}
}


//A* search algorithm
bool Grid::FindPath(Tile* start, Tile* goal)
{
	for (size_t i = 0; i < NumRows; ++i)
	{
		for (size_t j = 0; j < NumCols; ++j)
		{
			mTiles[i][j]->g = 0.0f;
			mTiles[i][j]->mInOpenSet = false;
			mTiles[i][j]->mInClosedSet = false;
		}
	}

	std::vector<Tile*> openSet;

	Tile* current = start;
	current->mInClosedSet = true;

	do
	{
		for (Tile* neighbour : current->mAdjacent)
		{
			if (neighbour->mBlocked)
			{
				continue;
			}

			if (!neighbour->mInClosedSet)
			{
				if (!neighbour->mInOpenSet)
				{
					neighbour->mParent = current;
					//h(x) is the euclidean distance between the current and goal node
					neighbour->h = (neighbour->GetPosition() - goal->GetPosition()).Length();
					//g(x) is the parent's g + cost of traversing the edge in the tree (actual path cost)
					neighbour->g = current->g + TileSize;
					neighbour->f = neighbour->h + neighbour->g;
					openSet.emplace_back(neighbour);
					neighbour->mInOpenSet = true;
				}
				else
				{
					//compute g(x) cost if current becomes the parent
					float newG = current->g + TileSize;
					if (newG < neighbour->g)
					{
						//adopt this node
						neighbour->mParent = current;
						neighbour->g = newG;
						neighbour->f = neighbour->g + neighbour->h;
					}
				}
			}
		}

		if (openSet.empty())
		{
			break;
		}

		auto iter = std::min_element(openSet.begin(), openSet.end(), [](Tile* a, Tile* b) { return a->f < b->f; });

		current = *iter;
		openSet.erase(iter);
		current->mInOpenSet = false;
		current->mInClosedSet = true;
	} while (current != goal);

	return (current == goal) ? true : false;
}

void Grid::UpdatePathTiles(class Tile* start)
{
	for (size_t i = 0; i < NumRows; ++i)
	{
		for (size_t j = 0; j < NumCols; ++j)
		{
			if (mTiles[i][j]->GetTileState() != Tile::EStart && mTiles[i][j]->GetTileState() != Tile::EEnd && mTiles[i][j]->GetTileState() != Tile::EPath)
			{
				mTiles[i][j]->SetTileState(Tile::EDefault);
			}
		}
	}

/*
	Tile* t = start->mParent;
	while (t != GetEndTile())
	{
		t->SetTileState(Tile::EPath);
		t = t->mParent;
	}*/
}

void Grid::ClearPath()
{
	for (size_t i = 0; i < NumRows; ++i)
	{
		for (size_t j = 0; j < NumCols; ++j)
		{
			if (mTiles[i][j]->GetTileState() == Tile::EPath)
			{
				mTiles[i][j]->SetTileState(Tile::EDefault);
			}
		}
	}
}

Tile* Grid::GetStartTile()
{
	return mTiles[mStartRow][mStartCol];
}

Tile* Grid::GetEndTile()
{
	return mTiles[mEndRow][mEndCol];
}

void Grid::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);

}

bool Grid::DetermineStartAndEndTile()
{

	int width = NumCols;
	int height = NumRows;

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

	SetStartTile(chosen_height_index_start, chosen_width_index_start);
	std::cout << chosen_height_index_start << " " << chosen_width_index_start << std::endl;
	SetEndTile(chosen_height_index_end, chosen_width_index_end);
	std::cout << chosen_height_index_end << " " << chosen_width_index_end << std::endl;
	return true;
}

std::vector<Direction> Grid::ChooseDirection(int current_height, int current_width, int end_height, int end_width)
{
	bool height_added = false, width_added = false;
	std::vector<Direction> to_return;
	std::vector<Direction> to_decide;
	int height_direction = 0, width_direction = 0;
	if (current_height - end_height < 0) 
	{
		height_direction = 1;
	}
	else if (current_height - end_height > 0) 
	{
		height_direction = -1;
	}

	if (current_width - end_width < 0) 
	{
		width_direction = 1;
	}
	else if (current_width - end_width > 0) 
	{
		width_direction = -1;
	}


	if (height_direction != 0 && width_direction != 0) 
	{

		if (height_direction == 1) 
		{
			for (int i = 0; i < PERCENTAGE2_MAIN; i++) 
			{
				to_decide.push_back(DOWN);
			}
			for (int i = 0; i < PERCENTAGE2_OTHER; i++) 
			{
				to_decide.push_back(UP);
			}
		}
		else 
		{
			for (int i = 0; i < PERCENTAGE2_MAIN; i++) 
			{
				to_decide.push_back(UP);
			}
			for (int i = 0; i < PERCENTAGE2_OTHER; i++) 
			{
				to_decide.push_back(DOWN);
			}
		}

		if (width_direction == 1) 
		{
			for (int i = 0; i < PERCENTAGE2_MAIN; i++) 
			{
				to_decide.push_back(RIGHT);
			}
			for (int i = 0; i < PERCENTAGE2_OTHER; i++) 
			{
				to_decide.push_back(LEFT);
			}
		}
		else 
		{
			for (int i = 0; i < PERCENTAGE2_MAIN; i++) 
			{
				to_decide.push_back(LEFT);
			}
			for (int i = 0; i < PERCENTAGE2_OTHER; i++) 
			{
				to_decide.push_back(RIGHT);
			}
		}
	}
	else if (height_direction != 0) 
	{

		if (height_direction == 1) 
		{
			for (int i = 0; i < PERCENTAGE1_MAIN; i++) 
			{
				to_decide.push_back(DOWN);
			}
			for (int i = 0; i < PERCENTAGE1_OTHER; i++) 
			{
				to_decide.push_back(UP);
				to_decide.push_back(LEFT);
				to_decide.push_back(RIGHT);
			}
		}
		else 
		{
			for (int i = 0; i < PERCENTAGE1_MAIN; i++) 
			{
				to_decide.push_back(UP);
			}
			for (int i = 0; i < PERCENTAGE1_OTHER; i++) 
			{
				to_decide.push_back(DOWN);
				to_decide.push_back(LEFT);
				to_decide.push_back(RIGHT);
			}
		}
	}
	else if (width_direction != 0) 
	{

		if (width_direction == 1) 
		{
			for (int i = 0; i < PERCENTAGE1_MAIN; i++) 
			{
				to_decide.push_back(RIGHT);
			}
			for (int i = 0; i < PERCENTAGE1_OTHER; i++) 
			{
				to_decide.push_back(UP);
				to_decide.push_back(LEFT);
				to_decide.push_back(DOWN);
			}
		}
		else 
		{
			for (int i = 0; i < PERCENTAGE1_MAIN; i++) 
			{
				to_decide.push_back(LEFT);
			}
			for (int i = 0; i < PERCENTAGE1_OTHER; i++)
			{
				to_decide.push_back(DOWN);
				to_decide.push_back(UP);
				to_decide.push_back(RIGHT);
			}
		}
	}
	else 
	{
		return to_return;
	}

	for (int i = 0; i < 4; i++) 
	{
		Direction temp = to_decide[rand() % to_decide.size()];
		to_return.push_back(temp);
		std::vector<Direction> aux_vec;
		for (int j = 0; j < to_decide.size(); j++) 
		{
			if (to_decide[j] != temp) 
			{
				aux_vec.push_back(to_decide[j]);
			}
		}
		to_decide = aux_vec;
	}

	return to_return;
}

int Grid::IsForbidden(std::vector<std::vector<Tile*> > &field, int row, int col)
{
	if ((field[row][col]->GetState() == Tile::EStart) || (field[row][col]->GetState() == Tile::EPath)) {
		return 1;
	}

	return 0;
}

bool Grid::DeterminePath() 
{
	std::vector<Direction> directions;
	std::vector<std::vector<Tile*> > temp_field = mTiles;
	std::vector<std::vector<Tile*> > field = mTiles;

	int start_height = 0, start_width = 0, end_height = 0, end_width = 0;
	for (std::vector<std::vector<Tile*> >::iterator it = field.begin(); it != field.end(); ++it) 
	{
		for (std::vector<Tile*>::iterator it2 = it->begin(); it2 != it->end(); ++it2) 
		{
			if ((*it2)->GetTileState() == Tile::EStart) 
			{
				start_width = std::distance(it->begin(), it2);
				start_height = std::distance(field.begin(), it);
			}
			else if ((*it2)->GetTileState() == Tile::EEnd)
			{
				end_width = std::distance(it->begin(), it2);
				end_height = std::distance(field.begin(), it);
			}
		}
	}

	int current_height = start_height;
	int current_width = start_width;

	while (1) 
	{

		std::vector<Direction> dir_vec = ChooseDirection(current_height, current_width, end_height, end_width);
		int horizontal_direction;
		int vertical_direction;
		bool success = false;

		for (int i = 0; i < 4; i++) 
		{

			if (dir_vec[i] == UP) 
			{
				horizontal_direction = 0;
				vertical_direction = -1;
			}
			else if (dir_vec[i] == DOWN) 
			{
				horizontal_direction = 0;
				vertical_direction = 1;
			}
			else if (dir_vec[i] == RIGHT) 
			{
				horizontal_direction = 1;
				vertical_direction = 0;
			}
			else if (dir_vec[i] == LEFT) 
			{
				horizontal_direction = -1;
				vertical_direction = 0;
			}
			else {
				horizontal_direction = 0;
				vertical_direction = 0;
			}

			if ((current_height + vertical_direction < 0) || (current_height + vertical_direction >= field.size())) 
			{
				continue;
			}
			if ((current_width + horizontal_direction < 0) || (current_width + horizontal_direction >= field[0].size())) 
			{
				continue;
			}
			if (IsForbidden(temp_field, current_height + vertical_direction, current_width + horizontal_direction)) 
			{
				continue;
			}

			current_height += vertical_direction;
			current_width += horizontal_direction;
			success = true;
			

			break;
		}

		if (!success) 
		{
			current_height = start_height;
			current_width = start_width;
			temp_field = field;
			directions.clear();
			std::cout << "RESETING!\n";
			continue;
		}

		if ((current_height == end_height) && (current_width == end_width)) 
		{
			break;
		}

		temp_field[current_height][current_width]->SetTileState(Tile::EPath);

		if (horizontal_direction == 1) 
		{
			directions.push_back(RIGHT);
		}
		else if (horizontal_direction == -1) 
		{
			directions.push_back(LEFT);
		}
		else if (vertical_direction == 1) 
		{
			directions.push_back(DOWN);
		}
		else if (vertical_direction == -1) 
		{
			directions.push_back(UP);
		}
	}

	for (int i = 0; i < directions.size(); i++) 
	{
		if (directions[i] == UP) 
		{
			std::cout << "UP, ";
		}
		else if (directions[i] == DOWN) 
		{
			std::cout << "DOWN, ";
		}
		else if (directions[i] == LEFT) 
		{
			std::cout << "LEFT, ";
		}
		else if (directions[i] == RIGHT) 
		{
			std::cout << "RIGHT, ";
		}
	}

	mTiles = temp_field;

	return true;
}

bool Grid::PlaceWalls()
{
	int free_spaces = 0;
	std::vector<std::pair<int, int> > free_spaces_vec;
	std::vector<std::vector<Tile*> > field = mTiles;

	for (std::vector<std::vector<Tile*> >::iterator it = field.begin(); it != field.end(); ++it) 
	{
		for (std::vector<Tile*>::iterator it2 = it->begin(); it2 != it->end(); ++it2) 
		{
			if ((*it2)->mBlocked == false && (*it2)->GetTileState() == Tile::EDefault)
			{
				free_spaces++;
				free_spaces_vec.push_back(std::make_pair(std::distance(field.begin(), it), std::distance(it->begin(), it2)));
			}
		}
	}

	int temp_size = free_spaces_vec.size();
	for (int i = 0; i < (int)(free_spaces * WALLS_PERCENTAGE); i++) 
	{
		int index = rand() % temp_size;
		std::pair<int, int> temp_pair = free_spaces_vec[index];
		temp_size--;
		mTiles[temp_pair.first][temp_pair.second]->mBlocked = true;
		free_spaces_vec.erase(free_spaces_vec.begin() + index);
	}

	return true;
}

bool Grid::CheckWin()
{
	if (mSelectedTiles.front()->GetTileState() == Tile::EStart && mSelectedTiles.back()->GetTileState() == Tile::EEnd || mSelectedTiles.front()->GetTileState() == Tile::EEnd && mSelectedTiles.back()->GetTileState() == Tile::EStart)
	{
		std::vector<Tile*>::iterator it;
		for (it = mSelectedTiles.begin(); it != mSelectedTiles.end(); ++it)
		{
			if ((*it)->mBlocked)
			{
				std::cout << "LOSE" << std::endl;
				break;
			}
		}

		if (it == mSelectedTiles.end())
		{
			std::cout << "WIN" << std::endl;
		}
		if (GetGame()->GetState() != Game::EOver)
		{
			GetGame()->SetState(Game::EOver);
		}
		return true;
	}
	return false;
}