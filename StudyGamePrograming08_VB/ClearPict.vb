Imports OpenTK.Mathematics

Public Class ClearPict
    Inherits Actor

    Private sc As SpriteComponent

    Sub New(ByRef game As Game)
        MyBase.New(game)
        SetState(State.EPaused)
        SetPosition(-50.0 * Vector3.UnitZ)

        'スプライトコンポーネント作成、テクスチャ設定
        sc = New SpriteComponent(Me, 100)
        sc.SetTexture(game.GetRenderer().GetTexture("Assets\ClearPict.png"))
        sc.SetVisible(False)
    End Sub
    Public Overrides Sub UpdateActor(ByVal deltaTime As Double)
        Dim numAsteroids As Integer = GetGame().GetAsteroids().Count()
        If (numAsteroids <= 0) Then
            SetState(State.EActive)
            sc.SetVisible(True)
        End If
    End Sub
End Class