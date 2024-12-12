#include "Renderer.h"
#include <SDL.h>
#include <SDL_image.h>
#include <algorithm>
#include "SpriteComponent.h"
#include <glew.h>
#include "Shader.h"
#include "VertexInfo.h"
#include "Texture.h"

Renderer::Renderer(Game* game) 
	: mGame(game)
	, mWindow(nullptr)
	, mShader(nullptr)
	, mVertexInfo(nullptr)
{}

Renderer::~Renderer()
{}

bool Renderer::Initialize(float screenWidth, float screenHeight)
{
	// OpenGL�����ݒ�
	// OpenGL�̃R�A�v���t�@�C�����g�p
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	// version 3.3���w��
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	// �e�J���[�`�����l��8�r�b�g(RGBA)
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	// �_�u���o�b�t�@�L����
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	// OpenGL�̃n�[�h�E�F�A�A�N�Z�����[�^�g�p
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	mScreenWidth = screenWidth;
	mScreenHeight = screenHeight;

	// SDL�E�B���h�E���쐬
	mWindow = SDL_CreateWindow(
		"Game Programming in C++",
		100,
		100,
		static_cast<int>(screenWidth),
		static_cast<int>(screenHeight),
		SDL_WINDOW_OPENGL					//SDL�E�B���h�E��OpenGL���g�p����ۂɎw��
	);

	if (!mWindow)
	{
		SDL_Log("�E�B���h�E�̍쐬�Ɏ��s���܂���: %s", SDL_GetError());
		return false;
	}

	// OpenGL�R���e�N�X�g�𐶐��i���ׂĂ�OpenGL�@�\�ɃA�N�Z�X����j
	mContext = SDL_GL_CreateContext(mWindow);

	// GLEW��������
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		SDL_Log("GLEW�̏������Ɏ��s���܂����B");
		return false;
	}
	glGetError();

	// �o�[�e�b�N�X�z��I�u�W�F�N�g�̐���
	CreateVertexInfo();

	// �V�F�[�_�[�̐���
	if (!LoadShaders())
	{
		SDL_Log("�V�F�[�_�[�̓ǂݍ��݂Ɏ��s���܂����B");
		return false;
	}

	// ��ʃN���A�̐F��ݒ�
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

	return true;
}

void Renderer::UnloadData()
{
	mTextures.clear();
}

void Renderer::Shutdown()
{
	delete mVertexInfo;
	mShader->Unload();
	delete mShader;
	SDL_GL_DeleteContext(mContext);
	SDL_DestroyWindow(mWindow);
}


void Renderer::Draw()
{
	// ��ʂ��N���A	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// �J���[�o�b�t�@�̃A���t�@�u�����f�B���O��L����
	glEnable(GL_BLEND);
	glBlendFunc(
		GL_SRC_ALPHA,				// srcFactor��srcAlpha
		GL_ONE_MINUS_SRC_ALPHA		// dstFactor��(1-srcAlpha)
	);
	//�[�x�L����
	//glEnable(GL_DEPTH_TEST);

	// �V�F�[�_�[�ƃo�[�e�b�N�X�z��I�u�W�F�N�g��L����
	mVertexInfo->SetActive();
	mShader->SetActive();

	// �V�F�[�_�[�̃r���[�ϊ�&�ˉe�ϊ��s����X�V
	mShader->SetMatrixUniform("uViewProj", mView * mProj);

	for (auto sprite : mSprites)
	{
		if (sprite->GetVisible())
		{
			sprite->Draw(mShader);
		}
	}
	// �_�u���o�b�t�@������
	SDL_GL_SwapWindow(mWindow);
}

void Renderer::AddSprite(SpriteComponent* sprite)
{
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
	mSprites.insert(iter, sprite);
}

void Renderer::RemoveSprite(SpriteComponent* sprite)
{
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	mSprites.erase(iter);
}

class Texture* Renderer::GetTexture(const std::string& filename)
{
	class Texture* tex = nullptr;
	auto iter = mTextures.find(filename);
	if (iter != mTextures.end())
	{
		tex = iter->second;
	}
	else
	{
		tex = new Texture();
		if (tex->Load(filename))
		{
			mTextures.emplace(filename, tex);
		}
		else
		{
			delete tex;
			tex = nullptr;
		}
	}
	return tex;
}

void Renderer::CreateVertexInfo()
{
	int numVerts = 4;			//���_�̐�
	//���_���W(vector2)
	float vertPos[] = {
		-0.5f, 0.5f, 0.0f, 			// ����
		-0.5f, -0.5f, 0.0f,			// ����
		 0.5f, -0.5f, 0.0f,			// �E��
		 0.5f, 0.5f, 0.0f 			// �E��
	};
	//�e�N�X�`�����W(vector2)
	float texCoord[] = {
		0.0f, 0.0f,			//����
		0.0f, 1.0f,			//����		
		1.0f, 1.0f,			//�E��
		1.0f, 0.0f			//�E��
	};
	//���_�J���[(vector4 RGBA)
	float vertColor[] = {
		1.0f, 0.0f, 0.0f, 1.0f,		//R
		0.0f, 1.0f, 0.0f, 1.0f,		//G
		0.0f, 0.0f, 1.0f, 1.0f,		//B
		1.0f, 1.0f, 1.0f, 1.0f		//W
	};
	int numIndices = 6;			//�C���f�b�N�X�̐�
	//�C���f�b�N�X
	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	mVertexInfo = new VertexInfo(vertPos, texCoord, vertColor, indices, numVerts, numIndices);
}

bool Renderer::LoadShaders()
{
	// �V�F�[�_�[�𐶐�
	mShader = new Shader();
	if (!mShader->Load("Shaders/shader.vert", "Shaders/shader.frag"))
	{
		return false;
	}
	mShader->SetActive();
	// �r���[�ϊ��Ǝˉe�ϊ��s����쐬�B�����ł͕��s���e�ϊ����s���B
	mView = Matrix4::Identity;
	//mProj = Matrix4::CreateSimpleViewProj(mScreenWidth, mScreenHeight);
	mProj = Matrix4::CreateOrtho(mScreenWidth, mScreenHeight, 0.1f, 1000.0f);
	mShader->SetMatrixUniform("uViewProj", mView * mProj);
	return true;
}