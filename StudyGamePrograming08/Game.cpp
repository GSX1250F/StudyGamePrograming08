#include "Game.h"
#include <glew.h>
#include "Texture.h"
#include "VertexArray.h"
#include "Shader.h"
#include <algorithm>
#include "Actor.h"
#include "SpriteComponent.h"
#include "Ship.h"
#include "Asteroid.h"
#include "BackGround.h"
#include "ClearPict.h"
#include "Random.h"
#include "InputSystem.h"
#include <thread>
#include <chrono>
//#include <SDL.h>
//#include <SDL_image.h>

Game::Game()
	:mWindow(nullptr),
//	mRenderer(nullptr),
	mSpriteShader(nullptr),
	mIsRunning(true),
	mUpdatingActors(false),
	mWindowWidth(1024),
	mWindowHeight(768)
{
}

bool Game::Initialize()
{
	// SDL初期化
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER) != 0)
	{
		SDL_Log("SDLを初期化できません: %s", SDL_GetError());
		return false;
	}

	// コアOpenGLプロファイルを使う
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
						SDL_GL_CONTEXT_PROFILE_CORE);
	// バージョン3.3を指定
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	//RGBA各チャネル8ビットのカラーバッファを使う
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	//ダブルバッファを有効にする
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	//ハードウェアアクセラレーションを使う
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	// SDLウィンドウを作成
	mWindow = SDL_CreateWindow(
		"Game Programming in C++ (Chapter 8)", 
		100, 
		100, 
		mWindowWidth, 
		mWindowHeight, 
		SDL_WINDOW_OPENGL
	);
	if (!mWindow)
	{
		SDL_Log("ウィンドウの作成に失敗しました: %s", SDL_GetError());
		return false;
	}
	//OpenGLコンテクストの作成
	mContext = SDL_GL_CreateContext(mWindow);
	//GLEWを初期化
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		SDL_Log("GLEWの初期化に失敗しました。");
		return false;
	}
	//一部のプラットフォームでは、GLEWが無害なエラーコードを出すので、それをクリアする。
	glGetError();

	// シェーダーを作成およびコンパイル可能かを確認
	if (!LoadShaders())
	{
		SDL_Log("シェーダー読み込みに失敗");
		return false;
	}

	// スプライト描画用の配列を作成
	CreateSpriteVerts();

	Random::Init();		//乱数設定の初期化?

	// InputSystemを生成・初期化
	mInputSystem = new InputSystem();
	if (!mInputSystem->Initialize())
	{
		SDL_Log("Input Systemの初期化に失敗しました。");
		return false;
	}


	LoadData();

	mTicksCount = SDL_GetTicks();

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
	mInputSystem->PrepareForUpdate();

	SDL_Event event;
	// キューにイベントがあれば繰り返す
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				mIsRunning = false;
				break;
			case SDL_MOUSEWHEEL:
				mInputSystem->ProcessEvent(event);
				break;
			default:
				break;
		}
	}

	mInputSystem->Update();
	const InputState& state = mInputSystem->GetState();

	//const Uint8* keyState = SDL_GetKeyboardState(NULL);
	// ESCキーが押されて、"放されたら"、ゲーム終了。
	if (state.Keyboard.GetKeyState(SDL_SCANCODE_ESCAPE) == EReleased)
	{
		mIsRunning = false;
	}

	mUpdatingActors = true;
	for (auto actor : mActors)
	{
		actor->ProcessInput(state);
	}
	mUpdatingActors = false;
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
	if (deltaTime > 0.05f) { deltaTime = 0.05f; }			// デルタタイムを最大値で制限する
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
	//クリアカラーを灰色に設定
	glClearColor(0.86f, 0.86f, 0.86f, 1.0f);
	
	//カラーバッファをクリア
	glClear(GL_COLOR_BUFFER_BIT);
	
	// すべてのスプライトコンポーネントを描画
	// カラーバッファのαブレンディングを有効化
	glEnable(GL_BLEND);
	glBlendFunc(
		GL_SRC_ALPHA,				// srcFactorはsrcAlpha
		GL_ONE_MINUS_SRC_ALPHA		// dstFactorは(1-srcAlpha)
	);
	
	// スプライトのシェーダーと頂点配列オブジェクトをアクティブ化
	mSpriteShader->SetActive();
	mSpriteVerts->SetActive();

	// すべてのスプライトを描画
	for (auto sprite : mSprites)
	{
		sprite->Draw(mSpriteShader);
	}

	//バッファを交換
	SDL_GL_SwapWindow(mWindow);
	
}

void Game::LoadData()
{
	//プレイヤーの宇宙船を作成
	mShip = new Ship(this);

	// 小惑星を最初に複数生成
	int initialNumAsteroids = 15;		//初期値
	numAsteroids = 0;
	for (int i = 0; i < initialNumAsteroids; i++)
	{
		IncreaseAsteroid();
	}

	//背景を作成
	new BackGround(this, 0, -10.0f, 5,"Assets/Farback01.png");
	new BackGround(this, 1, -10.0f, 5, "Assets/Farback02.png");
	new BackGround(this, 0, -20.0f, 15, "Assets/Stars.png");
	new BackGround(this, 1, -20.0f, 15, "Assets/Stars.png");
	
	mClearPict = new ClearPict(this);
}

