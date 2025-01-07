Imports OpenTK.Windowing.GraphicsLibraryFramework
Imports OpenTK.Input
Imports OpenTK.Mathematics
Imports OpenTK.Graphics.OpenGL.GL
Imports OpenTK.Windowing.Common
Imports OpenTK.Windowing.Common.Input
Public Enum Direction
	Forward
	Backward
	Clockwise
	CounterClockwise
End Enum
Public Enum InputDevice
	Mouse_L_Button
	Mouse_R_Button
	Mouse_MoveUp
	Mouse_MoveDown
	Mouse_MoveLeft
	Mouse_MoveRight
	Mouse_ScrollUp
	Mouse_ScrollDown
	Controller_A_Button
	Controller_B_Button
	Controller_X_Button
	Controller_Y_Button
	Controller_L1_Button
	Controller_R1_Button
	Controller_Dpad_Up
	Controller_Dpad_Down
	Controller_Dpad_Left
	Controller_Dpad_Right
	Controller_L2_Trigger
	Controller_R2_Trigger
	Controller_L_Stick_TiltLeft
	Controller_L_Stick_TiltRight
	Controller_L_Stick_TiltUp
	Controller_L_Stick_TiltDown
	Controller_R_Stick_TiltLeft
	Controller_R_Stick_TiltRight
	Controller_R_Stick_TiltUp
	Controller_R_Stick_TiltDown
End Enum

Public Structure KeyConfig
	Dim dir As Direction
	Dim input As Keys
End Structure

