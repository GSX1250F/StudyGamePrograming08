Imports System.Net
Imports OpenTK
Imports OpenTK.Audio

Public Class SoundPlayer2
    Implements IDisposable      '明示的にクラスを開放するために必要
    'public
    Sub New(ByRef game As Game)
        mGame = game
    End Sub
    Protected disposed = False     '開放処理が実施済みかのフラグ
    Public Overloads Sub Dispose() Implements IDisposable.Dispose
        Dispose(True)
    End Sub
    Protected Overridable Overloads Sub Dispose(ByVal disposing As Boolean)
        If Not Me.disposed Then
            If disposing Then
                '*** アンマネージリソースの開放
            End If
            '*** マネージドリソースの開放
        End If
        disposed = True
    End Sub
    Protected Overrides Sub Finalize()
        MyBase.Finalize()
        Dispose(False)
    End Sub
    Public Function Initialize() As Boolean

    End Function
    Public Sub Shutdown()
        UnloadData()
        Me.Dispose()
    End Sub
    Public Sub UnloadData()

    End Sub
    Public Sub Play()

    End Sub
    Public Sub AddChunk(ByRef filename As String)

    End Sub
    Public Sub SetChunkControl(ByRef channel)


End Class
