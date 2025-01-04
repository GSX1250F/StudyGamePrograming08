Imports OpenTK.Mathematics

Public Class BackGround
    Inherits Actor

    Private mBGs As New List(Of Actor)
    Sub New(ByRef game As Game)
        MyBase.New(game)

        '背景1つ目
        Dim bgactor As New Actor(game)
        bgactor.SetPosition(Vector3.Zero)
        Dim sc As New SpriteComponent(bgactor, 5)
        sc.SetTexture(GetGame().GetRenderer().GetTexture("Assets\Farback01.png"))
        Dim mc As New MoveComponent(bgactor, 30)
        mc.SetVelocity(-10.0 * Vector3.UnitX)
        mBGs.Add(bgactor)

        '背景2つ目
        bgactor = New Actor(game)
        bgactor.SetPosition(game.mWindowWidth * Vector3.UnitX)
        sc = New SpriteComponent(bgactor, 5)
        sc.SetTexture(GetGame().GetRenderer().GetTexture("Assets\Farback02.png"))
        mc = New MoveComponent(bgactor, 30)
        mc.SetVelocity(-10.0 * Vector3.UnitX)
        mBGs.Add(bgactor)

        '背景3つ目
        bgactor = New Actor(game)
        bgactor.SetPosition(Vector3.Zero)
        sc = New SpriteComponent(bgactor, 10)
        sc.SetTexture(GetGame().GetRenderer().GetTexture("Assets\Stars.png"))
        mc = New MoveComponent(bgactor, 30)
        mc.SetVelocity(-20.0 * Vector3.UnitX)
        mBGs.Add(bgactor)

        '背景4つ目
        bgactor = New Actor(game)
        bgactor.SetPosition(game.mWindowWidth * Vector3.UnitX)
        sc = New SpriteComponent(bgactor, 5)
        sc.SetTexture(GetGame().GetRenderer().GetTexture("Assets\Stars.png"))
        mc = New MoveComponent(bgactor, 30)
        mc.SetVelocity(-20.0 * Vector3.UnitX)
        mBGs.Add(bgactor)


    End Sub
    Public Overrides Sub UpdateActor(ByVal deltaTime As Double)
        'ラッピング処理
        Dim v As New Vector2
        For Each bg In mBGs
            If (bg.GetPosition().X < -GetGame().mWindowWidth) Then
                bg.SetPosition(bg.GetPosition() + 2.0 * GetGame().mWindowWidth * Vector3.UnitX)
            ElseIf (bg.GetPosition().X > GetGame().mWindowWidth) Then
                bg.SetPosition(bg.GetPosition() - 2.0 * GetGame().mWindowWidth * Vector3.UnitX)
            End If
            If (bg.GetPosition().Y < -GetGame().mWindowHeight) Then
                bg.SetPosition(bg.GetPosition() + 2.0 * GetGame().mWindowHeight * Vector3.UnitY)
            ElseIf (bg.GetPosition().Y > GetGame().mWindowHeight) Then
                bg.SetPosition(bg.GetPosition() - 2.0 * GetGame().mWindowHeight * Vector3.UnitY)
            End If
        Next
    End Sub
End Class