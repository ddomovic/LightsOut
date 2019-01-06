#pragma once
#include <SDL.h>
#include <unordered_map>
#include <string>
#include <vector>
#include "Math.h"

class Game
{
public:

	enum State
	{
		EVisible,
		EHidden,
		EOver
	};

	Game();
	bool Initialize();
	void RunLoop();
	void Shutdown();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);

	State GetState() const { return mState; }
	void SetState(State state);

	SDL_Texture* GetTexture(const std::string& fileName);

	class Grid* GetGrid() { return mGrid; }

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
};