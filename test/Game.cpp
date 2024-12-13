#include "Game.h"
#include <SDL.h>
#include <sstream>
#include <thread>
#include <chrono>

Game::Game()
	: mIsRunning(true) 
{}
bool Game::Initialize()
{
	// SDL初期化
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		SDL_Log("SDLを初期化できません: %s", SDL_GetError());
		return false;
	}
	
	LoadData();

	mTicksCount = SDL_GetTicks();	//TicksCount初期化

	return true;
}
void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}
void Game::ProcessInput()
{
	SDL_Event event;
	SDL_Event keyEvent;
	bool keyFlag = false;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			mIsRunning = false;
		}
		if (event.type == SDL_KEYDOWN)
		{
			if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				mIsRunning = false;
			}
			else
			{
				keyEvent = event;
				keyFlag = true;
			}
		}
		if (!keyFlag) { keyEvent = event; }
	}

}

void Game::UpdateGame()
{
	// フレームレート調整（62.5fps)
	if (SDL_GetTicks() - mTicksCount < 100) {
		int sleep = 100 - (SDL_GetTicks() - mTicksCount);
		std::this_thread::sleep_for(std::chrono::milliseconds(sleep));    // sleepミリ秒処理を止める
	}

	//while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));	// 前のフレームから 16ms 経つまで待つ.※sleepしないのでCPU使用率が上がる。
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;		// デルタタイムの計算
	if (deltaTime > 0.05f) { deltaTime = 0.05f; }			// デルタタイムを最大値で制限する
	mTicksCount = SDL_GetTicks();

}

void Game::GenerateOutput() {}

void Game::Shutdown()
{
	UnloadData();
	SDL_Quit();
}

void Game::LoadData() {}

void Game::UnloadData() {}

