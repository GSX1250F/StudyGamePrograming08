Imports OpenTK.Windowing.GraphicsLibraryFramework
Imports OpenTK.Windowing.Common

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
        mMouseCursorIsGrabbed = False
    End Sub
    Public Sub Update()
        mState.Keyboard = mGame.KeyboardState
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
