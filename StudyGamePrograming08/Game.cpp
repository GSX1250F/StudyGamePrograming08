#include "Game.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <algorithm>
#include "Actor.h"
#include "Renderer.h"
#include "SoundPlayer.h"
#include "InputSystem.h"
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
	,mWindowWidth(1024)
	,mWindowHeight(768)
{}

bool Game::Initialize()
{
	// SDL初期化
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		SDL_Log("SDLを初期化できません: %s", SDL_GetError());
		return false;
	}
	// レンダラー作成
	mRenderer = new Renderer(this);
	if (!mRenderer->Initialize(mWindowWidth, mWindowHeight))
	{
		SDL_Log("レンダラーの初期化に失敗しました");
		delete mRenderer;
		mRenderer = nullptr;
		return false;
	}
	// サウンドプレイヤ作成
	mSoundPlayer = new SoundPlayer(this);
	if (!mSoundPlayer->Initialize())
	{
		SDL_Log("サウンドプレイヤの初期化に失敗しました");
		delete mSoundPlayer;
		mSoundPlayer = nullptr;
		return false;
	}

	
		
	Random::Init();		//乱数設定の初期化?

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
	const Uint8* keyState = NULL;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			mIsRunning = false;
		}
		keyState = SDL_GetKeyboardState(NULL);
		if (keyState[SDL_SCANCODE_ESCAPE])
		{
			mIsRunning = false;
		}		
	}
	if (keyState) 
	{
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
	// フレームレート調整（62.5fps)
	if (SDL_GetTicks() - mTicksCount < 16) {
		int sleep = 16 - (SDL_GetTicks() - mTicksCount);
		std::this_thread::sleep_for(std::chrono::milliseconds(sleep));    // sleepミリ秒処理を止める
	}
	
	//while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));	// 前のフレームから 16ms 経つまで待つ.※sleepしないのでCPU使用率が上がる。
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;		// デルタタイムの計算
	if (deltaTime > 0.05f){	deltaTime = 0.05f;}			// デルタタイムを最大値で制限する
	mTicksCount = SDL_GetTicks();

	// すべてのアクターを更新
	mUpdatingActors = true;
	for (auto actor : mActors)
	{
		actor->Update(deltaTime);
	}
	mUpdatingActors = false;

	// 待ちアクターをmActorsに移動
	for (auto pending : mPendingActors)
	{
		pending->ComputeWorldTransform();
		mActors.emplace_back(pending);
	}
	mPendingActors.clear();

	// 死んだアクターを一時配列に追加
	std::vector<Actor*> deadActors;
	for (auto actor : mActors)
	{
		if (actor->GetState() == Actor::EDead)
		{
			deadActors.emplace_back(actor);
		}
	}

	// 死んだアクターを削除
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
	//プレイヤーの宇宙船を作成
	mShip = new Ship(this);
	
	// 小惑星を最初に複数生成
	int initialNumAsteroids = 1;		//初期値
	for (int i = 0; i < initialNumAsteroids; i++)
	{
		AddAsteroid();
	}

	// 小惑星を一定時間ごとに追加する
	new AsteroidControl(this);

	// ゲームクリアー画面
	new ClearPict(this);

	// 背景
	new BackGround(this);	
}

void Game::UnloadData()
{
	// アクターを配列から消去
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
	// アクターが更新中は、待ちアクターに追加する
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
	// 消去したいアクターが待ちアクターにある場合
	auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
	if (iter != mPendingActors.end())
	{
		// 配列の一番最後と交換し、消去する
		std::iter_swap(iter, mPendingActors.end() - 1);
		mPendingActors.pop_back();
	}

	// 消去したいアクターがアクターにある場合
	iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end())
	{
		// 配列の一番最後と交換し、消去する
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}
}


//Game Specific
void Game::AddAsteroid()
{
	Asteroid* ast = new Asteroid(this);	
	mAsteroids.emplace_back(ast);	//小惑星が増加するときは配列の一番最後に追加する。
}

void Game::RemoveAsteroid(Asteroid* ast)
{
	auto iter = std::find(mAsteroids.begin(), mAsteroids.end(), ast);
	if (iter != mAsteroids.end())
	{
		mAsteroids.erase(iter);
	}
}