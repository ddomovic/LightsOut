#pragma once
#include "Actor.h"
#include <vector>

#define WALLS_PERCENTAGE 0.7
#define PERCENTAGE2_MAIN 40
#define PERCENTAGE2_OTHER 10
#define PERCENTAGE1_MAIN 70
#define PERCENTAGE1_OTHER 10

class Grid : public Actor
{
public:
	Grid(class Game* game, const size_t rows, const size_t cols);
	void ProcessClickDown(int x, int y);
	void ProcessClickUp();
	bool FindPath(class Tile* start, class Tile* goal);
	bool PlaceWalls();
	bool DeterminePath();
	void ClearPath();
	std::vector<enum Direction> ChooseDirection(int currentHeight, int currentWidth, int endHeight, int endWidth);
	int IsForbidden(std::vector<std::vector<class Tile*>> &field, int row, int col);
	void SetHidden();
	bool CheckWin();

	class Tile* GetStartTile();
	class Tile* GetEndTile();
	void SetStartTile(size_t row, size_t col) { mStartRow = row; mStartCol = col; }
	void SetEndTile(size_t row, size_t col) { mEndRow = row; mEndCol = col; }

	void UpdateActor(float deltaTime) override;

	bool DetermineStartAndEndTile();

private:
	void SelectTile(size_t row, size_t col);
	void UpdatePathTiles(class Tile* start);

	std::vector<class Tile*> mSelectedTiles;
	std::vector<std::vector<class Tile*>> mTiles;
	const size_t NumRows;
	const size_t NumCols;
	const float StartY = 192.0f;
	const float StartX = 288.0f;
	const float TileSize = 64.0f;

	size_t mStartRow;
	size_t mStartCol;
	size_t mEndRow;
	size_t mEndCol;

};