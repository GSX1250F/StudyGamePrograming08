#pragma once
#include "SpriteComponent.h"
#include "Texture.h"
#include <vector>

class AnimSpriteComponent : public SpriteComponent
{
public:
	AnimSpriteComponent(class Actor* owner, int drawOrder = 100);
	// フレームごとにアニメーションを更新する(componentからオーバーライド)
	void Update(float deltaTime) override;
	// アニメーションに使うテクスチャを設定する
	void SetAnimTextures(const std::vector<Texture*>& textures, int beg = 1, int last = -1, bool loop_flag = true);
	// アニメーションに使うテクスチャの範囲を設定する
	void SetAnimNum(int beg, int last, bool loop_flag);
	// アニメーションのFPSを設定 / 取得
	float GetAnimFPS() const { return mAnimFPS; }
	void SetAnimFPS(float fps) { mAnimFPS = fps; }

	// 現在表示中のアニメーションの情報取得
	int GetAnimNumBeg() { return mAnimNumBeg; }
	int GetAnimNumLast() { return mAnimNumLast; }
	int GetAnimNumCurr() { return static_cast<int>(mCurrFrame); }

	// アニメーションをループさせるか
	bool mLoopFlag;

	// アニメーション中かどうか
	bool mIsAnimating;

private:
	// アニメーションでのすべてのテクスチャ
	std::vector<Texture*> mAnimTextures;
	// 現在表示中のアニメーションテクスチャのための番号
	int mAnimNumBeg;
	int mAnimNumLast;
	// 現在表示中のフレーム
	float mCurrFrame;
	// アニメーションのフレームレート
	float mAnimFPS;
};

