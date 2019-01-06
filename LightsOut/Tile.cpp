#include "pch.h"
#include"Tile.h"
#include"Game.h"
#include"SpriteComponent.h"
#include<iostream>
Tile::Tile(Game* game) : Actor(game), mParent(nullptr), f(0.0f), g(0.0f), h(0.0f), mBlocked(false), mSprite(nullptr), mTileState(EDefault), mSelected(false), mHidden(false)
{
	mSprite = new SpriteComponent(this);
	UpdateTexture();
}

void Tile::SetTileState(TileState state)
{
	mTileState = state;
	UpdateTexture();
}

void Tile::ToggleSelect()
{
	mSelected = !mSelected;
	UpdateTexture();
}

void Tile::ToggleHidden()
{
	mHidden = !mHidden;
	UpdateTexture();
}

void Tile::UpdateTexture()
{
	std::string text;
	switch (mTileState)
	{
	case EStart:
		if (mSelected)
		{
			text = "Assets/TileStartSelected.png";
		}
		else
		{
			text = "Assets/TileGreyStart.png";
		}
		break;
	case EEnd:
		if (mSelected)
		{
			text = "Assets/TileStartSelected.png";
		}
		else
		{
			text = "Assets/TileGreyStart.png";
		}
		break;
	case EPath:
		if (mSelected)
		{
			text = "Assets/TileGreenSelected.png";
		}
		else if (mHidden)
		{
			text = "Assets/TileGreyDefault.png";
		}
		else
		{
			text = "Assets/TileTanSelected.png";
		}
		break;
	case EDefault:
	default:
		if (mSelected && mBlocked && !mHidden)
		{
			text = "Assets/TileWallSelected.png";
		}
		else if (mSelected)
		{
			text = "Assets/TileGreenSelected.png";
		}
		else if (mHidden)
		{
			text = "Assets/TileGreyDefault.png";
		}
		else if (mBlocked)
		{
			text = "Assets/TileRedSelected.png";
		}
		else
		{
			text = "Assets/TileGreyDefault.png";
		}
		break;
	}

	mSprite->SetTexture(GetGame()->GetTexture(text));
}