void Game::UnloadData()
{
	// アクターを消去
	while (!mActors.empty())
	{
		delete mActors.back();
	}

	// テクスチャを消去
	for (auto i : mTextures)
	{
		i.second->Unload();
		delete i.second;
	}
	mTextures.clear();
}

bool Game::LoadShaders()
{
	mSpriteShader = new Shader();
	if (!mSpriteShader->Load("Shaders/Sprite.vert", "Shaders/Sprite.frag"))
	{
		return false;
	}
	mSpriteShader->SetActive();
	// ビュー射影行列を設定
	Matrix4 viewProj = Matrix4::CreateSimpleViewProj(static_cast<float>(mWindowWidth), static_cast<float>(mWindowHeight));
	mSpriteShader->SetMatrixUniform("uViewProj", viewProj);
	return true;

}

void Game::CreateSpriteVerts()
{
	// 短形の頂点バッファ　　頂点座標(Vector3), テクスチャ座標(Vector2), 頂点カラー(RGBA)
	// 頂点座標は、中央が原点、右・奥・上方向に+x,+y,+z方向
	// テクスチャ座標は、左上が原点
	float vertices[] = {
		-0.5f,  0.5f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f,			// 左上  
		 0.5f,  0.5f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 1.f,			// 右上
		 0.5f, -0.5f, 0.f, 1.f, 1.f, 0.f, 0.f, 1.f, 1.f,			// 右下
		-0.5f, -0.5f, 0.f, 0.f, 1.f, 0.5f, 0.5f, 0.5f, 1.f			// 左下
	};
	// インデックスバッファ
	// 頂点バッファの順番にID 0,1,2,3が割り当てられており、ポリゴンの頂点IDを３つ指定する。
	unsigned int indices[] = {
		0, 1, 2,		// 左上 , 右上 , 右下
		2, 3, 0			// 右下 , 左下 , 左上
	};

	// 頂点バッファの数＝4 ,  インデックスバッファの数=6
	mSpriteVerts = new VertexArray(vertices, 4, indices, 6);
}

Texture* Game::GetTexture(const std::string& fileName)
{
	Texture* tex = nullptr;
	auto iter = mTextures.find(fileName);
	if (iter != mTextures.end())
	{
		tex = iter->second;
	}
	else
	{
		tex = new Texture();
		if (tex->Load(fileName))
		{
			mTextures.emplace(fileName, tex);
		}
		else
		{
			delete tex;
			tex = nullptr;
		}
	}
	return tex;
}

void Game::IncreaseAsteroid()
{
	new Asteroid(this);
	numAsteroids++;
}

void Game::AddAsteroid(Asteroid* ast)
{
	mAsteroids.emplace_back(ast);
}

void Game::RemoveAsteroid(Asteroid* ast)
{
	auto iter = std::find(mAsteroids.begin(), mAsteroids.end(), ast);
	if (iter != mAsteroids.end())
	{
		mAsteroids.erase(iter);
	}
	numAsteroids--;
}

void Game::AddBackGround(BackGround* bg)
{
	mBackGrounds.emplace_back(bg);
}


void Game::Shutdown()
{
	UnloadData();

	mInputSystem->Shutdown();
	delete mInputSystem;

	//IMG_Quit();
	delete mSpriteVerts;
	mSpriteShader->Unload();
	delete mSpriteShader;
	//SDL_DestroyRenderer(mRenderer);
	SDL_GL_DeleteContext(mContext);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
	
}

void Game::AddActor(Actor* actor)
{
	// If we're updating actors, need to add to pending
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
	// Is it in pending actors?
	auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
	if (iter != mPendingActors.end())
	{
		// Swap to end of vector and pop off (avoid erase copies)
		std::iter_swap(iter, mPendingActors.end() - 1);
		mPendingActors.pop_back();
	}

	// Is it in actors?
	iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end())
	{
		// Swap to end of vector and pop off (avoid erase copies)
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}
}

void Game::AddSprite(SpriteComponent* sprite)
{
	// Find the insertion point in the sorted vector
	// (The first element with a higher draw order than me)
	int myDrawOrder = sprite->GetDrawOrder();
	auto iter = mSprites.begin();
	for (;
		iter != mSprites.end();
		++iter)
	{
		if (myDrawOrder < (*iter)->GetDrawOrder())
		{
			break;
		}
	}

	// Inserts element before position of iterator
	mSprites.insert(iter, sprite);
}

void Game::RemoveSprite(SpriteComponent* sprite)
{
	// (We can't swap because it ruins ordering)
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	mSprites.erase(iter);
}

