Imports OpenTK.Windowing.GraphicsLibraryFramework
Imports OpenTK.Windowing.Common
Imports System.Runtime.CompilerServices
Imports OpenTK.Mathematics
Imports System.Threading.Tasks.Dataflow
Imports OpenTK.Graphics.OpenGL

Public Enum ButtonState
    ENone
    EPressed
    EHeld
    EReleased
End Enum
Public Structure InputState
    Dim Keyboard As KeyboardInputState
    Dim Mouse As MouseInputState
    Dim Controller As ControllerInputState
End Structure

Public Class KeyboardInputState
    'public:
    Public Function GetKeyValue(ByVal key As Keys) As Boolean
        Return mState.IsKeyDown(key)
    End Function
    Public Function GetKeyState(ByVal key As Keys) As ButtonState
        If (mState.WasKeyDown(key) = False) Then
            If (mState.IsKeyDown(key) = False) Then
                Return ButtonState.ENone
            Else
                Return ButtonState.EPressed
            End If
        Else
            If (mState.IsKeyDown(key) = False) Then
                Return ButtonState.EReleased
            Else
                Return ButtonState.EHeld
            End If
        End If
    End Function

    Friend mState As KeyboardState
End Class
Public Class MouseInputState
    'Public:
    Public Function GetPosition() As Vector2
        If mIsRelative Then
            Return mState.Delta
        Else
            Return mState.Position
        End If
    End Function
    Public Function GetScrollWheel() As Vector2
        Return mState.ScrollDelta
    End Function
    Public Function GetIsRelative() As Boolean
        Return mIsRelative
    End Function
    Public Function GetButtonValue(ByVal button As MouseButton) As Boolean
        Return mState.IsButtonDown(button)
    End Function
    Public Function GetButtonState(ByVal button As MouseButton) As ButtonState
        If (mState.WasButtonDown(button) = False) Then
            If (mState.IsButtonDown(button) = False) Then
                Return ButtonState.ENone
            Else
                Return ButtonState.EPressed
            End If
        Else
            If (mState.IsButtonDown(button) = False) Then
                Return ButtonState.EReleased
            Else
                Return ButtonState.EHeld
            End If
        End If
    End Function

    Friend mState As MouseState
    Friend mIsRelative As Boolean
End Class

Public Enum ControllerButton
    A = 0
    B = 1
    X = 2
    Y = 3
    L1 = 4
    R1 = 5
    Back = 6
    Start = 7
    L3 = 8
    R3 = 9
    Dpad_Up = 10
    Dpad_Right = 11
    Dpad_Down = 12
    Dpad_Left = 13
End Enum
Public Enum ControllerAnalog
    L_stick_X = 0
    L_stick_Y = 1
    R_stick_X = 2
    R_stick_Y = 3
    L2_trigger = 4
    R2_trigger = 5
End Enum

