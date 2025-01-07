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
	friend class InputSystem;
	bool GetKeyValue(SDL_Scancode keyCode) const;	// �L�[�̐^�U�l�݂̂��擾
	ButtonState GetKeyState(SDL_Scancode keyCode) const;	// ���݂Ƃ��̂P�O�̃t���[�������Ԃ��擾
private:
	const Uint8* mCurrState;
	Uint8 mPrevState[SDL_NUM_SCANCODES];
};

// �}�E�X���͂̃w���p�[�N���X
class MouseState
{
public:
	friend class InputSystem;
	const Vector2& GetPosition() const { return mMousePos; }	// �}�E�X�̈ʒu
	const Vector2& GetScrollWheel() const { return mScrollWheel; }	// �X�N���[���z�C�[���̈ʒu
	bool GetIsRelative() const { return mIsRelative; }	// ���΃}�E�X���[�h���ǂ������擾
	bool GetButtonValue(int button) const;	// �}�E�X�{�^�����
	ButtonState GetButtonState(int button) const;	// ���݂Ƃ��̂P�O�̃t���[�������Ԃ��擾
private:
	Vector2 mMousePos;
	Vector2 mScrollWheel;
	Uint32 mCurrButtons;
	Uint32 mPrevButtons;
	bool mIsRelative;	// ���΃}�E�X���[�h���ۂ�
};

// �R���g���[�����͂̃w���p�[�N���X
class ControllerState
{
public:
	friend class InputSystem;
	bool GetButtonValue(SDL_GameControllerButton button) const;
	ButtonState GetButtonState(SDL_GameControllerButton button) const;
	const Vector2& GetLeftStick() const { return mLeftStick; }
	const Vector2& GetRightStick() const { return mRightStick; }
	float GetLeftTrigger() const { return mLeftTrigger; }
	float GetRightTrigger() const { return mRightTrigger; }
	bool GetIsConnected() const { return mIsConnected; }	// �ڑ�����Ă��邩�ǂ���
private:
	Uint8 mCurrButtons[SDL_CONTROLLER_BUTTON_MAX];
	Uint8 mPrevButtons[SDL_CONTROLLER_BUTTON_MAX];
	Vector2 mLeftStick;
	Vector2 mRightStick;
	float mLeftTrigger;
	float mRightTrigger;
	bool mIsConnected;	// �ڑ�����Ă��邩�ǂ���
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

