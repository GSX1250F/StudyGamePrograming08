Imports OpenTK.Windowing.GraphicsLibraryFramework
Imports OpenTK.Mathematics

Public Class InputComponent
	Inherits MoveComponent

	' 前進・回転方向の力の最大値
	Private mMaxForwardForce As Double
	Private mMaxRotForce As Double
	Private mMaxForwardVelocity As Double
	Private mMaxRotSpeed As Double
	Private mFwdKey As Integer
	Private mBwdKey As Integer
	Private mCwsKey As Integer
	Private mCCwsKey As Integer
	Sub New(ByRef owner As Actor, ByVal updateOrder As Integer)
		MyBase.New(owner, updateOrder)
		mMaxForwardVelocity = 0.0
		mMaxRotSpeed = 0.0
		mMaxForwardForce = 0.0
		mMaxRotForce = 0.0
	End Sub
	Public Overrides Sub ProcessInput(ByVal keyState As KeyboardState)
		Dim fwd As Double = 0.0
		Dim rot As Double = 0.0
		If keyState.IsKeyDown(mFwdKey) = True Then
			'fwd = mMaxForwardVelocity       '単純移動の場合
			fwd = mMaxForwardForce      'ニュートン力学を使う場合
		End If
		If keyState.IsKeyDown(mBwdKey) = True Then
			'fwd = -mMaxForwardVelocity       '単純移動の場合
			fwd = -mMaxForwardForce      'ニュートン力学を使う場合
		End If
		If keyState.IsKeyDown(mCCwsKey) = True Then
			'rot = mMaxRotSpeed       '単純移動の場合
			rot = mMaxRotForce      'ニュートン力学を使う場合
		End If
		If keyState.IsKeyDown(mCwsKey) = True Then
			'rot = -mMaxRotSpeed       '単純移動の場合
			rot = -mMaxRotForce      'ニュートン力学を使う場合
		End If

		'単純移動の場合
		'SetVelocity(fwd * mOwner.GetForward())
		'SetRotSpeed(rot)

		'ニュートン力学を使う場合
		SetMoveForce(fwd * mOwner.GetForward())
		SetRotForce(rot)
	End Sub
	Public Function GetForwardKey() As Integer
		Return mFwdKey
	End Function
	Public Sub SetForwardKey(ByVal key As Integer)
		mFwdKey = key
	End Sub
	Public Function GetBackwardKey() As Integer
		Return mBwdKey
	End Function
	Public Sub SetBackwardKey(ByVal key As Integer)
		mBwdKey = key
	End Sub
	Public Function GetClockwiseKey() As Integer
		Return mCwsKey
	End Function
	Public Sub SetClockwiseKey(ByVal key As Integer)
		mCwsKey = key
	End Sub
	Public Function GetCounterClockwiseKey() As Integer
		Return mCCwsKey
	End Function
	Public Sub SetCounterClockwiseKey(ByVal key As Integer)
		mCCwsKey = key
	End Sub
	Public Sub SetMaxForwardVelocity(ByVal value As Double)
		mMaxForwardVelocity = value
	End Sub
	Public Sub SetMaxRotSpeed(ByVal value As Double)
		mMaxRotSpeed = value
	End Sub
	Public Sub SetMaxForwardForce(ByVal value As Double)
		mMaxForwardForce = value
	End Sub
	Public Sub SetMaxRotForce(ByVal value As Double)
		mMaxRotForce = value
	End Sub




End Class