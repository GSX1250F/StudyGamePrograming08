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
        Return mCurrState.IsKeyDown(key)
    End Function
    Public Function GetKeyState(ByVal key As Keys) As ButtonState
        If (mPrevState.IsKeyDown(key) = False) Then
            If (mCurrState.IsKeyDown(key) = False) Then
                Return ButtonState.ENone
            Else
                Return ButtonState.EPressed
            End If
        Else
            If (mCurrState.IsKeyDown(key) = False) Then
                Return ButtonState.EReleased
            Else
                Return ButtonState.EHeld
            End If
        End If
    End Function

    'private:
    Friend mCurrState As KeyboardState
    Friend mPrevState As KeyboardState
End Class
Public Class MouseInputState
    'Public:
    Public Function GetPosition() As Vector2
        If mIsRelative Then
            Return mCurrState.Delta
        Else
            Return mCurrState.Position
        End If
    End Function
    Public Function GetScrollWheel() As Vector2
        Return mCurrState.ScrollDelta
    End Function
    Public Function GetIsRelative() As Boolean
        Return mIsRelative
    End Function
    Public Function GetButtonValue(ByVal button As MouseButton) As Boolean
        Return mCurrState.IsButtonDown(button)
    End Function
    Public Function GetButtonState(ByVal button As MouseButton) As ButtonState
        If (mPrevState.IsButtonDown(button) = False) Then
            If (mCurrState.IsButtonDown(button) = False) Then
                Return ButtonState.ENone
            Else
                Return ButtonState.EPressed
            End If
        Else
            If (mCurrState.IsButtonDown(button) = False) Then
                Return ButtonState.EReleased
            Else
                Return ButtonState.EHeld
            End If
        End If
    End Function

    'Private:
    Friend mCurrState As MouseState
    Friend mPrevState As MouseState
    Friend mIsRelative As Boolean
End Class
Public Class ControllerInputState
    'Public
    Public Function GetButtonValue(ByVal button As Integer) As Boolean
        Return mCurrState.IsButtonDown(button)
    End Function
    Public Function GetButtonState(ByVal button As Integer) As ButtonState
        If (mPrevState.IsButtonDown(button) = False) Then
            If (mCurrState.IsButtonDown(button) = False) Then
                Return ButtonState.ENone
            Else
                Return ButtonState.EPressed
            End If
        Else
            If (mCurrState.IsButtonDown(button) = False) Then
                Return ButtonState.EReleased
            Else
                Return ButtonState.EHeld
            End If
        End If
    End Function
    Public Function GetAxis(ByVal id As Integer) As Double
        Return mCurrState.GetAxis(id)
    End Function
    Public Function GetIsConnected() As Boolean
        Return mIsConnected
    End Function
    'Friend:
    Friend mCurrState As JoystickState
    Friend mPrevState As JoystickState
    Friend mIsConnected As Boolean
End Class

Public Class InputSystem
    'public:
    Sub New(ByRef game As Game)
        mGame = game
        mState.Keyboard = mKeyboardState
        mState.Mouse = mMouseState
        mState.Controller = mControllerState
        Initialize()
    End Sub
    Public Sub Initialize()
        'キーボード
        mState.Keyboard.mCurrState = mGame.KeyboardState
        mState.Keyboard.mPrevState = mState.Keyboard.mCurrState

        'マウス
        mState.Mouse.mCurrState = mGame.MouseState
        mState.Mouse.mPrevState = mState.Mouse.mCurrState
        mGame.CursorState = CursorState.Normal

        'コントローラ
        If mGame.JoystickStates.Count > 0 Then

            mState.Controller.mIsConnected = True
            mState.Controller.mCurrState = mGame.JoystickStates.First
            mState.Controller.mPrevState = mState.Controller.mCurrState
        Else
            mState.Controller.mIsConnected = False
            mState.Controller.mCurrState = Nothing
            mState.Controller.mPrevState = Nothing
        End If

    End Sub

    Public Sub PrepareForUpdate()
        ' 現在の状態を１つ前の状態にコピーする。
        ' キーボード
        mState.Keyboard.mPrevState = mState.Keyboard.mCurrState

        ' マウス
        mState.Mouse.mPrevState = mState.Mouse.mCurrState

        ' コントローラ
        mState.Controller.mPrevState = mState.Controller.mCurrState

    End Sub
    Public Sub Update()
        'OpenTKから現在状態をコピー
        mState.Keyboard.mCurrState = mGame.KeyboardState
        mState.Mouse.mCurrState = mGame.MouseState
        If (mState.Controller.mIsConnected) Then
            'コントローラは１つだけとしておく。
            mState.Controller.mCurrState = mGame.JoystickStates.First
        End If
    End Sub
    Public Function GetState() As InputState
        Return mState
    End Function
    Public Sub SetRelativeMouseMode(ByVal value As Boolean)
        If value Then
            mGame.CursorState = CursorState.Grabbed
            mState.Mouse.mIsRelative = True
        Else
            mGame.CursorState = CursorState.Normal
            mState.Mouse.mIsRelative = False
        End If
    End Sub

    'private:
    Private mState As InputState
    Private mGame As Game
    Private Function Filter1D(ByVal input As Integer) As Double
        Return 0.0
    End Function
    Private Function Filter2D(ByVal inputX As Integer, ByVal inputY As Integer) As Vector2
        Return Vector2.Zero
    End Function

    Private mKeyboardState As New KeyboardInputState
    Private mMouseState As New MouseInputState
    Private mControllerState As New ControllerInputState
End Class