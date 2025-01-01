Imports OpenTK.Windowing.GraphicsLibraryFramework
Imports OpenTK.Input
Imports OpenTK.Mathematics


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
		rot = mMaxRotForce * GetRotRatio(inputState)

		'ニュートン力学を使う場合
		SetMoveForce(fwd * mOwner.GetForward())
		SetRotForce(rot)
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
		For Each keys In mForwardMouse
			If Vector2.Dot(inputState.Mouse.Delta, keys.Key) * keys.Value <> 0.0 Then
				ratio = Vector2.Dot(inputState.Mouse.Delta, keys.Key) * keys.Value
			End If
		Next
		For Each keys In mForwardScroll
			If Vector2.Dot(inputState.Mouse.ScrollDelta, keys.Key) * keys.Value <> 0.0 Then
				ratio = Vector2.Dot(inputState.Mouse.ScrollDelta, keys.Key) * keys.Value
			End If
		Next
		Return ratio
	End Function

	Public Function GetRotRatio(ByRef inputState As InputState) As Double
		Dim ratio As Double = 0.0
		For Each keys In mRotationKey
			If (inputState.Keyboard.IsKeyDown(keys.Key) Or
				inputState.Keyboard.IsKeyPressed(keys.Key)) Then
				ratio = keys.Value
			End If
		Next
		For Each keys In mRotationMouse
			If Vector2.Dot(inputState.Mouse.Delta, keys.Key) * keys.Value <> 0.0 Then
				ratio = Vector2.Dot(inputState.Mouse.Delta, keys.Key) * keys.Value
			End If
		Next
		For Each keys In mRotationScroll
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
	Public Sub SetForwardMouse(ByVal v As Vector2, ByVal ratio As Double)
		mForwardMouse.Add(v, ratio)
	End Sub
	Public Sub SetRotationMouse(ByVal v As Vector2, ByVal ratio As Double)
		mRotationMouse.Add(v, ratio)
	End Sub
	Public Sub SetForwardScroll(ByVal v As Vector2, ByVal ratio As Double)
		mForwardScroll.Add(v, ratio)
	End Sub
	Public Sub SetRotationScroll(ByVal v As Vector2, ByVal ratio As Double)
		mRotationScroll.Add(v, ratio)
	End Sub


	'private:
	' 前進・回転方向の力の最大値
	Private mMaxForwardForce As Double
	Private mMaxRotForce As Double
	Private mMaxForwardVelocity As Double
	Private mMaxRotSpeed As Double
	Private mForwardKey As New Dictionary(Of Keys, Double)
	Private mRotationKey As New Dictionary(Of Keys, Double)
	Private mForwardMouse As New Dictionary(Of Vector2, Double)
	Private mRotationMouse As New Dictionary(Of Vector2, Double)
	Private mForwardScroll As New Dictionary(Of Vector2, Double)
	Private mRotationScroll As New Dictionary(Of Vector2, Double)
End Class