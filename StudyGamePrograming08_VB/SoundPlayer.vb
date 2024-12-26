
Public Class SoundPlayer
    Implements IDisposable      '明示的にクラスを開放するために必要

#If Win64 Then
    Private Declare PtrSafe Function mciSendString Lib "winmm.dll" Alias "mciSendStringA" (ByVal lpstrCommand As String, ByVal lpstrReturnString As String, ByVal uReturnLength As Integer, ByVal hwndCallback As Integer) As Integer
#Else
    Private Declare Function mciSendString Lib "winmm" Alias "mciSendStringA" (ByVal lpstrCommand As String, ByVal lpstrReturnString As String, ByVal uReturnLength As Integer, ByVal hwndCallback As Integer) As Integer
#End If

    Structure ChunkControl
        Dim AliasName As String
        Dim Control As String
    End Structure
    Private mChunks As New Dictionary(Of String, String)    'ファイル名とエイリアスのマップ
    Private mChunkControls As New List(Of ChunkControl)

    Private disposedValue As Boolean
    Sub New(ByRef game As Game)
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
    Public Sub Shutdown()
        Me.Dispose()
    End Sub
    Public Sub UnloadData()
        For Each iter In mChunks
            Dim cmd As String = "close " & iter.Value
            mciSendString(cmd, Nothing, 0, 0)
        Next
        mChunks.Clear()
    End Sub
    Public Sub Play()
        Dim cmd As String
        For Each cc In mChunkControls
            If cc.Control = "replay" Then
                cmd = "stop " & cc.AliasName
                mciSendString(cmd, Nothing, 0, 0)
                cmd = "play " & cc.AliasName & " from 0"
                mciSendString(cmd, Nothing, 0, 0)
            ElseIf cc.Control = "pause" Then
                cmd = "pause " & cc.AliasName
                mciSendString(cmd, Nothing, 0, 0)
            ElseIf cc.Control = "stop" Then
                cmd = "stop " & cc.AliasName
                mciSendString(cmd, Nothing, 0, 0)
            ElseIf cc.Control = "resume" Then
                cmd = "resume " & cc.AliasName
                mciSendString(cmd, Nothing, 0, 0)
            ElseIf cc.Control = "play" Then
                '再生中であれば何もしない。
                cmd = "status " & cc.AliasName & " mode"
                Dim status As String = "          "
                mciSendString(cmd, status, status.Length(), 0)      '状態取得
                If Strings.Left(status, 7) = "playing" Then
                    '何もしない
                ElseIf Strings.Left(status, 5) = "pause" Then
                    cmd = "resume " & cc.AliasName
                    mciSendString(cmd, Nothing, 0, 0)
                Else
                    cmd = "play " & cc.AliasName & " from 0"
                    mciSendString(cmd, Nothing, 0, 0)
                End If
            End If
        Next
        mChunkControls.Clear()
    End Sub
    Public Sub AddChunk(ByVal filename As String)
        'すでにロード済みなら、何もしない
        If mChunks.Count = 0 Or Not mChunks.ContainsKey(filename) Then
            'ファイル名の音声ファイルを空いているAliasNameで開き、mChunksに加える。
            Dim cmd As String = "open """ & filename & """ alias " & "AliasName" & mChunks.Count
            If mciSendString(cmd, Nothing, 0, 0) = 0 Then
                mChunks(filename) = "AliasName" & mChunks.Count
            End If
        End If
    End Sub
    Public Sub SetChunkControl(ByVal filename As String, ByVal control As String)
        'ChunkControlsに追加する。
        Dim cc As ChunkControl
        Dim value As String = ""
        If mChunks.TryGetValue(filename, value) Then
            cc.AliasName = value
            cc.Control = control
            mChunkControls.Add(cc)
        End If

    End Sub
End Class
