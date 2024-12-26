Imports OpenTK
Imports OpenTK.Graphics
Imports OpenTK.Graphics.OpenGL
Imports OpenTK.Mathematics
Imports OpenTK.Windowing.Common

Public Class Renderer
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
    Public Function Initialize(ByVal screenWidth As Integer, ByVal screenHeight As Integer) As Boolean
        mScreenWidth = screenWidth
        mScreenHeight = screenHeight

        ' 画面クリアの色を設定
        GL.ClearColor(0.3, 0.3, 0.3, 1.0)

        ' ビューポート設定
        GL.Viewport(0, 0, screenWidth, screenHeight)

        ' 頂点情報オブジェクトの生成
        CreateVertexInfo()

        ' シェーダーの生成
        If (LoadShaders() <> True) Then
            Console.WriteLine("シェーダーの読み込みに失敗しました。")
            Return False
        End If

        Return True
    End Function
    Public Sub Shutdown()
        UnloadData()
        mVertexInfo.Dispose()
        mShader.Unload()
        mShader.Dispose()
        Me.Dispose()
    End Sub
    Public Sub UnloadData()
        mTextures.Clear()
    End Sub
    Public Sub Draw()
        '画面をクリア
        GL.Clear(ClearBufferMask.ColorBufferBit)
        'カラーバッファのアルファブレンディングを有効化
        GL.Enable(EnableCap.Blend)
        GL.BlendFunc(BlendingFactor.SrcAlpha, BlendingFactor.OneMinusSrcAlpha)

        ' シェーダーとバーテックス配列オブジェクトを有効化
        mVertexInfo.SetActive()
        mShader.SetActive()

        'すべてのスプライトコンポーネントを描画
        For Each sprite In mSprites
            If sprite.GetVisible() = True Then
                sprite.Draw(mShader)
            End If
        Next
        'ビューポート、ビュー変換、ビュー射影変換を更新
        GL.Viewport(0, 0, mScreenWidth, mScreenHeight)
        mShader.SetMatrixUniform("uViewProj", mView * mProj)
        mGame.SwapBuffers()
    End Sub

    Public Sub AddSprite(ByRef sprite As SpriteComponent)
        Dim myDrawOrder As Integer = sprite.GetDrawOrder()
        Dim cnt As Integer = mSprites.Count     '配列の要素数
        Dim iter As Integer
        If cnt > 0 Then
            For iter = 0 To mSprites.Count - 1
                If myDrawOrder < mSprites(iter).GetDrawOrder() Then
                    Exit For
                End If
            Next
        End If
        mSprites.Insert(iter, sprite)
    End Sub
    Public Sub RemoveSprite(ByRef sprite As SpriteComponent)
        Dim iter As Integer = mSprites.IndexOf(sprite)
        '見つからなかったら-1が返される。
        If iter >= 0 Then
            mSprites.RemoveAt(iter)
        End If
    End Sub

    Public Function GetTexture(ByRef filename As String) As Texture
        Dim tex As Texture = Nothing
        '画像ファイルが連想配列になければ追加する。
        Dim b As Boolean = mTextures.ContainsKey(filename)
        If b = True Then
            'すでに読み込み済みなので、そのデータを返す。
            tex = mTextures(filename)
        Else
            'Textureクラスを生成し、連想配列に追加する。
            tex = New Texture()
            If (tex.Load(filename)) Then
                mTextures.Add(filename, tex)
            Else
                tex.Dispose()
                tex = Nothing
            End If
        End If
        Return tex
    End Function

    Public Function GetScreenWidth() As Double
        Return mScreenWidth
    End Function
    Public Function GetScreenHeight() As Double
        Return mScreenHeight
    End Function
    Public Function GetVertexInfo() As VertexInfo
        Return mVertexInfo
    End Function

    'private
    Private Sub CreateVertexInfo()
        Dim numVerts As Integer = 4
        '頂点座標(vector2)
        Dim vertPos As Single() = {
            -0.5, 0.5,
            -0.5, -0.5,
            0.5, -0.5,
            0.5, 0.5
        }
        'テクスチャ座標(vector2)
        Dim texCoord As Single() = {
            0.0, 1.0,
            0.0, 0.0,
            1.0, 0.0,
            1.0, 1.0
        }
        '頂点カラー(vector4 RGBA)
        Dim vertColor As Single() = {
            1.0, 0.0, 0.0, 1.0,
            0.0, 1.0, 0.0, 1.0,
            0.0, 0.0, 1.0, 1.0,
            1.0, 1.0, 1.0, 1.0
        }
        'インデックス
        Dim numIndices As Integer = 6
        Dim indices As UInteger() = {
            0, 1, 2,
            2, 3, 0
        }

        mVertexInfo = New VertexInfo(vertPos, texCoord, vertColor, indices, numVerts, numIndices)
    End Sub
    Private Function LoadShaders() As Boolean
        ' シェーダーを生成
        mShader = New Shader()
        If (mShader.Load("Shaders/shader.vert", "Shaders/shader.frag") <> True) Then
            Return False
        End If
        mShader.SetActive()
        'ビュー変換&射影変換行列の作成
        mView = Matrix4.Identity
        mProj = Matrix4.CreateOrthographic(mScreenWidth, mScreenHeight, -500.0, 500.0)
        mShader.SetMatrixUniform("uViewProj", mProj)
        Return True
    End Function
    Private disposedValue As Boolean
    Private mGame As Game     'GameクラスはOpenTK.GameWindowの子クラス
    Private mScreenWidth As Integer
    Private mScreenHeight As Integer
    Private mTextures As New Dictionary(Of String, Texture)
    Private mSprites As New List(Of SpriteComponent)
    Private mVertexInfo As VertexInfo
    Private mShader As Shader
    Private mView As Matrix4
    Private mProj As Matrix4
End Class