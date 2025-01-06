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
    Dpad_Up = 10
    Dpad_Down = 12
    Dpad_Left = 13
    Dpad_Right = 11
    Back = 6
    Start = 7
    L3 = 8
    R3 = 9
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
                    Return (mState.GetAxis(id) + 1.0) / 2.0
                Case ControllerAnalog.L_stick_Y, ControllerAnalog.R_stick_Y
                    Return -mState.GetAxis(id)
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
        If mGame.JoystickStates.Count > 0 Then

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
    Private Function Filter1D(ByVal input As Integer) As Double
        Return 0.0
    End Function
    Private Function Filter2D(ByVal inputX As Integer, ByVal inputY As Integer) As Vector2
        Return Vector2.Zero
    End Function
    Private mStates As InputState
    Private mGame As Game
    Private mKeyboardInputState As KeyboardInputState
    Private mMouseInputState As MouseInputState
    Private mControllerInputState As ControllerInputState
End Class