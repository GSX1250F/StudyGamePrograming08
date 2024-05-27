#include "SpriteComponent.h"
#include "Texture.h"
#include "Shader.h"
#include "Actor.h"
#include "Game.h"


SpriteComponent::SpriteComponent(Actor* owner, int drawOrder)
	:Component(owner),
	mTexture(nullptr),
	mDrawOrder(drawOrder),
	mTexWidth(0),
	mTexHeight(0)
{
	mOwner->GetGame()->AddSprite(this);
}

SpriteComponent::~SpriteComponent()
{
	mOwner->GetGame()->RemoveSprite(this);
}

/*
void SpriteComponent::Draw(SDL_Renderer* renderer)
{
	//------------------追加した箇所------------------------
	if (mTexture && mOwner->GetState() != mOwner->EPaused)
	{
		SDL_Rect r;
		// 高さと幅を所有アクターのスケールに合わせる
		r.w = static_cast<int>(mTexWidth * mOwner->GetScale());
		r.h = static_cast<int>(mTexHeight * mOwner->GetScale());
		// Center the rectangle around the position of the owner
		r.x = static_cast<int>(mOwner->GetPosition().x - r.w / 2);
		r.y = static_cast<int>(mOwner->GetPosition().y - r.h / 2);

		SDL_RenderCopyEx(renderer, mTexture, nullptr, &r, -Math::ToDegrees(mOwner->GetRotation()), nullptr, SDL_FLIP_NONE);
	}


}
*/

void SpriteComponent::Draw(Shader* shader)
{
	if (mTexture && mOwner->GetState() != mOwner->EPaused)	// EPausedのときはDrawしないように修正
	{
		// テクスチャの幅と高さで短径をスケーリング
		Matrix4 scaleMat = Matrix4::CreateScale(
			static_cast<float>(mTexWidth),
			static_cast<float>(mTexHeight),
			1.0f);

		Matrix4 world = scaleMat * mOwner->GetWorldTransform();

		// ワールド変換の設定
		shader->SetMatrixUniform("uWorldTransform", world);
		// 現在のテクスチャをセット
		mTexture->SetActive();

		// 短径を描画
		glDrawElements(
			GL_TRIANGLES,		// 描画するポリゴン／プリミティブの種類
			6,					// インデックスバッファにあるインデックスの数
			GL_UNSIGNED_INT,	// インデックスの型
			nullptr				// 通常はnullptr
		);
	}
}

void SpriteComponent::SetTexture(Texture* texture)
{
	mTexture = texture;
	// Set width/height
	mTexWidth = texture->GetWidth();
	mTexHeight = texture->GetHeight();
	// Actorの半径を、widthとheightの平均/2にする。
	mOwner->SetRadius((mTexWidth + mTexHeight) * 0.25f);
}