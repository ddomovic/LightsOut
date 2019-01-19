#pragma once
#include <SDL.h>
#include <unordered_map>
#include <string>
#include <vector>
#include "Math.h"
#include<iostream>
class Game
{
public:

	enum State
	{
		EVisible,
		EHidden,
		ELose,
		EWin
	};

	enum Difficulty
	{
		EEasy,
		ENormal,
		EHard
	};

	Game(Difficulty difficulty);
	bool Initialize();
	void RunLoop();
	void Shutdown();

	void LoadLevel();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);

	State GetState() const { return mState; }
	void SetState(State state);

	SDL_Texture* GetTexture(const std::string& fileName);
	SDL_Texture* GetTextTexture(const std::string text);

	class Grid* GetGrid() { return mGrid; }

	void SetPoints(double points) { mPoints = points; std::cout << "\nPOINTS: " << mPoints << std::endl; }
	double GetPoints() { return mPoints; }

	int GetLevel() { return mLevel; }
	int GetDifficulty() { return mDifficulty; }

private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();

	std::unordered_map<std::string, SDL_Texture*> mTextures;

	std::vector<class Actor*> mActors;
	std::vector<class Actor*> mPendingActors;

	std::vector<class SpriteComponent*> mSprites;

	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	Uint32 mTicksCount;
	bool mIsRunning;
	bool mUpdatingActors;

	class Grid* mGrid;
	State mState;

	double mPoints;
	class TextSpriteComponent* mTextPoints;

	int mLevel;
	int mEndLevel;

	Difficulty mDifficulty;
};