Imports System
Imports OpenTK.Windowing.Desktop

Module Program
    Sub Main(args As String())
        Dim game As Game = New Game(1600, 900, "StudyGamePrograming in VB app")
        Dim success As Boolean = game.Initialize()
        If (success) Then
            game.Run()      '�����I��OpenTK�ɂ��Q�[�����[�v�ɓ���B
        End If
        game.Shutdown()
    End Sub
End Module
