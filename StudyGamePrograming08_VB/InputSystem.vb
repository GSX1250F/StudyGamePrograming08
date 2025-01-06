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

    'private:
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

    'Private:
    Friend mState As MouseState
    Friend mIsRelative As Boolean
End Class
Public Class ControllerInputState
    'Public
    Public Function GetButtonValue(ByVal button As Integer) As Boolean
        Return mState.IsButtonDown(button)
    End Function
    Public Function GetButtonState(ByVal button As Integer) As ButtonState
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
    Public Function GetAxis(ByVal id As Integer) As Double
        Return mState.GetAxis(id)
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
        mStates = New InputState
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

    Public Sub Update()
        'OpenTKから現在状態をコピー
        mStates.Keyboard.mState = mGame.KeyboardState
        mStates.Mouse.mState = mGame.MouseState
        If (mStates.Controller.mIsConnected) Then
            'コントローラは１つだけとしておく。
            mStates.Controller.mState = mGame.JoystickStates.First
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
    Private Function Filter1D(ByVal input As Integer) As Double
        Return 0.0
    End Function
    Private Function Filter2D(ByVal inputX As Integer, ByVal inputY As Integer) As Vector2
        Return Vector2.Zero
    End Function

    Private mKeyboardInputState As KeyboardInputState
    Private mMouseInputState As MouseInputState
    Private mControllerInputState As ControllerInputState
End Class