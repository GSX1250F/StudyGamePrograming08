#pragma once
#include "Actor.h"
class Asteroid : public Actor
{
public:
	Asteroid(Game* game);
	~Asteroid();

	class CircleComponent* GetCircle() { return mCircle; }

	// �A�N�^�[�Ǝ��̍X�V����(�I�[�o�[���C�h�\)
	void UpdateActor(float deltaTime) override;

private:
	class CircleComponent* mCircle;		//�Փ˃`�F�b�N�̂��߂̃A�N�Z�X�|�C���^�B���̃I�u�W�F�N�g����Q�Ƃ��邽�߁B
	float mAsteroidCooldown;		//����or���B�܂ł̑ҋ@����
};

