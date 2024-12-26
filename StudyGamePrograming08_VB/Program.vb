Imports System
Imports OpenTK.Windowing.Desktop

Module Program
    Sub Main(args As String())
        Dim game As Game = New Game(1024, 768, "StudyGamePrograming in VB app")
        Dim success As Boolean = game.Initialize()
        Dim fps = 60.0
        If (success) Then
            game.UpdateFrequency = fps
            game.Run()      '�����I��OpenTK�ɂ��Q�[�����[�v�ɓ���B
        End If
        game.Shutdown()
    End Sub
End Module
