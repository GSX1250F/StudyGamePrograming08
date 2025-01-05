Imports OpenTK.Windowing.GraphicsLibraryFramework
Imports OpenTK.Windowing.Common
Imports System.Runtime.CompilerServices

Public Enum ButtonState
    ENone
    EPressed
    EHeld
    EReleased
End Enum
Public Structure InputState
    Dim Keyboard As KeyboardInputSystem
    Dim Mouse As MouseState
    Dim Controller As IReadOnlyList(Of JoystickState)
End Structure

Public Class InputSystem


    'public:
    Sub New(ByRef game As Game)
        mGame = game
        'キーボード
        mState.Keyboard.mCurrState = mGame.KeyboardState
        mState.Keyboard.mPrevState = Nothing

        mGame.CursorState = CursorState.Normal
        mMouseCursorIsGrabbed = False


    End Sub
    Public Sub Update()
        mState.Keyboard.mCurrState = mGame.KeyboardState
        mState.Mouse = mGame.MouseState
        mState.Controller = mGame.JoystickStates
    End Sub
    Public Function GetState() As InputState
        Return mState
    End Function
    Public Sub SetMouseCursorGrabbed(ByVal value As Boolean)
        If value Then
            mGame.CursorState = CursorState.Grabbed
            mMouseCursorIsGrabbed = True
        Else
            mGame.CursorState = CursorState.Normal
            mMouseCursorIsGrabbed = False
        End If
    End Sub
    Public Function GetMouseCursorIsGrabbed() As Boolean
        Return mMouseCursorIsGrabbed
    End Function
    'private:
    Private mState As InputState
    Private mGame As Game
    Private mMouseCursorIsGrabbed As Boolean
End Class

Friend Class KeyboardInputSystem
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
