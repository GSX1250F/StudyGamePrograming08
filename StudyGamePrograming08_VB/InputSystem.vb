﻿Imports System.Runtime.CompilerServices
Imports OpenTK.Windowing.GraphicsLibraryFramework
Imports OpenTK
Imports OpenTK.Windowing.Common
Imports OpenTK.Input

Public Structure InputState
    Dim Keyboard As KeyboardState
    Dim Mouse As MouseState
    Dim Controller As JoystickState
End Structure

Public Class InputSystem
    Sub New()
    End Sub
    Public Sub SetState(ByRef inputState As InputState)
        mState = inputState
    End Sub
    Public Function GetState() As InputState
        Return mState
    End Function

    Private mState As InputState
End Class
