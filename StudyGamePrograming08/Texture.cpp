#include "Texture.h"
#include <SDL.h>
#include <gl/glew.h>
#include <SOIL2.h>

Texture::Texture()
	:mTextureID(0)
	, mWidth(0)
	, mHeight(0)
{
}

Texture::~Texture()
{
}

bool Texture::Load(const std::string& fileName)
{
	int channels = 0;

	unsigned char* image = SOIL_load_image(
		fileName.c_str(),			// ファイル名
		&mWidth,					// 幅が記録される
		&mHeight,					// 高さが記録される
		&channels,					// チャネル数が記録される
		SOIL_LOAD_AUTO				// 画像ファイルの種類（またはauto）
	);

	if (image == nullptr)
	{
		SDL_Log("SOILが画像 %s のロードに失敗しました: %s", fileName.c_str(), SOIL_last_result());
		return false;
	}

	int format = GL_RGB;
	if (channels == 4)
	{
		format = GL_RGBA;
	}

	glGenTextures(1, &mTextureID);
	glBindTexture(GL_TEXTURE_2D, mTextureID);

	glTexImage2D(
		GL_TEXTURE_2D,				// テクスチャターゲット
		0,							// Level of Detail:詳細レベル（今は0とする）
		format,						// OpenGLが使うべきカラーフォーマット
		mWidth,						// テクスチャの幅
		mHeight,					// テクスチャの高さ
		0,							// 境界色（この値は0にする）
		format,						// 入力データのカラーフォーマット
		GL_UNSIGNED_BYTE,			// 入力データのビット深度。unsigned byteで８ビットチャネルを指定
		image						// 画像データへのポインタ
	);

	SOIL_free_image_data(image);

	// バイリニアフィルタリングを有効化
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return true;
}

void Texture::Unload()
{
	glDeleteTextures(1, &mTextureID);
}

void Texture::SetActive()
{
	glBindTexture(GL_TEXTURE_2D, mTextureID);
}
