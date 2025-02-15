﻿Imports System.Security.Cryptography
Imports OpenTK.Mathematics
Imports OpenTK.Windowing.GraphicsLibraryFramework

Public Class Ship
    Inherits Actor
    'public:
    Sub New(ByRef game As Game)
        MyBase.New(game)
        mSSC = New SomeSpriteComponent(Me, 30)
        mSSC.TextureFiles = New List(Of String) From {
            "Assets\Ship01.png",
            "Assets\Ship02.png",
            "Assets\Ship03.png",
            "Assets\Ship04.png",
            "Assets\Ship05.png"}
        mSSC.SetSomeTextures(mSSC.TextureFiles)

        mIC = New InputComponent(Me, 10)
        mIC.SetMaxForwardVelocity(250.0)
        mIC.SetMaxRotSpeed(10.0)
        mIC.SetMaxForwardForce(300.0)
        mIC.SetMaxRotForce(150.0)
        mIC.SetMoveResist(20.0)
        mIC.SetRotResist(15.0)
        mIC.SetMass(1.0)
        mIC.SetKeyConfig(Direction.Forward, Keys.Up)
        mIC.SetKeyConfig(Direction.Backward, Keys.Down)
        mIC.SetKeyConfig(Direction.Clockwise, Keys.Right)
        mIC.SetKeyConfig(Direction.CounterClockwise, Keys.Left)
        mIC.SetInputDeviceConfig(Direction.Forward, InputDevice.Mouse_ScrollUp, 3.0)
        mIC.SetInputDeviceConfig(Direction.Backward, InputDevice.Mouse_ScrollDown, 3.0)
        mIC.SetInputDeviceConfig(Direction.Clockwise, InputDevice.Mouse_MoveRight, 0.03)
        mIC.SetInputDeviceConfig(Direction.CounterClockwise, InputDevice.Mouse_MoveLeft, 0.03)
        mIC.SetInputDeviceConfig(Direction.Forward, InputDevice.Controller_Dpad_Up, 1.0)
        mIC.SetInputDeviceConfig(Direction.Backward, InputDevice.Controller_Dpad_Down, 1.0)
        mIC.SetInputDeviceConfig(Direction.Clockwise, InputDevice.Controller_Dpad_Right, 1.0)
        mIC.SetInputDeviceConfig(Direction.CounterClockwise, InputDevice.Controller_Dpad_Left, 1.0)
        mIC.SetInputDeviceConfig(Direction.Forward, InputDevice.Controller_L_Stick_TiltUp, 1.0)
        mIC.SetInputDeviceConfig(Direction.Backward, InputDevice.Controller_L_Stick_TiltDown, 1.0)
        mIC.SetInputDeviceConfig(Direction.Clockwise, InputDevice.Controller_R_Stick_TiltRight, 1.0)
        mIC.SetInputDeviceConfig(Direction.CounterClockwise, InputDevice.Controller_R_Stick_TiltLeft, 1.0)

        mCircle = New CircleComponent(Me, 10)

        '効果音生成
        mChunkFiles.AddRange(
            {"Assets/thruster.mp3",
             "Assets/beam.mp3",
             "Assets/explosion.mp3"})
        For Each file In mChunkFiles
            game.GetSoundPlayer().AddChunk(file)
        Next

        Init()
    End Sub
    Public Overrides Sub ActorInput(ByRef inputState As InputState)
        If mCrash = False Then
            If mIC.GetRotationRatio(inputState) < 0 Then
                mSSC.SelectTexture(mSSC.TextureFiles(1))
                GetGame().GetSoundPlayer().SetChunkControl(mChunkFiles(0), "play")
            ElseIf mIC.GetRotationRatio(inputState) > 0 Then
                mSSC.SelectTexture(mSSC.TextureFiles(2))
                GetGame().GetSoundPlayer().SetChunkControl(mChunkFiles(0), "play")
            ElseIf mIC.GetForwardRatio(inputState) > 0 Then
                mSSC.SelectTexture(mSSC.TextureFiles(3))
                GetGame().GetSoundPlayer().SetChunkControl(mChunkFiles(0), "play")
            ElseIf mIC.GetForwardRatio(inputState) < 0 Then
                mSSC.SelectTexture(mSSC.TextureFiles(4))
                GetGame().GetSoundPlayer().SetChunkControl(mChunkFiles(0), "play")
            Else
                mSSC.SelectTexture(mSSC.TextureFiles(0))
            End If

            If ((inputState.Keyboard.GetKeyState(Keys.Space) = ButtonState.EPressed Or
                 inputState.Mouse.GetButtonState(MouseButton.Left) = ButtonState.EPressed Or
                 inputState.Mouse.GetButtonState(MouseButton.Right) = ButtonState.EPressed Or
                 inputState.Controller.GetButtonState(ControllerButton.X) = ButtonState.EPressed Or
                 inputState.Controller.GetButtonState(ControllerButton.A) = ButtonState.EPressed Or
                 inputState.Controller.GetButtonState(ControllerButton.L1) = ButtonState.EPressed Or
                 inputState.Controller.GetButtonState(ControllerButton.R1) = ButtonState.EPressed) And
                 (mLaserCooldown <= 0.0)) Then
                ' レーザーオブジェクトを作成、位置と回転角を宇宙船とあわせる。
                Dim laser As New Laser(GetGame())
                laser.SetPosition(GetPosition() + GetRadius() * GetForward())
                laser.SetRotation(GetRotation())
                laser.Shot()
                ' レーザー冷却期間リセット
                mLaserCooldown = 0.7
                GetGame().GetSoundPlayer().SetChunkControl(mChunkFiles(1), "replay")
            End If

        End If
    End Sub
    Public Overrides Sub UpdateActor(ByVal deltaTime As Double)
        mLaserCooldown -= deltaTime
        mCrashCooldown -= deltaTime
        mCrashingTime -= deltaTime

        If (mCrash = False) Then
            '画面外にでたら反対の位置に移動（ラッピング処理）
            If (GetPosition().X < GetGame().mWindowWidth * (-0.5) - GetRadius() Or
                GetPosition().X > GetGame().mWindowWidth * 0.5 + GetRadius()) _
                Then
                SetPosition(New Vector3(-GetPosition().X, GetPosition().Y, GetPosition().Z))
            End If

            If (GetPosition().Y < GetGame().mWindowHeight * (-0.5) - GetRadius() Or
                GetPosition().Y > GetGame().mWindowHeight * 0.5 + GetRadius()) _
                Then
                SetPosition(New Vector3(GetPosition().X, -GetPosition().Y, GetPosition().Z))
            End If

            '小惑星と衝突したかを判定
            For Each ast In GetGame().GetAsteroids()
                If Intersect(mCircle, ast.GetCircle()) And ast.GetState() = State.EActive Then
                    mCrashPos = GetPosition()
                    mCrashRot = GetRotation()
                    mCrash = True
                    mCrashCooldown = 4.0
                    mCrashingTime = 2.0
                    GetGame().GetSoundPlayer().SetChunkControl(mChunkFiles(2), "play")
                    Exit For
                End If
            Next
        Else
            If mCrashingTime > 0.0 Then
                '衝突時の演出。
                SetPosition(mCrashPos)       'MoveComponentが更新されても衝突したときの位置に置きなおし
                Dim inc = Quaternion.FromAxisAngle(Vector3.UnitZ, -3.0 * 2.0 * Math.PI * deltaTime)
                mCrashRot = Quaternion.Multiply(mCrashRot, inc)
                SetRotation(mCrashRot)       'MoveComponentが更新されても衝突してからの回転角度に置きなおし
                SetScale(GetScale() * 0.98)
            Else
                If mCrashCooldown > 0.0 Then
                    '衝突演出後、リスポーンするまで表示停止
                    SetState(State.EPaused)
                    mSSC.SetVisible(False)
                Else
                    Init()
                End If

            End If
        End If
    End Sub
    Public Sub Init()
        SetScale(0.8)
        SetPosition(Vector3.Zero)
        SetRotation(Quaternion.FromAxisAngle(-1.0 * Vector3.UnitZ, RandomNumberGenerator.GetInt32(0, 1000) * 0.01 * Math.PI * 2.0))        'SetRotation(0.0)
        mIC.SetVelocity(Vector3.Zero)
        mIC.SetRotSpeed(Vector3.Zero)
        SetState(State.EActive)
        mSSC.SetVisible(True)

        mLaserCooldown = 0.0
        mCrashCooldown = 0.0
        mCrashingTime = 0.0
        mCrash = False
    End Sub

    'private:
    Private mLaserCooldown As Double     'レーザーを次に撃てるまでの時間
    Private mCrash As Boolean        '衝突検知
    Private mCrashingTime As Double     '衝突演出時間
    Private mCrashCooldown As Double  '衝突演出後、リセットされるまでスプライトを消す時間
    Private mCrashPos As Vector3     '衝突時の位置
    Private mCrashRot As Quaternion     '衝突時の向き
    Private mCircle As CircleComponent
    Private mSSC As SomeSpriteComponent
    Private mIC As InputComponent
    Private mChunkFiles As New List(Of String)
End Class