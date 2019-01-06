#pragma once

#include "Actor.h"
#include <vector>

class Tile : public Actor
{
public:
	friend class Grid;

	enum TileState
	{
		EDefault,
		EPath,
		EStart,
		EEnd
	};

	Tile(class Game* game);
	void SetTileState(TileState state);
	TileState GetTileState() const { return mTileState; }
	void ToggleSelect();
	void ToggleHidden();
	const Tile* GetParent() const { return mParent; }

private:
	TileState mTileState;
	void UpdateTexture();
	class SpriteComponent* mSprite;
	bool mSelected;
	bool mHidden;

	Tile* mParent;
	std::vector<Tile*> mAdjacent;
	float f;
	float g;
	float h;
	bool mInOpenSet;
	bool mInClosedSet;
	bool mBlocked;
};