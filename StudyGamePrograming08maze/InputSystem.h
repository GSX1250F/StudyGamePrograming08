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

// キーボード入力のヘルパークラス
class KeyboardState
{
public:
	friend class InputSystem;
	bool GetKeyValue(SDL_Scancode keyCode) const;	// キーの真偽値のみを取得
	ButtonState GetKeyState(SDL_Scancode keyCode) const;	// 現在とその１つ前のフレームから状態を取得
private:
	const Uint8* mCurrState;
	Uint8 mPrevState[SDL_NUM_SCANCODES];
};

// マウス入力のヘルパークラス
class MouseState
{
public:
	friend class InputSystem;
	const Vector2& GetPosition() const { return mMousePos; }	// マウスの位置
	const Vector2& GetScrollWheel() const { return mScrollWheel; }	// スクロールホイールの位置
	bool GetIsRelative() const { return mIsRelative; }	// 相対マウスモードかどうかを取得
	bool GetButtonValue(int button) const;	// マウスボタン状態
	ButtonState GetButtonState(int button) const;	// 現在とその１つ前のフレームから状態を取得
private:
	Vector2 mMousePos;
	Vector2 mScrollWheel;
	Uint32 mCurrButtons;
	Uint32 mPrevButtons;
	bool mIsRelative;	// 相対マウスモードか否か
};

// コントローラ入力のヘルパークラス
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
	bool GetIsConnected() const { return mIsConnected; }	// 接続されているかどうか
private:
	Uint8 mCurrButtons[SDL_CONTROLLER_BUTTON_MAX];
	Uint8 mPrevButtons[SDL_CONTROLLER_BUTTON_MAX];
	Vector2 mLeftStick;
	Vector2 mRightStick;
	float mLeftTrigger;
	float mRightTrigger;
	bool mIsConnected;	// 接続されているかどうか
};

// 現在の入力状態を格納するラッパー
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

	// SDL_PolEventsループの直前に呼ばれる
	void PrepareForUpdate();
	// SDL_PolEventsループの直後に呼ばれる
	void Update();
	// InputSystemにSDLイベントを渡すために呼ばれる
	void ProcessEvent(union SDL_Event& event);

	const InputState& GetState() const { return mState; }

	// マウスの相対モードを設定(true:オン, false:オフ)
	void SetRelativeMouseMode(bool value);

private:
	InputState mState;
	// アナログスティックデッドゾーン設定（１次元）
	float Filter1D(int input);
	// アナログスティックデッドゾーン設定（２次元）
	Vector2 Filter2D(int inputX, int inputY);
	SDL_GameController* mController;
};

