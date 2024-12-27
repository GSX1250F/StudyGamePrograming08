#pragma once
#include <SDL.h>
#include <string>
#include <vector>

class Game
{
public:
	Game();
	bool Initialize();
	void RunLoop();
	void Shutdown();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	class Renderer* GetRenderer() { return mRenderer; }
	class SoundPlayer* GetSoundPlayer() { return mSoundPlayer; }

	void SetRunning(bool isrunning) { mIsRunning = isrunning; }

	//�Q�[���E�B���h�E�̑傫��
	int mWindowWidth;
	int mWindowHeight;

	//Game-specific	
	void AddAsteroid();
	void RemoveAsteroid(class Asteroid* ast);
	std::vector<class Asteroid*>& GetAsteroids() { return mAsteroids; }
	class Ship* GetShip() { return mShip; }
	
private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();

	std::vector<class Actor*> mActors;
	std::vector<class Actor*> mPendingActors;

	class Renderer* mRenderer;
	class SoundPlayer* mSoundPlayer;
	Uint32 mTicksCount;
	bool mIsRunning;
	bool mUpdatingActors;


	//Game-specific
	class Ship* mShip; // �v���C���[�̉F���D
	std::vector<class Asteroid*> mAsteroids; //���f��
};