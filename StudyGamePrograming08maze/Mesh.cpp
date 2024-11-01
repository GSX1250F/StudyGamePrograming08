#include "Mesh.h"
#include "Renderer.h"
#include "Texture.h"
#include "VertexInfo.h"
#include <fstream>
#include <sstream>
#include <rapidjson/document.h>
#include <SDL_log.h>
#include "Math.h"

Mesh::Mesh()
	: mVertexInfo(nullptr)
	, mRadius(0.0f)
	, mSpecPower(100.0f)
{}

Mesh::~Mesh()
{}

bool Mesh::Load(const std::string& fileName, Renderer* renderer)
{
	std::ifstream file(fileName);
	if (!file.is_open())
	{
		SDL_Log("メッシュファイルが存在しません。: %s", fileName.c_str());
		return false;
	}

	std::stringstream fileStream;
	fileStream << file.rdbuf();
	std::string contents = fileStream.str();
	rapidjson::StringStream jsonStr(contents.c_str());
	rapidjson::Document doc;
	doc.ParseStream(jsonStr);

	if (!doc.IsObject())
	{
		SDL_Log("メッシュファイル %s は有効なJSONファイルではありません。", fileName.c_str());
		return false;
	}

	int ver = doc["version"].GetInt();

	// Check the version
	if (ver != 1)
	{
		SDL_Log("メッシュファイル %s のバージョンが1ではありません。", fileName.c_str());
		return false;
	}

	mShaderName = doc["shader"].GetString();

	// Temporary 頂点配列フォーマット／シェーダーをスキップ
	size_t vertSize = 8;

	// テクスチャを読み込み
	const rapidjson::Value& textures = doc["textures"];
	if (!textures.IsArray() || textures.Size() < 1)
	{
		SDL_Log("メッシュファイル %s にテクスチャが見つかりません。最低でも１つ必要です。", fileName.c_str());
		return false;
	}

	mSpecPower = static_cast<float>(doc["specularPower"].GetDouble());

	for (rapidjson::SizeType i = 0; i < textures.Size(); i++)
	{
		// このテクスチャがすでに読み込まれたか？
		std::string texName = textures[i].GetString();
		Texture* t = renderer->GetTexture(texName);
		if (t == nullptr)
		{
			// テクスチャ読み込みを試行
			t = renderer->GetTexture(texName);
			if (t == nullptr)
			{
				// Nullのままの場合、デフォルトのテクスチャを使用する。
				t = renderer->GetTexture("Assets/Default.png");
			}
		}
		mTextures.emplace_back(t);
	}

	// 頂点配列群をロード
	const rapidjson::Value& vertsJson = doc["vertices"];
	if (!vertsJson.IsArray() || vertsJson.Size() < 1)
	{
		SDL_Log("メッシュファイル %s が頂点配列群を持っていません", fileName.c_str());
		return false;
	}

	std::vector<float> vertices;
	vertices.reserve(vertsJson.Size() * vertSize);
	mRadius = 0.0f;
	for (rapidjson::SizeType i = 0; i < vertsJson.Size(); i++)
	{
		// For now, just assume we have 8 elements
		const rapidjson::Value& vert = vertsJson[i];
		if (!vert.IsArray() || vert.Size() != 8)
		{
			SDL_Log("%s が想定外の頂点配列フォーマットです。", fileName.c_str());
			return false;
		}

		Vector3 pos(vert[0].GetDouble(), vert[1].GetDouble(), vert[2].GetDouble());
		mRadius = Math::Max(mRadius, pos.LengthSq());

		// Add the floats
		for (rapidjson::SizeType i = 0; i < vert.Size(); i++)
		{
			vertices.emplace_back(static_cast<float>(vert[i].GetDouble()));
		}
	}

	// We were computing length squared earlier
	mRadius = Math::Sqrt(mRadius);

	// Load in the indices
	const rapidjson::Value& indJson = doc["indices"];
	if (!indJson.IsArray() || indJson.Size() < 1)
	{
		SDL_Log("Mesh %s has no indices", fileName.c_str());
		return false;
	}

	std::vector<unsigned int> indices;
	indices.reserve(indJson.Size() * 3);
	for (rapidjson::SizeType i = 0; i < indJson.Size(); i++)
	{
		const rapidjson::Value& ind = indJson[i];
		if (!ind.IsArray() || ind.Size() != 3)
		{
			SDL_Log("Invalid indices for %s", fileName.c_str());
			return false;
		}

		indices.emplace_back(ind[0].GetUint());
		indices.emplace_back(ind[1].GetUint());
		indices.emplace_back(ind[2].GetUint());
	}

	// Now create a vertex array
	mVertexInfo = new VertexInfo(vertices.data(), static_cast<unsigned>(vertices.size()) / vertSize,
		indices.data(), static_cast<unsigned>(indices.size()));
	return true;
}

void Mesh::Unload()
{
	delete mVertexInfo;
	mVertexInfo = nullptr;
}

Texture* Mesh::GetTexture(size_t index)
{
	if (index < mTextures.size())
	{
		return mTextures[index];
	}
	else
	{
		return nullptr;
	}
}
