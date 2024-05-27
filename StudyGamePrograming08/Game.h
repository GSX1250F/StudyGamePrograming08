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

	//�Q�[���E�B���h�E�̑傫��
	int mWindowWidth;
	int mWindowHeight;

	//Game-specific	
	void AddAsteroid(class Asteroid* ast);
	void RemoveAsteroid(class Asteroid* ast);
	std::vector<class Asteroid*>& GetAsteroids() { return mAsteroids; }
	void AddBackGround(class BackGround* bg);
	void IncreaseAsteroid();
	int numAsteroids;
	class ClearPict* mClearPict;	//�Q�[���N���A�摜

private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	bool LoadShaders();
	void CreateSpriteVerts();
	void LoadData();
	void UnloadData();

	// �e�N�X�`���̔z��
	std::unordered_map<std::string, class Texture*> mTextures;

	// ���ׂẴA�N�^�[
	std::vector<class Actor*> mActors;
	// ���ׂĂ̑҂��A�N�^�[
	std::vector<class Actor*> mPendingActors;

	// ���ׂĂ̕`�悳���X�v���C�g�R���|�[�l���g
	std::vector<class SpriteComponent*> mSprites;

	// �X�v���C�g�V�F�[�_�[
	class Shader* mSpriteShader;

	// �X�v���C�g���_�z��
	class VertexArray* mSpriteVerts;

	SDL_Window* mWindow;
	SDL_GLContext mContext;
	//SDL_Renderer* mRenderer;
	Uint32 mTicksCount;
	bool mIsRunning;
	// �A�N�^�[���X�V����
	bool mUpdatingActors;

	// InputSystem�ւ̃|�C���^
	class InputSystem* mInputSystem;

	//Game-specific
	class Ship* mShip; // player ship
	std::vector<class Asteroid*> mAsteroids; //asteroids
	std::vector<class BackGround*> mBackGrounds;  //�w�i
	
	
};