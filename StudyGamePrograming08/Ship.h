#pragma once
#include "Actor.h"
#include <string>
#include <vector>

class Ship : public Actor
{
public:
	Ship(Game* game);
	
	void ActorInput(const uint8_t* keyState) override;
	void UpdateActor(float deltaTime) override;

	void Init();

private:
	float mLaserCooldown;
	bool mCrash;				//�Փˌ��m
	float mCrashingTime;		//�Փˉ��o����
	float mCrashCooldown;		//�Փˉ��o��A���Z�b�g�����܂ŃX�v���C�g����������
	Vector3 mCrashPos;		//�Փ˂����Ƃ��̈ʒu
	Quaternion mCrashRot;			//�Փ˂����Ƃ��̌���
	class CircleComponent* mCircle; 
	class SomeSpriteComponent* mSSC;
	class InputComponent* mIC;
	std::vector<std::string> mChunkFiles;
};