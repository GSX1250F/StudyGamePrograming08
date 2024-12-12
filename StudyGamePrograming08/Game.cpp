#include "Game.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <algorithm>
#include "Actor.h"
#include "Renderer.h"
#include "SoundPlayer.h"
#include "SpriteComponent.h"
#include "Ship.h"
#include "Asteroid.h"
#include "AsteroidControl.h"
#include "BackGround.h"
#include "ClearPict.h"
#include "Random.h"
#include <thread>
#include <chrono>

Game::Game()
	:mRenderer(nullptr)
	,mSoundPlayer(nullptr)
	,mIsRunning(true)
	,mUpdatingActors(false)
	,mTicksCount(0)
	,mWindowWidth(1024)
	,mWindowHeight(768)
{}

bool Game::Initialize()
{
	// SDL������
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		SDL_Log("SDL���������ł��܂���: %s", SDL_GetError());
		return false;
	}
	// �����_���[�쐬
	mRenderer = new Renderer(this);
	if (!mRenderer->Initialize(mWindowWidth, mWindowHeight))
	{
		SDL_Log("�����_���[�̏������Ɏ��s���܂���");
		delete mRenderer;
		mRenderer = nullptr;
		return false;
	}
	// �T�E���h�v���C���쐬
	mSoundPlayer = new SoundPlayer(this);
	if (!mSoundPlayer->Initialize())
	{
		SDL_Log("�T�E���h�v���C���̏������Ɏ��s���܂���");
		delete mSoundPlayer;
		mSoundPlayer = nullptr;
		return false;
	}
		
	Random::Init();		//�����ݒ�̏�����?

	LoadData();

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
	// �L���[�ɃC�x���g������ΌJ��Ԃ�
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			mIsRunning = false;
		}
		const Uint8* keyState = SDL_GetKeyboardState(NULL);
		if (keyState[SDL_SCANCODE_ESCAPE])
		{
			mIsRunning = false;
		}

		mUpdatingActors = true;
		for (auto actor : mActors)
		{
			actor->ProcessInput(keyState);
		}
		mUpdatingActors = false;

	}	
}

void Game::UpdateGame()
{
	// �t���[�����[�g�����i62.5fps)
	if (SDL_GetTicks() - mTicksCount < 16) {
		int sleep = 16 - (SDL_GetTicks() - mTicksCount);
		std::this_thread::sleep_for(std::chrono::milliseconds(sleep));    // sleep�~���b�������~�߂�
	}
	
	//while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));	// �O�̃t���[������ 16ms �o�܂ő҂�.��sleep���Ȃ��̂�CPU�g�p�����オ��B
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;		// �f���^�^�C���̌v�Z
	if (deltaTime > 0.05f){	deltaTime = 0.05f;}			// �f���^�^�C�����ő�l�Ő�������
	mTicksCount = SDL_GetTicks();

	// ���ׂẴA�N�^�[���X�V
	mUpdatingActors = true;
	for (auto actor : mActors)
	{
		actor->Update(deltaTime);
	}
	mUpdatingActors = false;

	// �҂��A�N�^�[��mActors�Ɉړ�
	for (auto pending : mPendingActors)
	{
		pending->ComputeWorldTransform();
		mActors.emplace_back(pending);
	}
	mPendingActors.clear();

	// ���񂾃A�N�^�[���ꎞ�z��ɒǉ�
	std::vector<Actor*> deadActors;
	for (auto actor : mActors)
	{
		if (actor->GetState() == Actor::EDead)
		{
			deadActors.emplace_back(actor);
		}
	}

	// ���񂾃A�N�^�[���폜
	for (auto actor : deadActors)
	{
		delete actor;
	}
}

void Game::GenerateOutput()
{
	mRenderer->Draw();
	mSoundPlayer->Play();
}

void Game::Shutdown()
{
	UnloadData();
	if (mRenderer)
	{
		mRenderer->Shutdown();
	}
	if (mSoundPlayer)
	{
		mSoundPlayer->Shutdown();
	}
	SDL_Quit();
}

void Game::LoadData()
{
	//�v���C���[�̉F���D���쐬
	mShip = new Ship(this);
	
	// ���f�����ŏ��ɕ�������
	int initialNumAsteroids = 20;		//�����l
	for (int i = 0; i < initialNumAsteroids; i++)
	{
		AddAsteroid();
	}

	// ���f������莞�Ԃ��Ƃɒǉ�����
	new AsteroidControl(this);

	// �Q�[���N���A�[���
	new ClearPict(this);

	// �w�i
	new BackGround(this);	
}

void Game::UnloadData()
{
	// �A�N�^�[��z�񂩂����
	while (!mActors.empty())
	{
		delete mActors.back();
	}

	if (mRenderer)
	{
		mRenderer->UnloadData();
	}

	if (mSoundPlayer)
	{
		mSoundPlayer->UnloadData();
	}

}

void Game::AddActor(Actor* actor)
{
	// �A�N�^�[���X�V���́A�҂��A�N�^�[�ɒǉ�����
	if (mUpdatingActors)
	{
		mPendingActors.emplace_back(actor);
	}
	else
	{
		mActors.emplace_back(actor);
	}
}

void Game::RemoveActor(Actor* actor)
{
	// �����������A�N�^�[���҂��A�N�^�[�ɂ���ꍇ
	auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
	if (iter != mPendingActors.end())
	{
		// �z��̈�ԍŌ�ƌ������A��������
		std::iter_swap(iter, mPendingActors.end() - 1);
		mPendingActors.pop_back();
	}

	// �����������A�N�^�[���A�N�^�[�ɂ���ꍇ
	iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end())
	{
		// �z��̈�ԍŌ�ƌ������A��������
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}
}


//Game Specific
void Game::AddAsteroid()
{
	Asteroid* ast = new Asteroid(this);	
	mAsteroids.emplace_back(ast);	//���f������������Ƃ��͔z��̈�ԍŌ�ɒǉ�����B
}

void Game::RemoveAsteroid(Asteroid* ast)
{
	auto iter = std::find(mAsteroids.begin(), mAsteroids.end(), ast);
	if (iter != mAsteroids.end())
	{
		mAsteroids.erase(iter);
	}
}