Imports OpenTK.Windowing.GraphicsLibraryFramework
Imports OpenTK.Input


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
		If (inputState.Keyboard.IsKeyDown(Keys.Up) Or
			inputState.Keyboard.IsKeyPressed(Keys.Up)) Then
			Return 1.0
		ElseIf (inputState.Keyboard.IsKeyDown(Keys.Down) Or
				inputState.Keyboard.IsKeyPressed(Keys.Down)) Then
			Return -1.0
		ElseIf (inputState.Mouse.ScrollDelta.Y <> 0.0) Then
			Return inputState.Mouse.ScrollDelta.Y * 5.0
		Else
			Return 0.0
		End If
	End Function

	Public Function GetRotRatio(ByRef inputState As InputState) As Double
		If (inputState.Keyboard.IsKeyDown(Keys.Left) Or
			inputState.Keyboard.IsKeyPressed(Keys.Left)) Then
			Return 1.0
		ElseIf (inputState.Keyboard.IsKeyDown(Keys.RIGHT) Or
				inputState.Keyboard.IsKeyPressed(Keys.Right)) Then
			Return -1.0
		ElseIf (inputState.Mouse.Delta.X <> 0.0) Then
			Return -inputState.Mouse.Delta.X * 0.05
		Else
			Return 0.0
		End If
	End Function

	'private:
	' 前進・回転方向の力の最大値
	Private mMaxForwardForce As Double
	Private mMaxRotForce As Double
	Private mMaxForwardVelocity As Double
	Private mMaxRotSpeed As Double

End Class