#include "InputSystem.h"
#include <SDL.h>
#include <cstring>

bool KeyboardState::GetKeyValue(SDL_Scancode keyCode) const
{
	if (mCurrState[keyCode] == 1) 
	{
		return true; 
	}
	else 
	{
		return false; 
	}
	//return mCurrState[keyCode] == 1;
}

ButtonState KeyboardState::GetKeyState(SDL_Scancode keyCode) const
{
	// prev		curr
	// 0		0		-> ENone
	// 0		1		-> EPressed
	// 1		0		-> EReleased
	// 1		1		-> EHeld		

	if (mPrevState[keyCode] == 0)
	{
		if (mCurrState[keyCode] == 0)
		{
			return ENone;
		}
		else
		{
			return EPressed;
		}
	}
	else // �P�O�̏�Ԃ� 1 �ɂȂ�B
	{
		if (mCurrState[keyCode] == 0)
		{
			return EReleased;
		}
		else
		{
			return EHeld;
		}
	}
}

void MouseState::SetRelativeMouseMode(bool value)
{
	SDL_bool set = value ? SDL_TRUE : SDL_FALSE;
	SDL_SetRelativeMouseMode(set);

	mIsRelative = value;
}

bool MouseState::GetButtonValue(int button) const
{
	return (SDL_BUTTON(button) & mCurrButtons);
}

ButtonState MouseState::GetButtonState(int button) const
{
	int mask = SDL_BUTTON(button);
	if ((mask & mPrevButtons) == 0)
	{
		if ((mask & mCurrButtons) == 0)
		{
			return ENone;
		}
		else
		{
			return EPressed;
		}
	}
	else // �P�O�̏�Ԃ� 1 �ɂȂ�B
	{
		if ((mask & mCurrButtons) == 0)
		{
			return EReleased;
		}
		else
		{
			return EHeld;
		}
	}
}

bool ControllerState::GetButtonValue(SDL_GameControllerButton button) const
{
	return mCurrButtons[button] == 1;
}

ButtonState ControllerState::GetButtonState(SDL_GameControllerButton button) const
{
	if (mPrevButtons[button] == 0)
	{
		if (mCurrButtons[button] == 0)
		{
			return ENone;
		}
		else
		{
			return EPressed;
		}
	}
	else // �P�O�̏�Ԃ� 1 �ɂȂ�B
	{
		if (mCurrButtons[button] == 0)
		{
			return EReleased;
		}
		else
		{
			return EHeld;
		}
	}
}

bool InputSystem::Initialize()
{
	// �L�[�{�[�h
	mState.Keyboard.mCurrState = SDL_GetKeyboardState(NULL);	// ���݂̏�Ԃ������|�C���^��ۑ�
	memset(mState.Keyboard.mPrevState, 0, SDL_NUM_SCANCODES);	// �O��̏�Ԃ��������������N���A

	// �}�E�X
	mState.Mouse.mCurrButtons = 0;
	mState.Mouse.mPrevButtons = 0;

	// �R���g���[�����ڑ�����Ă����炻����擾
	mController = SDL_GameControllerOpen(0);
	mState.Controller.mIsConnected = (mController != nullptr);	// �R���g���[����Ԃ�������
	memset(mState.Controller.mCurrButtons, 0, SDL_CONTROLLER_BUTTON_MAX);
	memset(mState.Controller.mPrevButtons, 0, SDL_CONTROLLER_BUTTON_MAX);

	return true;
}

void InputSystem::Shutdown()
{
}

void InputSystem::PrepareForUpdate()
{
	// ���݂̏�Ԃ��P�O�̏�ԂɃR�s�[����B
	// �L�[�{�[�h
	memcpy(mState.Keyboard.mPrevState, mState.Keyboard.mCurrState, SDL_NUM_SCANCODES);
	// �}�E�X
	mState.Mouse.mPrevButtons = mState.Mouse.mCurrButtons;
	mState.Mouse.mIsRelative = false;
	// �}�E�X�z�C�[���C�x���g�̓z�C�[�����������t���[�������Ńg���K�[�����̂ŁA�N���A���Ă���
	mState.Mouse.mScrollWheel = Vector2::Zero;

	// �R���g���[��
	memcpy(mState.Controller.mPrevButtons,
		   mState.Controller.mCurrButtons,
		   SDL_CONTROLLER_BUTTON_MAX);

}

