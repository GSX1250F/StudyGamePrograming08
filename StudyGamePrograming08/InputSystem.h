#pragma once
#include <SDL_scancode.h>
#include <SDL_gamecontroller.h>
#include <SDL_mouse.h>
#include "Math.h"

enum ButtonState
{
	ENone,
	EPressed,
	EHeld,
	EReleased
};

// �L�[�{�[�h���͂̃w���p�[�N���X
class KeyboardState
{
public:
	// InputSystem��Friend�N���X�Ƃ��邱�Ƃŗe�ՂɍX�V�ł���
	friend class InputSystem;
	// �L�[�̐^�U�l�݂̂��擾
	bool GetKeyValue(SDL_Scancode keyCode) const;
	// ���݂Ƃ��̂P�O�̃t���[�������Ԃ��擾
	ButtonState GetKeyState(SDL_Scancode keyCode) const;
private:
	const Uint8* mCurrState;	//���݂̏��
	Uint8 mPrevState[SDL_NUM_SCANCODES];	//�P�O�̃t���[���̏��
};

// �}�E�X���͂̃w���p�[�N���X
class MouseState
{
public:
	// InputSystem��Friend�N���X�Ƃ��邱�Ƃŗe�ՂɍX�V�ł���
	friend class InputSystem;

	// �}�E�X�̈ʒu
	const Vector2& GetPosition() const { return mMousePos; }
	// �X�N���[���z�C�[���̈ʒu
	const Vector2& GetScrollWheel() const { return mScrollWheel; }
	// ���΃}�E�X���[�h���ǂ������擾
	bool GetIsRelative() const { return mIsRelative; }

	// �}�E�X�{�^�����
	bool GetButtonValue(int button) const;
	ButtonState GetButtonState(int button) const;
private:
	// �}�E�X�ʒu��ۑ�
	Vector2 mMousePos;
	// �X�N���[���z�C�[���̈ʒu��ۑ�
	Vector2 mScrollWheel;
	// �{�^���̏�Ԃ�ۑ�
	Uint32 mCurrButtons;
	Uint32 mPrevButtons;
	// ���΃}�E�X���[�h���ۂ�
	bool mIsRelative;
};

// �R���g���[�����͂̃w���p�[�N���X
class ControllerState
{
public:
	// InputSystem��Friend�N���X�Ƃ��邱�Ƃŗe�ՂɍX�V�ł���
	friend class InputSystem;

	// �{�^���̏��
	bool GetButtonValue(SDL_GameControllerButton button) const;
	ButtonState GetButtonState(SDL_GameControllerButton button) const;
	// �X�e�B�b�N�E�g���K�[�̏��
	const Vector2& GetLeftStick() const { return mLeftStick; }
	const Vector2& GetRightStick() const { return mRightStick; }
	float GetLeftTrigger() const { return mLeftTrigger; }
	float GetRightTrigger() const { return mRightTrigger; }
	// �ڑ�����Ă��邩�ǂ���
	bool GetIsConnected() const { return mIsConnected; }
private:
	// ���݂ƂP�O�̃{�^���̏�Ԃ�ۑ�
	Uint8 mCurrButtons[SDL_CONTROLLER_BUTTON_MAX];
	Uint8 mPrevButtons[SDL_CONTROLLER_BUTTON_MAX];
	// Left/right�X�e�B�b�N
	Vector2 mLeftStick;
	Vector2 mRightStick;
	// Left/right�g���K�[
	float mLeftTrigger;
	float mRightTrigger;
	// �ڑ�����Ă��邩�ǂ���
	bool mIsConnected;
};

// ���݂̓��͏�Ԃ��i�[���郉�b�p�[
struct InputState
{
	KeyboardState Keyboard;
	MouseState Mouse;
	ControllerState Controller;
};

class InputSystem
{
public:
	bool Initialize();
	void Shutdown();

	// SDL_PolEvents���[�v�̒��O�ɌĂ΂��
	void PrepareForUpdate();
	// SDL_PolEvents���[�v�̒���ɌĂ΂��
	void Update();
	// InputSystem��SDL�C�x���g��n�����߂ɌĂ΂��
	void ProcessEvent(union SDL_Event& event);

	const InputState& GetState() const { return mState; }

	// �}�E�X�̑��΃��[�h��ݒ�(true:�I��, false:�I�t)
	void SetRelativeMouseMode(bool value);

private:
	InputState mState;
	// �A�i���O�X�e�B�b�N�f�b�h�]�[���ݒ�i�P�����j
	float Filter1D(int input);
	// �A�i���O�X�e�B�b�N�f�b�h�]�[���ݒ�i�Q�����j
	Vector2 Filter2D(int inputX, int inputY);
	SDL_GameController* mController;
};

