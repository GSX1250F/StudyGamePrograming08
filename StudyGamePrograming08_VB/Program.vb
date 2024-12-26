Imports System
Imports OpenTK.Windowing.Desktop

Module Program
    Sub Main(args As String())
        Dim game As Game = New Game(1024, 768, "StudyGamePrograming in VB app")
        Dim success As Boolean = game.Initialize()
        If (success) Then
            game.Run()
        End If
        game.Shutdown()
    End Sub
End Module
