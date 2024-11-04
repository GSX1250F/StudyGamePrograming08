Public Class Plane
    Inherits Actor
    'public
    Sub New(ByRef game As Game)
        MyBase.New(game)
        SetScale(3.0)
        Dim mc As MeshComponent = New MeshComponent(Me)
        mc.SetMesh(game.GetRenderer.GetMesh("Assets/Plane.gpmesh"))
    End Sub
End Class