void InputSystem::Update()
{
	// �}�E�X
	int x = 0, y = 0;
	if (mState.Mouse.mIsRelative)
	{
		// ���΃��[�h�̂Ƃ�
		mState.Mouse.mCurrButtons =	SDL_GetRelativeMouseState(&x, &y);
	}
	else
	{
		// ��΃��[�h�̂Ƃ�
		mState.Mouse.mCurrButtons =	SDL_GetMouseState(&x, &y);
	}
	mState.Mouse.mMousePos.x = static_cast<float>(x);
	mState.Mouse.mMousePos.y = static_cast<float>(y);

	// �R���g���[��
	// �{�^��
	for (int i = 0; i < SDL_CONTROLLER_BUTTON_MAX; i++)
	{
		mState.Controller.mCurrButtons[i] =
			SDL_GameControllerGetButton(mController,SDL_GameControllerButton(i));
	}


	// �g���K�[
	mState.Controller.mLeftTrigger =
		Filter1D(SDL_GameControllerGetAxis(mController,	SDL_CONTROLLER_AXIS_TRIGGERLEFT));
	mState.Controller.mRightTrigger =
		Filter1D(SDL_GameControllerGetAxis(mController,	SDL_CONTROLLER_AXIS_TRIGGERRIGHT));


	// �A�i���O�X�e�B�b�N
	x = SDL_GameControllerGetAxis(mController,	SDL_CONTROLLER_AXIS_LEFTX);
	y = -SDL_GameControllerGetAxis(mController,	SDL_CONTROLLER_AXIS_LEFTY);
	mState.Controller.mLeftStick = Filter2D(x, y);

	x = SDL_GameControllerGetAxis(mController,	SDL_CONTROLLER_AXIS_RIGHTX);
	y = -SDL_GameControllerGetAxis(mController,	SDL_CONTROLLER_AXIS_RIGHTY);
	mState.Controller.mRightStick = Filter2D(x, y);
}

void InputSystem::ProcessEvent(SDL_Event& event)
{
	switch (event.type)
	{
	case SDL_MOUSEWHEEL:
		mState.Mouse.mScrollWheel = Vector2(
			static_cast<float>(event.wheel.x),
			static_cast<float>(event.wheel.y));
		break;
	default:
		break;
	}

}



float InputSystem::Filter1D(int input)
{
	// �f�b�h�]�[�������̒l�͂O���Ɖ���
	const int deadZone = 250;
	// �ő�l�𒴂���l�͂P�O�O���Ɖ���
	const int maxValue = 30000;

	float retVal = 0.0f;

	// ���͂̐�Βl�����
	int absValue = input > 0 ? input : -input;
	// �f�b�h�]�[�������̒l�͖���
	if (absValue > deadZone)
	{
		// �f�b�h�]�[���ƍő�l�̊Ԃɂ���l�̏����l���v�Z
		retVal = static_cast<float>(absValue - deadZone) /
			(maxValue - deadZone);
		// ���������̒l�ɍ��킹��
		retVal = input > 0 ? retVal : -1.0f * retVal;
		// �l��-1.0f����1.0f�̊ԂɎ��߂�
		retVal = Math::Clamp(retVal, -1.0f, 1.0f);
	}

	return retVal;
}

Vector2 InputSystem::Filter2D(int inputX, int inputY)
{
	const float deadZone = 8000.0f;
	const float maxValue = 30000.0f;

	// 2�����x�N�g�����쐬
	Vector2 dir;
	dir.x = static_cast<float>(inputX);
	dir.y = static_cast<float>(inputY);

	float length = dir.Length();

	// ���� length < deadZone�̂Ƃ�,���͂Ȃ��Ƃ݂Ȃ�
	if (length < deadZone)
	{
		dir = Vector2::Zero;
	}
	else
	{
		// �f�b�h�]�[���ƍő�l�̓��S�~�Ԃ̏������v�Z
		float f = (length - deadZone) / (maxValue - deadZone);
		// f �� 0.0f �� 1.0f�̊ԂɎ��߂�
		f = Math::Clamp(f, 0.0f, 1.0f);
		// �x�N�g���𐳋K�����A�����l�ɃX�P�[�����O
		dir *= f / length;
	}

	return dir;
}

