#pragma once
#include "Component.h"
#include "Texture.h"
#include <SDL.h>

class SpriteComponent : public Component
{
public:
	// �`�揇��(draworder)���Ⴂ�قǉ����ɒu�����
	SpriteComponent(class Actor* owner, int drawOrder = 100);
	~SpriteComponent();

	virtual void Draw(class Shader* shader);
	virtual void SetTexture(class Texture* texture);

	int GetDrawOrder() const { return mDrawOrder; }
	int GetTexWidth() const { return mTexWidth; }
	int GetTexHeight() const { return mTexHeight; }

	void SetTexWidth(int width) { mTexWidth = width; }
	void SetTexHeight(int height) { mTexHeight = height; }


private:
	class Texture* mTexture;
	int mDrawOrder;
	int mTexWidth;			//�X�v���C�g�̉���
	int mTexHeight;			//�X�v���C�g�̏c��
};