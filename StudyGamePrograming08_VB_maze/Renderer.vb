Imports System.IO.MemoryMappedFiles
Imports System.Net.Mime.MediaTypeNames
Imports OpenTK
Imports OpenTK.Graphics
Imports OpenTK.Graphics.OpenGL
Imports OpenTK.Mathematics
Imports OpenTK.Windowing.Common

Public Structure DirectionalLight
    Dim mDirection As Vector3       '光の方向
    Dim mDiffuseColor As Vector3    '拡散反射色
    Dim mSpecColor As Vector3       '鏡面反射色
End Structure
Public Structure PointLight
    Dim mPosition As Vector3        '光源の位置
    Dim mDiffuseColor As Vector3    '拡散反射色
    Dim mSpecColor As Vector3       '鏡面反射色
    Dim mAttenuation As Single      '減衰係数
End Structure
Public Structure SpotLight
    Dim mPosition As Vector3        '点光源の位置
    Dim mDirection As Vector3       '光の方向
    Dim mDiffuseColor As Vector3    '拡散反射色
    Dim mSpecColor As Vector3       '鏡面反射色
    Dim mAttenuation As Single      '減衰係数
    Dim mCornAngle As Single        '照射角度
    Dim mFalloff As Single          '照射角度外減衰指数
End Structure

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

        ' 画面クリアの色を設定
        GL.ClearColor(0.2, 0.2, 0.2, 1.0)

        Return True
    End Function
    Public Sub UnloadData()
        For Each i In mTextures
            i.Value.Unload()
        Next
        mTextures.Clear()
        For Each i In mMeshes
            i.Value.Unload()
        Next
        mMeshes.Clear()
    End Sub
    Public Sub Shutdown()
        mVertexInfo.Dispose()
        mSpriteShader.Unload()
        mSpriteShader.Dispose()
        'mMeshShader.Unload()
        'mMeshShader.Dispose()
        For Each sh In mShaders
            sh.Key.Unload()
            sh.Key.Dispose()
        Next
        Me.Dispose()
    End Sub
    Public Sub Draw()
        '画面をクリア＆深度バッファをクリア
        GL.Clear(ClearBufferMask.ColorBufferBit Or ClearBufferMask.DepthBufferBit)

        '深度有効化、アルファブレンディング無効化
        GL.Enable(EnableCap.DepthTest)
        GL.Disable(EnableCap.Blend)

        'シェーダー毎に描画
        For Each sh In mShaders
            'メッシュを描画
            'mMeshShader.SetActive()
            'mMeshShader.SetMatrixUniform("uViewProj", mView * mProj)
            sh.Key.SetActive()
            sh.Key.SetMatrixUniform("uViewProj", mView * mProj)
            '光源のUniform変数を更新
            'SetLightUniforms(mMeshShader)
            SetLightUniforms(sh.Key)

            For Each mc In mMeshComps
                Dim shaderName As String = mc.GetMesh.GetShaderName
                If shaderName = sh.Value Then
                    mc.Draw(sh.Key)
                End If
            Next
        Next

        '深度無効化、アルファブレンディング有効化
        GL.Disable(EnableCap.DepthTest)
        GL.Enable(EnableCap.Blend)
        GL.BlendEquationSeparate(BlendEquationMode.FuncAdd, BlendEquationMode.FuncAdd)
        GL.BlendFuncSeparate(BlendingFactorSrc.SrcAlpha, BlendingFactorDest.OneMinusSrcAlpha, BlendingFactorSrc.One, BlendingFactorDest.Zero)
        'スプライトを描画
        mSpriteShader.SetActive()
        mVertexInfo.SetActive()
        For Each sprite In mSprites
            If sprite.GetVisible() = True Then
                sprite.Draw(mSpriteShader)
            End If
        Next

        'ダブルバッファを交換
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

    Public Sub AddMeshComp(ByRef mesh As MeshComponent)
        mMeshComps.Add(mesh)
    End Sub
    Public Sub RemoveMeshComp(ByRef mesh As MeshComponent)
        mMeshComps.Remove(mesh)
    End Sub

    Public Function GetTexture(ByRef filename As String) As Texture
        Dim tex As Texture = Nothing
        Dim b As Boolean = mTextures.ContainsKey(filename)
        If b = True Then
            tex = mTextures(filename)
        Else
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
    Public Function GetMesh(ByRef filename As String) As Mesh
        Dim m As Mesh = Nothing
        Dim b As Boolean = mMeshes.ContainsKey(filename)
        If b = True Then
            m = mMeshes(filename)
        Else
            m = New Mesh()
            If m.Load(filename, Me) Then
                mMeshes.Add(filename, m)
            Else
                m.Dispose()
                m = Nothing
            End If
        End If
        Return m
    End Function
    Public Function GetVertexInfo() As VertexInfo
        Return mVertexInfo
    End Function
    Public Function GetScreenWidth() As Single
        Return mScreenWidth
    End Function
    Public Function GetScreenHeight() As Single
        Return mScreenHeight
    End Function
    Public Sub SetViewMatrix(ByRef matrix As Matrix4)
        mView = matrix
    End Sub
    Public Sub SetProjMatrix(ByRef matrix As Matrix4)
        mProj = matrix
    End Sub
    Public Function GetAmbientlLight() As Vector3
        Return mAmbientLight
    End Function

    Public Sub SetAmbientLight(ByRef ambient As Vector3)
        mAmbientLight = ambient
    End Sub
    'Public Function GetDirectionalLight() As DirectionalLight
    '    Return mDirLight
    'End Function
    'Public Sub SetDirectionalLight(ByRef dir As DirectionalLight)
    '    mDirLight = dir
    'End Sub
    Public Function GetDirectionalLights() As List(Of DirectionalLight)
        Return mDirLights
    End Function
    Public Sub AddDirectionalLight(ByVal dl As DirectionalLight)
        mDirLights.Add(dl)
    End Sub
    Public Sub SetDirectionalLights(ByVal dl As List(Of DirectionalLight))
        mDirLights = dl
    End Sub
    Public Function GetPointLights() As List(Of PointLight)
        Return mPointLights
    End Function
    Public Sub AddPointLight(ByVal pl As PointLight)
        mPointLights.Add(pl)
    End Sub
    Public Sub SetPointLights(ByVal pl As List(Of PointLight))
        mPointLights = pl
    End Sub
    Public Function GetSpotLights() As List(Of SpotLight)
        Return mSpotLights
    End Function
    Public Sub AddSpotLight(ByVal sl As SpotLight)
        mSpotLights.Add(sl)
    End Sub
    Public Sub SetSpotLights(ByVal sl As List(Of SpotLight))
        mSpotLights = sl
    End Sub

    'private
    Private Sub CreateVertexInfo()
        '右手座標系
        '頂点座標(vector3), 法線ベクトル(Vector3), テクスチャ座標(Vector2)
        Dim vertices As Single() = {
             -0.5, -0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
              0.5, -0.5, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0,
              0.5, 0.5, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0,
             -0.5, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0
        }

        'インデックス
        Dim indices As UInteger() = {
                0, 1, 2,
                2, 3, 0
        }
        mVertexInfo = New VertexInfo(vertices, 4, indices, 6)
    End Sub
    Private Function LoadShaders() As Boolean
        'スプライト用シェーダーを生成
        mSpriteShader = New Shader()
        If (mSpriteShader.Load("Shaders/SpriteShader.vert", "Shaders/SpriteShader.frag") <> True) Then
            Return False
        End If
        mSpriteShader.SetActive()
        'スプライトの描画には、平行射影を行う。
        mView = Matrix4.Identity
        mProj = Matrix4.CreateOrthographic(mScreenWidth, mScreenHeight, -5000.0, 5000.0)
        mSpriteShader.SetMatrixUniform("uViewProj", mView * mProj)

        'メッシュ用シェーダーを生成
        'メッシュの描画には、透視射影を行う。
        Dim cameraPos As Vector3 = Vector3.Zero
        Dim cameraTarget As Vector3 = Vector3.UnitX
        Dim cameraUp As Vector3 = Vector3.UnitZ
        mView = Matrix4.LookAt(cameraPos, cameraTarget, cameraUp)
        mProj = Matrix4.CreatePerspectiveFieldOfView(70.0 / 180.0 * Math.PI, mScreenWidth / mScreenHeight, 0.01, 10000.0)
        'BasicShader
        Dim mMeshShader = New Shader()
        If (mMeshShader.Load("Shaders/BasicMesh.vert", "Shaders/BasicMesh.frag") <> True) Then
            Return False
        End If
        mMeshShader.SetActive()
        mMeshShader.SetMatrixUniform("uViewProj", mView * mProj)
        mShaders.Add(mMeshShader, "BasicMesh")
        'LambertShader
        mMeshShader = New Shader()
        If (mMeshShader.Load("Shaders/LambertMesh.vert", "Shaders/LambertMesh.frag") <> True) Then
            Return False
        End If
        mMeshShader.SetActive()
        mMeshShader.SetMatrixUniform("uViewProj", mView * mProj)
        mShaders.Add(mMeshShader, "LambertMesh")
        'PhongShader
        mMeshShader = New Shader()
        If (mMeshShader.Load("Shaders/PhongMesh.vert", "Shaders/PhongMesh.frag") <> True) Then
            Return False
        End If
        mMeshShader.SetActive()
        mMeshShader.SetMatrixUniform("uViewProj", mView * mProj)
        mShaders.Add(mMeshShader, "PhongMesh")

        'mMeshShader = New Shader()
        'If (mMeshShader.Load("Shaders/MeshShader.vert", "Shaders/MeshShader.frag") <> True) Then
        '    Return False
        'End If
        'mMeshShader.SetActive()

        Return True
    End Function

    Private Sub SetLightUniforms(ByRef shader As Shader)
        'カメラの位置はビューを反転して求める
        Dim invView As Matrix4 = mView
        invView.Invert()
        shader.SetVectorUniform("uCameraPos", invView.ExtractTranslation)
        '環境光
        shader.SetVectorUniform("uAmbientLight", mAmbientLight)
        '平行光源
        'shader.SetVectorUniform("uDirLight.mDirection", mDirLight.mDirection)
        'shader.SetVectorUniform("uDirLight.mDiffuseColor", mDirLight.mDiffuseColor)
        'shader.SetVectorUniform("uDirLight.mSpecColor", mDirLight.mSpecColor)
        Dim str As String
        For i As Integer = 0 To mDirLights.Count() - 1
            str = "uDirLights[" + i.ToString + "].mDirection"
            shader.SetVectorUniform(str, mDirLights(i).mDirection)
            str = "uDirLights[" + i.ToString + "].mDiffuseColor"
            shader.SetVectorUniform(str, mDirLights(i).mDiffuseColor)
            str = "uDirLights[" + i.ToString + "].mSpecColor"
            shader.SetVectorUniform(str, mDirLights(i).mSpecColor)
        Next
        '点光源
        For i As Integer = 0 To mPointLights.Count() - 1
            str = "uPointLights[" + i.ToString + "].mPosition"
            shader.SetVectorUniform(str, mPointLights(i).mPosition)
            str = "uPointLights[" + i.ToString + "].mDiffuseColor"
            shader.SetVectorUniform(str, mPointLights(i).mDiffuseColor)
            str = "uPointLights[" + i.ToString + "].mSpecColor"
            shader.SetVectorUniform(str, mPointLights(i).mSpecColor)
            str = "uPointLights[" + i.ToString + "].mAttenuation"
            shader.SetFloatUniform(str, mPointLights(i).mAttenuation)
        Next
        'スポットライト
        For i As Integer = 0 To mSpotLights.Count() - 1
            str = "uSpotLights[" + i.ToString + "].mPosition"
            shader.SetVectorUniform(str, mSpotLights(i).mPosition)
            str = "uSpotLights[" + i.ToString + "].mDirection"
            shader.SetVectorUniform(str, mSpotLights(i).mDirection)
            str = "uSpotLights[" + i.ToString + "].mDiffuseColor"
            shader.SetVectorUniform(str, mSpotLights(i).mDiffuseColor)
            str = "uSpotLights[" + i.ToString + "].mSpecColor"
            shader.SetVectorUniform(str, mSpotLights(i).mSpecColor)
            str = "uSpotLights[" + i.ToString + "].mAttenuation"
            shader.SetFloatUniform(str, mSpotLights(i).mAttenuation)
            str = "uSpotLights[" + i.ToString + "].mCornAngle"
            shader.SetFloatUniform(str, mSpotLights(i).mCornAngle)
            str = "uSpotLights[" + i.ToString + "].mFalloff"
            shader.SetFloatUniform(str, mSpotLights(i).mFalloff)
        Next
    End Sub

    Private disposedValue As Boolean
    Private mGame As Game     'GameクラスはOpenTK.GameWindowの子クラス
    Private mScreenWidth As Integer
    Private mScreenHeight As Integer
    Private mTextures As New Dictionary(Of String, Texture)
    Private mMeshes As New Dictionary(Of String, Mesh)
    Private mShaders As New Dictionary(Of Shader, String)
    Private mSprites As New List(Of SpriteComponent)
    Private mMeshComps As New List(Of MeshComponent)
    Private mVertexInfo As VertexInfo
    Private mSpriteShader As Shader
    'Private mMeshShader As Shader
    Private mView As Matrix4
    Private mProj As Matrix4
    Private mAmbientLight As Vector3
    'Private mDirLight As DirectionalLight
    Private mDirLights As New List(Of DirectionalLight)
    Private mPointLights As New List(Of PointLight)
    Private mSpotLights As New List(Of SpotLight)
End Class