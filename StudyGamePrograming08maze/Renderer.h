#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <SDL.h>
#include "Math.h"
#include "Texture.h"
#include "VertexInfo.h"

struct DirectionalLight
{
	Vector3 mDirection;		// 光の方向
	Vector3 mDiffuseColor;	// 拡散反射色
	Vector3 mSpecColor;		// 鏡面反射色
};

struct PointLight
{
	Vector3 mPosition;		// 光源の位置
	Vector3 mDiffuseColor;	// 拡散反射色
	Vector3 mSpecColor;		// 鏡面反射色
	float mAttenuation;		// 減衰係数
};

struct SpotLight
{
	Vector3 mPosition;		// 光源の位置
	Vector3 mDirection;		// 光の中心方向
	Vector3 mDiffuseColor;	// 拡散反射色
	Vector3 mSpecColor;		// 鏡面反射色
	float mAttenuation;		// 減衰係数
	float mCornAngle;		// 照射角度
	float mFalloff;			// 照射角度外減衰指数
};

class Renderer
{
public:
	Renderer(class Game* game);
	~Renderer();

	bool Initialize(float screenWidth, float screenHeight);
	void UnloadData();
	void Shutdown();

	void Draw();

	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);

	void AddMeshComp(class MeshComponent* mesh);
	void RemoveMeshComp(class MeshComponent* mesh);

	class Texture* GetTexture(const std::string& fileName);
	class Mesh* GetMesh(const std::string& fileName);
	class VertexInfo* GetVertexInfo() { return mVertexInfo; }

	void SetViewMatrix(const Matrix4& matrix) { mView = matrix; }
	void SetProjMatrix(const Matrix4& matrix) { mProj = matrix; }

	Vector3& GetAmbientLight() { return mAmbientLight; }
	void SetAmbientLight(const Vector3 ambient) { mAmbientLight = ambient; }
	//DirectionalLight& GetDirectionalLight() { return mDirLight; }
	std::vector<DirectionalLight>& GetDirectionalLights() { return mDirLights; }
	//void SetDirectionalLight(const DirectionalLight& dir) { mDirLight = dir; }
	void AddDirectionalLight(const DirectionalLight dl) { mDirLights.emplace_back(dl); }
	std::vector<PointLight>& GetPointLights() { return mPointLights; }
	void AddPointLight(const PointLight pl) { mPointLights.emplace_back(pl); }
	std::vector<SpotLight>& GetSpotLights() { return mSpotLights; }
	void AddSpotLight(const SpotLight sl) { mSpotLights.emplace_back(sl); }

	float GetScreenWidth() const { return mScreenWidth; }
	float GetScreenHeight() const { return mScreenHeight; }

private:
	void CreateVertexInfo();
	bool LoadShaders();
	void SetLightUniforms(class Shader* shader);

	std::unordered_map<std::string, class Texture*> mTextures;
	std::unordered_map<std::string, class Mesh*> mMeshes;
	std::unordered_map<class Shader*, std::string> mShaders;	//シェーダーとシェーダー名の連想配列
	std::vector<class SpriteComponent*> mSprites;
	std::vector<class MeshComponent*> mMeshComps;

	class Game* mGame;
	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	SDL_GLContext mContext;

	float mScreenWidth;
	float mScreenHeight;

	class VertexInfo* mVertexInfo;
	class Shader* mSpriteShader;
	//class Shader* mMeshShader;	

	Matrix4 mView;
	Matrix4 mProj;

	// 環境光と光源
	Vector3 mAmbientLight;
	//DirectionalLight mDirLight;
	std::vector<DirectionalLight> mDirLights;
	std::vector<PointLight> mPointLights;
	std::vector<SpotLight> mSpotLights;
};