Public Class ControllerInputState
    'Public
    Public Function GetButtonValue(ByVal button As Integer) As Boolean
        If mIsConnected Then
            Return mState.IsButtonDown(button)
        Else
            Return False
        End If
    End Function
    Public Function GetButtonState(ByVal button As ControllerButton) As ButtonState
        If mIsConnected Then
            If (mState.WasButtonDown(button) = False) Then
                If (mState.IsButtonDown(button) = False) Then
                    Return ButtonState.ENone
                Else
                    Return ButtonState.EPressed
                End If
            Else
                If (mState.IsButtonDown(button) = False) Then
                    Return ButtonState.EReleased
                Else
                    Return ButtonState.EHeld
                End If
            End If
        Else
            Return ButtonState.ENone
        End If

    End Function
    Public Function GetAxis(ByVal id As ControllerAnalog) As Double
        If mIsConnected Then
            Select Case id
                Case ControllerAnalog.L2_trigger, ControllerAnalog.R2_trigger
                    Return Filter1D((mState.GetAxis(id) + 1.0) / 2.0)
                Case ControllerAnalog.L_stick_X
                    Dim v = New Vector2(mState.GetAxis(ControllerAnalog.L_stick_X), mState.GetAxis(ControllerAnalog.L_stick_Y))
                    Return Filter2D(v).X
                Case ControllerAnalog.R_stick_X
                    Dim v = New Vector2(mState.GetAxis(ControllerAnalog.R_stick_X), mState.GetAxis(ControllerAnalog.R_stick_Y))
                    Return Filter2D(v).X
                Case ControllerAnalog.L_stick_Y
                    Dim v = New Vector2(mState.GetAxis(ControllerAnalog.L_stick_X), mState.GetAxis(ControllerAnalog.L_stick_Y))
                    Return -Filter2D(v).Y
                Case ControllerAnalog.R_stick_Y
                    Dim v = New Vector2(mState.GetAxis(ControllerAnalog.R_stick_X), mState.GetAxis(ControllerAnalog.R_stick_Y))
                    Return -Filter2D(v).Y
                Case Else
                    Return mState.GetAxis(id)
            End Select
        Else
            Return 0.0
        End If
    End Function
    Public Function GetIsConnected() As Boolean
        Return mIsConnected
    End Function
    'Friend:
    Friend mState As JoystickState
    Friend mIsConnected As Boolean
    'Private:
    Private Function Filter1D(ByVal input As Double) As Double
        ' デッドゾーン未満の値は０％と解釈
        Static Dim deadZone As Double = 250.0 / 32768.0
        ' 最大値を超える値は１００％と解釈
        Static Dim maxValue As Integer = 30000.0 / 32768.0
        Dim retVal As Double = 0.0
        ' 入力の絶対値を取る
        Dim absValue As Double = input
        If input < 0 Then absValue *= -1.0
        ' デッドゾーン未満の値は無視
        If (absValue > deadZone) Then
            ' デッドゾーンと最大値の間にある値の小数値を計算
            retVal = (absValue - deadZone) / (maxValue - deadZone)
            ' 符号を元の値に合わせる
            If input < 0 Then retVal *= -1.0
            ' 値を-1.0fから1.0fの間に収める
            retVal = Math.Clamp(retVal, -1.0, 1.0)
        End If
        Return retVal
    End Function
    Private Function Filter2D(ByVal input As Vector2) As Vector2
        Static Dim deadZone As Double = 8000.0 / 32768
        Static Dim maxValue As Double = 30000.0 / 32768
        ' 2次元ベクトルを作成
        Dim v As Vector2 = input
        Dim length As Double = v.Length
        ' もし length < deadZoneのとき,入力なしとみなす
        If (length < deadZone) Then
            v = Vector2.Zero
        Else
            ' デッドゾーンと最大値の同心円間の小数を計算
            Dim f As Double = (length - deadZone) / (maxValue - deadZone)
            ' f を 0.0f と 1.0fの間に収める
            f = Math.Clamp(f, 0.0, 1.0)
            ' ベクトルを正規化し、小数値にスケーリング
            v *= f / length
        End If
        Return v
    End Function
End Class

Public Class InputSystem
    'public:
    Sub New(ByRef game As Game)
        mGame = game
        mKeyboardInputState = New KeyboardInputState
        mMouseInputState = New MouseInputState
        mControllerInputState = New ControllerInputState
        mStates.Keyboard = mKeyboardInputState
        mStates.Mouse = mMouseInputState
        mStates.Controller = mControllerInputState
        Initialize()
    End Sub
    Public Sub Initialize()
        'キーボード
        mStates.Keyboard.mState = mGame.KeyboardState

        'マウス
        mStates.Mouse.mState = mGame.MouseState
        mGame.CursorState = CursorState.Normal
        mStates.Mouse.mIsRelative = False

        'コントローラ
        If mGame.JoystickStates.First IsNot Nothing Then
            mStates.Controller.mIsConnected = True
            mStates.Controller.mState = mGame.JoystickStates.First
        Else
            mStates.Controller.mIsConnected = False
            mStates.Controller.mState = Nothing
        End If

    End Sub

    Public Function GetState() As InputState
        Return mStates
    End Function
    Public Sub SetRelativeMouseMode(ByVal value As Boolean)
        If value Then
            mGame.CursorState = CursorState.Grabbed
            mStates.Mouse.mIsRelative = True
        Else
            mGame.CursorState = CursorState.Normal
            mStates.Mouse.mIsRelative = False
        End If
    End Sub

    'private:
    Private mStates As InputState
    Private mGame As Game
    Private mKeyboardInputState As KeyboardInputState
    Private mMouseInputState As MouseInputState
    Private mControllerInputState As ControllerInputState
End Class