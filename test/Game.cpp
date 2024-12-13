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
	// SDL������
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		SDL_Log("SDL���������ł��܂���: %s", SDL_GetError());
		return false;
	}
	
	LoadData();

	mTicksCount = SDL_GetTicks();	//TicksCount������

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
	// �t���[�����[�g�����i62.5fps)
	if (SDL_GetTicks() - mTicksCount < 100) {
		int sleep = 100 - (SDL_GetTicks() - mTicksCount);
		std::this_thread::sleep_for(std::chrono::milliseconds(sleep));    // sleep�~���b�������~�߂�
	}

	//while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));	// �O�̃t���[������ 16ms �o�܂ő҂�.��sleep���Ȃ��̂�CPU�g�p�����オ��B
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;		// �f���^�^�C���̌v�Z
	if (deltaTime > 0.05f) { deltaTime = 0.05f; }			// �f���^�^�C�����ő�l�Ő�������
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

