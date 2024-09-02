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
	// SDL������
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER) != 0)
	{
		SDL_Log("SDL���������ł��܂���: %s", SDL_GetError());
		return false;
	}

	// �R�AOpenGL�v���t�@�C�����g��
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
						SDL_GL_CONTEXT_PROFILE_CORE);
	// �o�[�W����3.3���w��
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	//RGBA�e�`���l��8�r�b�g�̃J���[�o�b�t�@���g��
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	//�_�u���o�b�t�@��L���ɂ���
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	//�n�[�h�E�F�A�A�N�Z�����[�V�������g��
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	// SDL�E�B���h�E���쐬
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
		SDL_Log("�E�B���h�E�̍쐬�Ɏ��s���܂���: %s", SDL_GetError());
		return false;
	}
	//OpenGL�R���e�N�X�g�̍쐬
	mContext = SDL_GL_CreateContext(mWindow);
	//GLEW��������
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		SDL_Log("GLEW�̏������Ɏ��s���܂����B");
		return false;
	}
	//�ꕔ�̃v���b�g�t�H�[���ł́AGLEW�����Q�ȃG���[�R�[�h���o���̂ŁA������N���A����B
	glGetError();

	// �V�F�[�_�[���쐬����уR���p�C���\�����m�F
	if (!LoadShaders())
	{
		SDL_Log("�V�F�[�_�[�ǂݍ��݂Ɏ��s");
		return false;
	}

	// �X�v���C�g�`��p�̔z����쐬
	CreateSpriteVerts();

	Random::Init();		//�����ݒ�̏�����?

	// InputSystem�𐶐��E������
	mInputSystem = new InputSystem();
	if (!mInputSystem->Initialize())
	{
		SDL_Log("Input System�̏������Ɏ��s���܂����B");
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
	// �L���[�ɃC�x���g������ΌJ��Ԃ�
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
	// ESC�L�[��������āA"�����ꂽ��"�A�Q�[���I���B
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
	// �t���[�����[�g�����i62.5fps)
	if (SDL_GetTicks() - mTicksCount < 16) {
		int sleep = 16 - (SDL_GetTicks() - mTicksCount);
		std::this_thread::sleep_for(std::chrono::milliseconds(sleep));    // sleep�~���b�������~�߂�
	}

	//while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));	// �O�̃t���[������ 16ms �o�܂ő҂�.��sleep���Ȃ��̂�CPU�g�p�����オ��B
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;		// �f���^�^�C���̌v�Z
	if (deltaTime > 0.05f) { deltaTime = 0.05f; }			// �f���^�^�C�����ő�l�Ő�������
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
	//�N���A�J���[���D�F�ɐݒ�
	glClearColor(0.86f, 0.86f, 0.86f, 1.0f);
	
	//�J���[�o�b�t�@���N���A
	glClear(GL_COLOR_BUFFER_BIT);
	
	// ���ׂẴX�v���C�g�R���|�[�l���g��`��
	// �J���[�o�b�t�@�̃��u�����f�B���O��L����
	glEnable(GL_BLEND);
	glBlendFunc(
		GL_SRC_ALPHA,				// srcFactor��srcAlpha
		GL_ONE_MINUS_SRC_ALPHA		// dstFactor��(1-srcAlpha)
	);
	
	// �X�v���C�g�̃V�F�[�_�[�ƒ��_�z��I�u�W�F�N�g���A�N�e�B�u��
	mSpriteShader->SetActive();
	mSpriteVerts->SetActive();

	// ���ׂẴX�v���C�g��`��
	for (auto sprite : mSprites)
	{
		sprite->Draw(mSpriteShader);
	}

	//�o�b�t�@������
	SDL_GL_SwapWindow(mWindow);
	
}

void Game::LoadData()
{
	//�v���C���[�̉F���D���쐬
	mShip = new Ship(this);

	// ���f�����ŏ��ɕ�������
	int initialNumAsteroids = 15;		//�����l
	numAsteroids = 0;
	for (int i = 0; i < initialNumAsteroids; i++)
	{
		IncreaseAsteroid();
	}

	//�w�i���쐬
	new BackGround(this, 0, -10.0f, 5,"Assets/Farback01.png");
	new BackGround(this, 1, -10.0f, 5, "Assets/Farback02.png");
	new BackGround(this, 0, -20.0f, 15, "Assets/Stars.png");
	new BackGround(this, 1, -20.0f, 15, "Assets/Stars.png");
	
	mClearPict = new ClearPict(this);
}

void Game::UnloadData()
{
	// �A�N�^�[������
	while (!mActors.empty())
	{
		delete mActors.back();
	}

	// �e�N�X�`��������
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
	// �r���[�ˉe�s���ݒ�
	Matrix4 viewProj = Matrix4::CreateSimpleViewProj(static_cast<float>(mWindowWidth), static_cast<float>(mWindowHeight));
	mSpriteShader->SetMatrixUniform("uViewProj", viewProj);
	return true;

}

void Game::CreateSpriteVerts()
{
	// �Z�`�̒��_�o�b�t�@�@�@���_���W(Vector3), �e�N�X�`�����W(Vector2), ���_�J���[(RGBA)
	// ���_���W�́A���������_�A�E�E���E�������+x,+y,+z����
	// �e�N�X�`�����W�́A���オ���_
	float vertices[] = {
		-0.5f,  0.5f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f,			// ����  
		 0.5f,  0.5f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 1.f,			// �E��
		 0.5f, -0.5f, 0.f, 1.f, 1.f, 0.f, 0.f, 1.f, 1.f,			// �E��
		-0.5f, -0.5f, 0.f, 0.f, 1.f, 0.5f, 0.5f, 0.5f, 1.f			// ����
	};
	// �C���f�b�N�X�o�b�t�@
	// ���_�o�b�t�@�̏��Ԃ�ID 0,1,2,3�����蓖�Ă��Ă���A�|���S���̒��_ID���R�w�肷��B
	unsigned int indices[] = {
		0, 1, 2,		// ���� , �E�� , �E��
		2, 3, 0			// �E�� , ���� , ����
	};

	// ���_�o�b�t�@�̐���4 ,  �C���f�b�N�X�o�b�t�@�̐�=6
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

