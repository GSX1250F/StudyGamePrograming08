#pragma once
#include "SpriteComponent.h"
#include "Texture.h"
#include <vector>

class AnimSpriteComponent : public SpriteComponent
{
public:
	AnimSpriteComponent(class Actor* owner, int drawOrder = 100);
	// �t���[�����ƂɃA�j���[�V�������X�V����(component����I�[�o�[���C�h)
	void Update(float deltaTime) override;
	// �A�j���[�V�����Ɏg���e�N�X�`����ݒ肷��
	void SetAnimTextures(const std::vector<Texture*>& textures, int beg = 1, int last = -1, bool loop_flag = true);
	// �A�j���[�V�����Ɏg���e�N�X�`���͈̔͂�ݒ肷��
	void SetAnimNum(int beg, int last, bool loop_flag);
	// �A�j���[�V������FPS��ݒ� / �擾
	float GetAnimFPS() const { return mAnimFPS; }
	void SetAnimFPS(float fps) { mAnimFPS = fps; }

	// ���ݕ\�����̃A�j���[�V�����̏��擾
	int GetAnimNumBeg() { return mAnimNumBeg; }
	int GetAnimNumLast() { return mAnimNumLast; }
	int GetAnimNumCurr() { return static_cast<int>(mCurrFrame); }

	// �A�j���[�V���������[�v�����邩
	bool mLoopFlag;

	// �A�j���[�V���������ǂ���
	bool mIsAnimating;

private:
	// �A�j���[�V�����ł̂��ׂẴe�N�X�`��
	std::vector<Texture*> mAnimTextures;
	// ���ݕ\�����̃A�j���[�V�����e�N�X�`���̂��߂̔ԍ�
	int mAnimNumBeg;
	int mAnimNumLast;
	// ���ݕ\�����̃t���[��
	float mCurrFrame;
	// �A�j���[�V�����̃t���[�����[�g
	float mAnimFPS;
};

