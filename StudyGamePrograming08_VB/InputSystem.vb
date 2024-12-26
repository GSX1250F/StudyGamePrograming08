Imports OpenTK.Mathematics
Imports OpenTK.Windowing.GraphicsLibraryFramework

Structure InputState
    Dim Keyboard As KeyboardState
    Dim Mouse As MouseState
    Dim Controller As JoystickState
End Structure

Class InputSystem
    'Public:
    Public Sub Update()

    End Sub

    Public Function GetState() As InputState
        Return mState
    End Function

    Public Sub SetRelativeMouseMode(ByVal value As Boolean)
        If (value) Then

        End If
    End Sub

    'Private
    Private mState As InputState
    Private Function Filter1D(ByVal input As Integer) As Double

    End Function
    Private Function Filter2D(ByVal inputX As Integer, ByVal inputY As Integer) As Vector2

    End Function
End Class