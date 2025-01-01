Imports System.Runtime.CompilerServices
Imports OpenTK.Windowing.GraphicsLibraryFramework
Imports OpenTK
Imports OpenTK.Windowing.Common
Imports OpenTK.Windowing.Desktop
Imports OpenTK.Input
Imports OpenTK.Mathematics
Imports System.Windows.Markup

Public Structure InputState
    Dim Keyboard As KeyboardState
    Dim Mouse As MouseState
    Dim Controller As IReadOnlyList(Of JoystickState)
End Structure

Public Class InputSystem
    'public:
    Sub New(ByRef game As Game)
        mGame = game
        mGame.CursorState = CursorState.Normal
    End Sub
    Public Sub Update()
        mState.Keyboard = mGame.KeyboardState
        mState.Mouse = mGame.MouseState
        mState.Controller = mGame.JoystickStates
    End Sub
    Public Function GetState() As InputState
        Return mState
    End Function
    Public Sub SetRelativeMouseMode(ByVal value As Boolean)
        If value Then
            mGame.CursorState = CursorState.Grabbed
        Else
            mGame.CursorState = CursorState.Normal
        End If
    End Sub
    'private:
    Private mState As InputState
    Private mGame As Game
End Class
