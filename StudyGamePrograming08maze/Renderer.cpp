#include "Renderer.h"
#include <SDL.h>
#include <SDL_image.h>
#include <algorithm>
#include <glew.h>
#include "SpriteComponent.h"
#include "MeshComponent.h"
#include "Shader.h"
#include "VertexInfo.h"
#include "Texture.h"
#include "Mesh.h"

Renderer::Renderer(Game* game)
	: mGame(game)
	, mWindow(nullptr)
	, mSpriteShader(nullptr)
	, mMeshShader(nullptr)
	, mVertexInfo(nullptr)
{}

Renderer::~Renderer()
{}

bool Renderer::Initialize(float screenWidth, float screenHeight)
{
	// OpenGL初期設定
	// OpenGLのコアプロファイルを使用
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	// version 3.3を指定
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	// 各カラーチャンネル8ビット(RGBA)
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	// ダブルバッファ有効化
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	// OpenGLのハードウェアアクセラレータ使用
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	mScreenWidth = screenWidth;
	mScreenHeight = screenHeight;

	// SDLウィンドウを作成
	mWindow = SDL_CreateWindow(
		"Game Programming in C++",
		100,
		100,
		static_cast<int>(screenWidth),
		static_cast<int>(screenHeight),
		SDL_WINDOW_OPENGL					//SDLウィンドウでOpenGLを使用する際に指定
	);

	if (!mWindow)
	{
		SDL_Log("ウィンドウの作成に失敗しました: %s", SDL_GetError());
		return false;
	}
	
	// OpenGLコンテクストを生成（すべてのOpenGL機能にアクセスする）
	mContext = SDL_GL_CreateContext(mWindow);

	// GLEWを初期化
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		SDL_Log("GLEWの初期化に失敗しました。");
		return false;
	}
	glGetError();

	// バーテックス配列オブジェクトの生成
	CreateVertexInfo();

	// シェーダーの生成
	if (!LoadShaders())
	{
		SDL_Log("シェーダーの読み込みに失敗しました。");
		return false;
	}

	// 画面クリアの色を設定
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	return true;
}

void Renderer::UnloadData()
{
	for (auto i : mTextures)
	{
		i.second->Unload();
		delete i.second;
	}
	mTextures.clear();

	for (auto i : mMeshes)
	{
		i.second->Unload();
		delete i.second;
	}
	mMeshes.clear();
}

void Renderer::Shutdown()
{
	delete mVertexInfo;
	mSpriteShader->Unload();
	delete mSpriteShader;
	mMeshShader->Unload();
	delete mMeshShader;
	SDL_GL_DeleteContext(mContext);
	SDL_DestroyWindow(mWindow);
}


void Renderer::Draw()
{
	// 画面をクリア & 深度バッファをクリア
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 深度有効化、アルファブレンディング無効化
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	// メッシュを描画
	mMeshShader->SetActive();
	mMeshShader->SetMatrixUniform("uViewProj", mView * mProj);

	// 光源のUniform変数を更新
	SetLightUniforms(mMeshShader);

	for (auto mc : mMeshComps)
	{
		mc->Draw(mMeshShader);
	}

	// 深度無効化、アルファブレンディング有効化
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
	// スプライトを描画
	mSpriteShader->SetActive();
	mVertexInfo->SetActive();
	for (auto sprite : mSprites)
	{
		sprite->Draw(mSpriteShader);
	}

	// ダブルバッファを交換
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
void Renderer::AddMeshComp(MeshComponent* mesh)
{
	mMeshComps.emplace_back(mesh);
}

void Renderer::RemoveMeshComp(MeshComponent* mesh)
{
	auto iter = std::find(mMeshComps.begin(), mMeshComps.end(), mesh);
	mMeshComps.erase(iter);
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
Mesh* Renderer::GetMesh(const std::string& fileName)
{
	class Mesh* m = nullptr;
	auto iter = mMeshes.find(fileName);
	if (iter != mMeshes.end())
	{
		m = iter->second;
	}
	else
	{
		m = new Mesh();
		if (m->Load(fileName, this))
		{
			mMeshes.emplace(fileName, m);
		}
		else
		{
			delete m;
			m = nullptr;
		}
	}
	return m;
}

void Renderer::CreateVertexInfo()
{
	// 左手座標系
	// 頂点座標(vector3), 法線ベクトル(Vector3), テクスチャ座標(Vector2)
	float vertices[] = {
		-0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,	 // top left
		 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,	 // top right
		 0.5f,-0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,	 // bottom right
		-0.5f,-0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f		 // bottom left
	};
	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};
	mVertexInfo = new VertexInfo(vertices, 4, indices, 6);
}

bool Renderer::LoadShaders()
{
	// スプライト用シェーダーを生成
	mSpriteShader = new Shader();
	if (!mSpriteShader->Load("Shaders/SpriteShader.vert", "Shaders/SpriteShader.frag"))
	{
		return false;
	}
	mSpriteShader->SetActive();
	// スプライトの描画には、平行射影を行う。
	mView = Matrix4::Identity;
	mProj = Matrix4::CreateSimpleViewProj(mScreenWidth, mScreenHeight);
	mSpriteShader->SetMatrixUniform("uViewProj", mView * mProj);
	
	// メッシュ用シェーダーを生成
	mMeshShader = new Shader();
	if (!mMeshShader->Load("Shaders/MeshShader.vert", "Shaders/MeshShader.frag"))
	{
		return false;
	}
	mMeshShader->SetActive();
	// メッシュの描画には、透視射影を行う。
	Vector3 cameraPos = Vector3::Zero;
	Vector3 cameraTarget = Vector3::UnitX;
	Vector3 cameraUp = Vector3::UnitZ;
	mView = Matrix4::CreateLookAt(cameraPos, cameraTarget, cameraUp);
	mProj = Matrix4::CreatePerspectiveFOV(Math::ToRadians(70.0f), mScreenWidth, mScreenHeight, 0.01f, 10000.0f);
	mMeshShader->SetMatrixUniform("uViewProj", mView * mProj);

	return true;
}

void Renderer::SetLightUniforms(Shader* shader)
{
	// カメラの位置はビューを反転して求める
	Matrix4 invView = mView;
	invView.Invert();
	shader->SetVectorUniform("uCameraPos", invView.GetTranslation());
	// 環境光
	shader->SetVectorUniform("uAmbientLight", mAmbientLight);
	// 平行光源
	shader->SetVectorUniform("uDirLight.mDirection", mDirLight.mDirection);
	shader->SetVectorUniform("uDirLight.mDiffuseColor", mDirLight.mDiffuseColor);
	shader->SetVectorUniform("uDirLight.mSpecColor", mDirLight.mSpecColor);
}