Public Structure InputDeviceConfig
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

		''単純移動の場合
		'fwd = mMaxForwardVelocity * GetForwardRatio(inputState)
		'rot = -mMaxRotSpeed * GetRotationRatio(inputState)
		'SetVelocity(fwd * mOwner.GetForward())
		'SetRotSpeed(rot * Vector3.UnitZ)

		'ニュートン力学を使う場合
		fwd = mMaxForwardForce * GetForwardRatio(inputState)
		rot = -mMaxRotForce * GetRotationRatio(inputState)
		SetForce(fwd * mOwner.GetForward())
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
		'キーボード
		For Each keyConfig In mKeyConfigs
			If (inputState.Keyboard.GetKeyValue(keyConfig.input)) Then
				Select Case keyConfig.dir
					Case Direction.Forward
						ratio += 1.0
					Case Direction.Backward
						ratio -= 1.0
				End Select
			End If
		Next
		'マウスとコントローラ
		For Each config In mInputDeviceConfigs
			Select Case config.dir
				Case Direction.Forward
					ratio += CalcRatio(config, inputState)
				Case Direction.Backward
					ratio -= CalcRatio(config, inputState)
			End Select
		Next
		Return ratio
	End Function
	Public Function GetRotationRatio(ByRef inputState As InputState) As Double
		Dim ratio As Double = 0.0
		'キーボード
		For Each keyConfig In mKeyConfigs
			If (inputState.Keyboard.GetKeyValue(keyConfig.input)) Then
				Select Case keyConfig.dir
					Case Direction.Clockwise
						ratio += 1.0
					Case Direction.CounterClockwise
						ratio -= 1.0
				End Select
			End If
		Next
		'マウスとコントローラ
		For Each config In mInputDeviceConfigs
			Select Case config.dir
				Case Direction.Clockwise
					ratio += CalcRatio(config, inputState)
				Case Direction.CounterClockwise
					ratio -= CalcRatio(config, inputState)
			End Select
		Next
		Return ratio
	End Function
	Public Sub SetKeyConfig(ByVal dir As Direction, ByVal input As Keys)
		Dim config As KeyConfig
		config.dir = dir
		config.input = input
		mKeyConfigs.Add(config)
	End Sub
	Public Sub SetInputDeviceConfig(ByVal dir As Direction, ByVal input As Keys, ByVal ratio As Double)
		Dim config As InputDeviceConfig
		config.dir = dir
		config.input = input
		config.ratio = ratio
		mInputDeviceConfigs.Add(config)
	End Sub
	Public Function CalcRatio(ByRef config As InputDeviceConfig, ByRef inputState As InputState)
		Dim ratio As Double = 0.0
		Select Case config.input
			Case InputDevice.Mouse_L_Button
				If inputState.Mouse.GetButtonValue(MouseButton.Left) Then
					ratio = config.ratio
				End If
			Case InputDevice.Mouse_R_Button
				If inputState.Mouse.GetButtonState(MouseButton.Right) Then
					ratio = config.ratio
				End If
			Case InputDevice.Mouse_MoveUp
				'相対モード
				If inputState.Mouse.GetIsRelative() Then
					ratio = inputState.Mouse.GetPosition.Y * config.ratio
				End If
			Case InputDevice.Mouse_MoveDown
				If inputState.Mouse.GetIsRelative() Then
					ratio = -inputState.Mouse.GetPosition.Y * config.ratio
				End If
			Case InputDevice.Mouse_MoveLeft
				If inputState.Mouse.GetIsRelative() Then
					ratio = -inputState.Mouse.GetPosition.X * config.ratio
				End If
			Case InputDevice.Mouse_MoveRight
				If inputState.Mouse.GetIsRelative() Then
					ratio = inputState.Mouse.GetPosition.X * config.ratio
				End If
			Case InputDevice.Mouse_ScrollUp
				ratio = inputState.Mouse.GetScrollWheel.Y * config.ratio
			Case InputDevice.Mouse_ScrollDown
				ratio = -inputState.Mouse.GetScrollWheel.Y * config.ratio
			Case InputDevice.Controller_Dpad_Up
				If inputState.Controller.GetButtonValue(ControllerButton.Dpad_Up) Then
					ratio = config.ratio
				End If
			Case InputDevice.Controller_Dpad_Down
				If inputState.Controller.GetButtonValue(ControllerButton.Dpad_Down) Then
					ratio = config.ratio
				End If
			Case InputDevice.Controller_Dpad_Left
				If inputState.Controller.GetButtonValue(ControllerButton.Dpad_Left) Then
					ratio = config.ratio
				End If
			Case InputDevice.Controller_Dpad_Right
				If inputState.Controller.GetButtonValue(ControllerButton.Dpad_Right) Then
					ratio = config.ratio
				End If
			Case InputDevice.Controller_A_Button
				If inputState.Controller.GetButtonValue(ControllerButton.A) Then
					ratio = config.ratio
				End If
			Case InputDevice.Controller_B_Button
				If inputState.Controller.GetButtonValue(ControllerButton.B) Then
					ratio = config.ratio
				End If
			Case InputDevice.Controller_X_Button
				If inputState.Controller.GetButtonValue(ControllerButton.X) Then
					ratio = config.ratio
				End If
			Case InputDevice.Controller_Y_Button
				If inputState.Controller.GetButtonValue(ControllerButton.Y) Then
					ratio = config.ratio
				End If
			Case InputDevice.Controller_L1_Button
				If inputState.Controller.GetButtonValue(ControllerButton.L1) Then
					ratio = config.ratio
				End If
			Case InputDevice.Controller_R1_Button
				If inputState.Controller.GetButtonValue(ControllerButton.R1) Then
					ratio = config.ratio
				End If
			Case InputDevice.Controller_L2_Trigger
				ratio = inputState.Controller.GetAxis(ControllerAnalog.L2_trigger) * config.ratio
			Case InputDevice.Controller_R2_Trigger
				ratio = inputState.Controller.GetAxis(ControllerAnalog.R2_trigger) * config.ratio
			Case InputDevice.Controller_L_Stick_TiltUp
				ratio = inputState.Controller.GetAxis(ControllerAnalog.L_stick_Y) * config.ratio
			Case InputDevice.Controller_L_Stick_TiltDown
				ratio = -inputState.Controller.GetAxis(ControllerAnalog.L_stick_Y) * config.ratio
			Case InputDevice.Controller_L_Stick_TiltLeft
				ratio = -inputState.Controller.GetAxis(ControllerAnalog.L_stick_X) * config.ratio
			Case InputDevice.Controller_L_Stick_TiltRight
				ratio = inputState.Controller.GetAxis(ControllerAnalog.L_stick_X) * config.ratio
			Case InputDevice.Controller_R_Stick_TiltUp
				ratio = inputState.Controller.GetAxis(ControllerAnalog.R_stick_Y) * config.ratio
			Case InputDevice.Controller_R_Stick_TiltDown
				ratio = -inputState.Controller.GetAxis(ControllerAnalog.R_stick_Y) * config.ratio
			Case InputDevice.Controller_R_Stick_TiltLeft
				ratio = -inputState.Controller.GetAxis(ControllerAnalog.R_stick_X) * config.ratio
			Case InputDevice.Controller_R_Stick_TiltRight
				ratio = inputState.Controller.GetAxis(ControllerAnalog.R_stick_X) * config.ratio
		End Select

		If ratio < 0.001 Or ratio < 0 Then
			ratio = 0.0
		End If

		Return ratio
	End Function

	'private:
	' 前進・回転方向の力の最大値
	Private mMaxForwardForce As Double
	Private mMaxRotForce As Double
	Private mMaxForwardVelocity As Double
	Private mMaxRotSpeed As Double
	Private mKeyConfigs As New List(Of KeyConfig)
	Private mInputDeviceConfigs As New List(Of InputDeviceConfig)
End Class