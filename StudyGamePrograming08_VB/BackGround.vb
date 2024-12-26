Imports OpenTK.Mathematics

Public Class BackGround
    Inherits Actor

    Private mBGs As New List(Of Actor)
    Sub New(ByRef game As Game)
        MyBase.New(game)

        '背景1つ目
        Dim bgactor As New Actor(game)
        Dim v As Vector2 = Vector2.Zero
        bgactor.SetPosition(v)
        Dim sc As New SpriteComponent(bgactor, 5)
        sc.SetTexture(GetGame().GetRenderer().GetTexture("Assets\Farback01.png"))
        Dim mc As New MoveComponent(bgactor, 30)
        v.X = -10.0
        v.Y = 0.0
        mc.SetVelocity(v)
        mBGs.Add(bgactor)

        '背景2つ目
        bgactor = New Actor(game)
        v.X = game.mWindowWidth
        v.Y = 0.0
        bgactor.SetPosition(v)
        sc = New SpriteComponent(bgactor, 5)
        sc.SetTexture(GetGame().GetRenderer().GetTexture("Assets\Farback02.png"))
        mc = New MoveComponent(bgactor, 30)
        v.X = -10.0
        v.Y = 0.0
        mc.SetVelocity(v)
        mBGs.Add(bgactor)

        '背景3つ目
        bgactor = New Actor(game)
        v.X = 0.0
        v.Y = 0.0
        bgactor.SetPosition(v)
        sc = New SpriteComponent(bgactor, 10)
        sc.SetTexture(GetGame().GetRenderer().GetTexture("Assets\Stars.png"))
        mc = New MoveComponent(bgactor, 30)
        v.X = -20.0
        v.Y = 0.0
        mc.SetVelocity(v)
        mBGs.Add(bgactor)

        '背景4つ目
        bgactor = New Actor(game)
        v.X = game.mWindowWidth
        v.Y = 0.0
        bgactor.SetPosition(v)
        sc = New SpriteComponent(bgactor, 5)
        sc.SetTexture(GetGame().GetRenderer().GetTexture("Assets\Stars.png"))
        mc = New MoveComponent(bgactor, 30)
        v.X = -20.0
        v.Y = 0.0
        mc.SetVelocity(v)
        mBGs.Add(bgactor)


    End Sub
    Public Overrides Sub UpdateActor(ByVal deltaTime As Double)
        'ラッピング処理
        Dim v As New Vector2
        For Each bg In mBGs
            If (bg.GetPosition().X < -GetGame().mWindowWidth) Then
                v.X = bg.GetPosition().X + 2.0 * GetGame().mWindowWidth
                v.Y = bg.GetPosition().Y
                bg.SetPosition(v)
            ElseIf (bg.GetPosition().X > GetGame().mWindowWidth) Then
                v.X = bg.GetPosition().X - 2.0 * GetGame().mWindowWidth
                v.Y = bg.GetPosition().Y
                bg.SetPosition(v)
            End If
            If (bg.GetPosition().Y < -GetGame().mWindowHeight) Then
                v.Y = bg.GetPosition().Y + 2.0 * GetGame().mWindowHeight
                v.X = bg.GetPosition().X
                bg.SetPosition(v)
            ElseIf (bg.GetPosition().Y > GetGame().mWindowHeight) Then
                v.Y = bg.GetPosition().Y - 2.0 * GetGame().mWindowHeight
                v.X = bg.GetPosition().X
                bg.SetPosition(v)
            End If
        Next
    End Sub
End Class