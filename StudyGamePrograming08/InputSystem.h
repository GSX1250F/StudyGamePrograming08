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
	// InputSystemのFriendクラスとすることで容易に更新できる
	friend class InputSystem;
	// キーの真偽値のみを取得
	bool GetKeyValue(SDL_Scancode keyCode) const;
	// 現在とその１つ前のフレームから状態を取得
	ButtonState GetKeyState(SDL_Scancode keyCode) const;
private:
	const Uint8* mCurrState;	//現在の状態
	Uint8 mPrevState[SDL_NUM_SCANCODES];	//１つ前のフレームの状態
};

// マウス入力のヘルパークラス
class MouseState
{
public:
	// InputSystemのFriendクラスとすることで容易に更新できる
	friend class InputSystem;

	// マウスの位置
	const Vector2& GetPosition() const { return mMousePos; }
	// スクロールホイールの位置
	const Vector2& GetScrollWheel() const { return mScrollWheel; }
	// 相対マウスモードかどうかを取得
	bool GetIsRelative() const { return mIsRelative; }

	// マウスボタン状態
	bool GetButtonValue(int button) const;
	ButtonState GetButtonState(int button) const;
private:
	// マウス位置を保存
	Vector2 mMousePos;
	// スクロールホイールの位置を保存
	Vector2 mScrollWheel;
	// ボタンの状態を保存
	Uint32 mCurrButtons;
	Uint32 mPrevButtons;
	// 相対マウスモードか否か
	bool mIsRelative;
};

// コントローラ入力のヘルパークラス
class ControllerState
{
public:
	// InputSystemのFriendクラスとすることで容易に更新できる
	friend class InputSystem;

	// ボタンの状態
	bool GetButtonValue(SDL_GameControllerButton button) const;
	ButtonState GetButtonState(SDL_GameControllerButton button) const;
	// スティック・トリガーの状態
	const Vector2& GetLeftStick() const { return mLeftStick; }
	const Vector2& GetRightStick() const { return mRightStick; }
	float GetLeftTrigger() const { return mLeftTrigger; }
	float GetRightTrigger() const { return mRightTrigger; }
	// 接続されているかどうか
	bool GetIsConnected() const { return mIsConnected; }
private:
	// 現在と１つ前のボタンの状態を保存
	Uint8 mCurrButtons[SDL_CONTROLLER_BUTTON_MAX];
	Uint8 mPrevButtons[SDL_CONTROLLER_BUTTON_MAX];
	// Left/rightスティック
	Vector2 mLeftStick;
	Vector2 mRightStick;
	// Left/rightトリガー
	float mLeftTrigger;
	float mRightTrigger;
	// 接続されているかどうか
	bool mIsConnected;
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

