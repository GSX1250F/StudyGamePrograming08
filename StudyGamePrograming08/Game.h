#pragma once
#include <SDL.h>
#include <unordered_map>
#include <string>
#include <vector>
#include "Math.h"

class Game
{
public:
	Game();
	bool Initialize();
	void RunLoop();
	void Shutdown();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);

	class Texture* GetTexture(const std::string& fileName);

	void SetRunning(bool isrunning) { mIsRunning = isrunning; }

	//ゲームウィンドウの大きさ
	int mWindowWidth;
	int mWindowHeight;

	//Game-specific	
	void AddAsteroid(class Asteroid* ast);
	void RemoveAsteroid(class Asteroid* ast);
	std::vector<class Asteroid*>& GetAsteroids() { return mAsteroids; }
	void AddBackGround(class BackGround* bg);
	void IncreaseAsteroid();
	int numAsteroids;
	class ClearPict* mClearPict;	//ゲームクリア画像

private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	bool LoadShaders();
	void CreateSpriteVerts();
	void LoadData();
	void UnloadData();

	// テクスチャの配列
	std::unordered_map<std::string, class Texture*> mTextures;

	// すべてのアクター
	std::vector<class Actor*> mActors;
	// すべての待ちアクター
	std::vector<class Actor*> mPendingActors;

	// すべての描画されるスプライトコンポーネント
	std::vector<class SpriteComponent*> mSprites;

	// スプライトシェーダー
	class Shader* mSpriteShader;

	// スプライト頂点配列
	class VertexArray* mSpriteVerts;

	SDL_Window* mWindow;
	SDL_GLContext mContext;
	//SDL_Renderer* mRenderer;
	Uint32 mTicksCount;
	bool mIsRunning;
	// アクターが更新中か
	bool mUpdatingActors;

	// InputSystemへのポインタ
	class InputSystem* mInputSystem;

	//Game-specific
	class Ship* mShip; // player ship
	std::vector<class Asteroid*> mAsteroids; //asteroids
	std::vector<class BackGround*> mBackGrounds;  //背景
	
	
};