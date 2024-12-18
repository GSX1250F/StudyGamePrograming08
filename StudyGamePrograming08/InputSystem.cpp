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
	else // １つ前の状態は 1 になる。
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
	else // １つ前の状態は 1 になる。
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
	else // １つ前の状態は 1 になる。
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
	// キーボード
	mState.Keyboard.mCurrState = SDL_GetKeyboardState(NULL);	// 現在の状態を示すポインタを保存
	memset(mState.Keyboard.mPrevState, 0, SDL_NUM_SCANCODES);	// 前回の状態を示すメモリをクリア

	// マウス
	mState.Mouse.mCurrButtons = 0;
	mState.Mouse.mPrevButtons = 0;

	// コントローラが接続されていたらそれを取得
	mController = SDL_GameControllerOpen(0);
	mState.Controller.mIsConnected = (mController != nullptr);	// コントローラ状態を初期化
	memset(mState.Controller.mCurrButtons, 0, SDL_CONTROLLER_BUTTON_MAX);
	memset(mState.Controller.mPrevButtons, 0, SDL_CONTROLLER_BUTTON_MAX);

	return true;
}

void InputSystem::Shutdown()
{
}

void InputSystem::PrepareForUpdate()
{
	// 現在の状態を１つ前の状態にコピーする。
	// キーボード
	memcpy(mState.Keyboard.mPrevState, mState.Keyboard.mCurrState, SDL_NUM_SCANCODES);
	// マウス
	mState.Mouse.mPrevButtons = mState.Mouse.mCurrButtons;
	mState.Mouse.mIsRelative = false;
	// マウスホイールイベントはホイールが動いたフレームだけでトリガーされるので、クリアしておく
	mState.Mouse.mScrollWheel = Vector2::Zero;

	// コントローラ
	memcpy(mState.Controller.mPrevButtons,
		   mState.Controller.mCurrButtons,
		   SDL_CONTROLLER_BUTTON_MAX);

}

void InputSystem::Update()
{
	// マウス
	int x = 0, y = 0;
	if (mState.Mouse.mIsRelative)
	{
		// 相対モードのとき
		mState.Mouse.mCurrButtons =	SDL_GetRelativeMouseState(&x, &y);
	}
	else
	{
		// 絶対モードのとき
		mState.Mouse.mCurrButtons =	SDL_GetMouseState(&x, &y);
	}
	mState.Mouse.mMousePos.x = static_cast<float>(x);
	mState.Mouse.mMousePos.y = static_cast<float>(y);

	// コントローラ
	// ボタン
	for (int i = 0; i < SDL_CONTROLLER_BUTTON_MAX; i++)
	{
		mState.Controller.mCurrButtons[i] =
			SDL_GameControllerGetButton(mController,SDL_GameControllerButton(i));
	}


	// トリガー
	mState.Controller.mLeftTrigger =
		Filter1D(SDL_GameControllerGetAxis(mController,	SDL_CONTROLLER_AXIS_TRIGGERLEFT));
	mState.Controller.mRightTrigger =
		Filter1D(SDL_GameControllerGetAxis(mController,	SDL_CONTROLLER_AXIS_TRIGGERRIGHT));


	// アナログスティック
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
	// デッドゾーン未満の値は０％と解釈
	const int deadZone = 250;
	// 最大値を超える値は１００％と解釈
	const int maxValue = 30000;

	float retVal = 0.0f;

	// 入力の絶対値を取る
	int absValue = input > 0 ? input : -input;
	// デッドゾーン未満の値は無視
	if (absValue > deadZone)
	{
		// デッドゾーンと最大値の間にある値の小数値を計算
		retVal = static_cast<float>(absValue - deadZone) /
			(maxValue - deadZone);
		// 符号を元の値に合わせる
		retVal = input > 0 ? retVal : -1.0f * retVal;
		// 値を-1.0fから1.0fの間に収める
		retVal = Math::Clamp(retVal, -1.0f, 1.0f);
	}

	return retVal;
}

Vector2 InputSystem::Filter2D(int inputX, int inputY)
{
	const float deadZone = 8000.0f;
	const float maxValue = 30000.0f;

	// 2次元ベクトルを作成
	Vector2 dir;
	dir.x = static_cast<float>(inputX);
	dir.y = static_cast<float>(inputY);

	float length = dir.Length();

	// もし length < deadZoneのとき,入力なしとみなす
	if (length < deadZone)
	{
		dir = Vector2::Zero;
	}
	else
	{
		// デッドゾーンと最大値の同心円間の小数を計算
		float f = (length - deadZone) / (maxValue - deadZone);
		// f を 0.0f と 1.0fの間に収める
		f = Math::Clamp(f, 0.0f, 1.0f);
		// ベクトルを正規化し、小数値にスケーリング
		dir *= f / length;
	}

	return dir;
}

