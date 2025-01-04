Imports OpenTK.Windowing.GraphicsLibraryFramework
Imports OpenTK.Input
Imports OpenTK.Mathematics

Enum Direction
	Forward
	Backward
	Clockwise
	CounterClockwise
End Enum

Enum InputDevice
	Mouse_L_Button
	Mouse_R_Button
	Mouse_MoveUp
	Mouse_MoveDown
	Mouse_MoveLeft
	Mouse_MoveRight
	Mouse_ScrollUp
	Mouse_ScrollDown
	Controller_Dpad_Up
	Controller_Dpad_Down
	Controller_Dpad_Left
	Controller_Dpad_Right
	Controller_X_Button
	Controller_Y_Button
	Controller_A_Button
	Controller_B_Button
	Controller_L_Button
	Controller_R_Button
	Controller_L_Trigger
	Controller_R_Trigger
	Controller_L_Stick_TiltUp
	Controller_L_Stick_TiltDown
	Controller_L_Stick_TiltLeft
	Controller_L_Stick_TiltRight
	Controller_R_Stick_TiltUp
	Controller_R_Stick_TiltDown
	Controller_R_Stick_TiltLeft
	Controller_R_Stick_TiltRight
End Enum

Structure KeyConfig
	Dim dir As Direction
	Dim input As Keys
End Structure

Structure InputDeviceConfig
	Dim dir As Direction
	Dim input As InputDevice
	Dim ratio As Double
End Structure

Public Class InputComponent
	Inherits MoveComponent

	'public:
	Sub New(ByRef owner As Actor, ByVal updateOrder As Integer)
		MyBase.New(owner, updateOrder)
		mMaxForwardVelocity = 0.0
		mMaxRotSpeed = 0.0
		mMaxForwardForce = 0.0
		mMaxRotForce = 0.0
	End Sub
	Public Overrides Sub ProcessInput(ByRef inputState As InputState)
		Dim fwd As Double = 0.0
		Dim rot As Double = 0.0
		fwd = mMaxForwardForce * GetForwardRatio(inputState)
		rot = -mMaxRotForce * GetRotationRatio(inputState)

		'ニュートン力学を使う場合
		SetMoveForce(fwd * mOwner.GetForward())
		SetRotForce(rot * Vector3.UnitZ)
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

	Public Function GetForwardRatio(ByRef inputState As InputState) As Double
		Dim ratio As Double = 0.0
		For Each keys In mForwardKey
			If (inputState.Keyboard.IsKeyDown(keys.Key) Or
				inputState.Keyboard.IsKeyPressed(keys.Key)) Then
				ratio = mForwardKey(keys.Key)
			End If
		Next
		For Each keys In mForwardMouseButton
			If (inputState.Mouse.IsButtonDown(keys.Key) Or
				inputState.Mouse.IsButtonPressed(keys.Key)) Then
				ratio = mForwardKey(keys.Key)
			End If
		Next
		For Each keys In mForwardMousePos
			If Vector2.Dot(inputState.Mouse.Delta, keys.Key) * keys.Value <> 0.0 Then
				ratio = Vector2.Dot(inputState.Mouse.Delta, keys.Key) * keys.Value
			End If
		Next
		For Each keys In mForwardMouseScroll
			If Vector2.Dot(inputState.Mouse.ScrollDelta, keys.Key) * keys.Value <> 0.0 Then
				ratio = Vector2.Dot(inputState.Mouse.ScrollDelta, keys.Key) * keys.Value
			End If
		Next
		Return ratio
	End Function

	Public Function GetRotationRatio(ByRef inputState As InputState) As Double
		Dim ratio As Double = 0.0
		For Each keys In mRotationKey
			If (inputState.Keyboard.IsKeyDown(keys.Key) Or
				inputState.Keyboard.IsKeyPressed(keys.Key)) Then
				ratio = keys.Value
			End If
		Next
		For Each keys In mRotationMouseButton
			If (inputState.Mouse.IsButtonDown(keys.Key) Or
				inputState.Mouse.IsButtonPressed(keys.Key)) Then
				ratio = keys.Value
			End If
		Next
		For Each keys In mRotationMousePos
			If Vector2.Dot(inputState.Mouse.Delta, keys.Key) * keys.Value <> 0.0 Then
				ratio = Vector2.Dot(inputState.Mouse.Delta, keys.Key) * keys.Value
			End If
		Next
		For Each keys In mRotationMouseScroll
			If Vector2.Dot(inputState.Mouse.ScrollDelta, keys.Key) * keys.Value <> 0.0 Then
				ratio = Vector2.Dot(inputState.Mouse.ScrollDelta, keys.Key) * keys.Value
			End If
		Next
		Return ratio
	End Function

	Public Sub SetForwardKey(ByVal key As Keys, ByVal ratio As Double)
		mForwardKey.Add(key, ratio)
	End Sub
	Public Sub SetRotationKey(ByVal key As Keys, ByVal ratio As Double)
		mRotationKey.Add(key, ratio)
	End Sub
	Public Sub SetForwardMouseButton(ByVal button As MouseButton, ByVal ratio As Double)
		mForwardMouseButton.Add(button, ratio)
	End Sub
	Public Sub SetRotationMouseButton(ByVal button As MouseButton, ByVal ratio As Double)
		mRotationMouseButton.Add(button, ratio)
	End Sub

	Public Sub SetForwardMousePos(ByVal v As Vector2, ByVal ratio As Double)
		mForwardMousePos.Add(v, ratio)
	End Sub
	Public Sub SetRotationMousePos(ByVal v As Vector2, ByVal ratio As Double)
		mRotationMousePos.Add(v, ratio)
	End Sub
	Public Sub SetForwardMouseScroll(ByVal v As Vector2, ByVal ratio As Double)
		mForwardMouseScroll.Add(v, ratio)
	End Sub
	Public Sub SetRotationMouseScroll(ByVal v As Vector2, ByVal ratio As Double)
		mRotationMouseScroll.Add(v, ratio)
	End Sub


	'private:
	' 前進・回転方向の力の最大値
	Private mMaxForwardForce As Double
	Private mMaxRotForce As Double
	Private mMaxForwardVelocity As Double
	Private mMaxRotSpeed As Double
	Private mForwardKey As New Dictionary(Of Keys, Double)
	Private mRotationKey As New Dictionary(Of Keys, Double)
	Private mForwardMouseButton As New Dictionary(Of MouseButton, Double)
	Private mRotationMouseButton As New Dictionary(Of MouseButton, Double)
	Private mForwardMousePos As New Dictionary(Of Vector2, Double)
	Private mRotationMousePos As New Dictionary(Of Vector2, Double)
	Private mForwardMouseScroll As New Dictionary(Of Vector2, Double)
	Private mRotationMouseScroll As New Dictionary(Of Vector2, Double)
End Class