Imports OpenTK.Graphics.OpenGL

Public Class MeshComponent
    Inherits Component
    'Public
    Sub New(ByRef owner As Actor)
        MyBase.New(owner, 10)
        mTextureIndex = 0
        mVisible = True
        mOwner.GetGame().GetRenderer().AddMeshComp(Me)
    End Sub
    Protected Overrides Sub Dispose(ByVal disposing As Boolean)
        If Not Me.disposed Then
            If disposing Then
                '*** アンマネージリソースの開放
            End If
            '*** マネージドリソースの開放
            mOwner.GetGame().GetRenderer().RemoveMeshComp(Me)
        End If
        MyBase.Dispose(disposing)
    End Sub
    Public Overridable Sub Draw(ByRef shader As Shader)
        If ((mMesh IsNot Nothing) And (mVisible)) Then
            'ワールド座標変換の設定
            shader.SetMatrixUniform("uWorldTransform", mOwner.GetWorldTransform())
            'テクスチャの設定
            Dim t As Texture = mMesh.GetTexture(mTextureIndex)
            If t IsNot Nothing Then
                t.SetActive()
            End If
            '鏡面反射指数の設定 
            Dim sp As Single = mMesh.GetSpecPower()
            shader.SetFloatUniform("uSpecPower", mMesh.GetSpecPower())
            'メッシュの頂点情報クラスをアクティブにする。
            Dim vi As VertexInfo = mMesh.GetVertexInfo()
            vi.SetActive()
            'Draw
            GL.DrawElements(
                PrimitiveType.Triangles,
                vi.GetNumIndices(),
                DrawElementsType.UnsignedInt,
                0
            )
        End If
    End Sub

    Public Overridable Sub SetMesh(ByRef mesh As Mesh)
        mMesh = mesh
    End Sub
    Public Overridable Function GetMesh() As Mesh
        Return mMesh
    End Function
    Public Sub SetTextureIndex(ByVal index As Integer)
        mTextureIndex = index
    End Sub
    Public Sub SetVisible(ByVal value As Boolean)
        mVisible = value
    End Sub
    Public Function GetVisible() As Boolean
        Return mVisible
    End Function

    'protected
    Protected mMesh As Mesh
    Protected mTextureIndex As Integer
    Protected mVisible As Boolean
End Class
