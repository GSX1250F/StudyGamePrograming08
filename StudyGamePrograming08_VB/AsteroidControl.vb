Public Class AsteroidControl
    Inherits Actor

    Private mAsteroidCooldown As Double      '小惑星増殖までの待機時間
    Private InitCooldown As Double      '小惑星増殖までの待機時間

    Public Sub New(ByRef game As Game)
        MyBase.New(game)
        InitCooldown = 5.0
        mAsteroidCooldown = InitCooldown
    End Sub
    Public Overrides Sub UpdateActor(ByVal deltaTime As Double)
        mAsteroidCooldown -= deltaTime
        '小惑星を一定時間ごとに増やす。小惑星の数が０になったらゲームクリア画面をアクティブにする。
        Dim numAsteroids As Integer = GetGame().GetAsteroids().Count()
        If (mAsteroidCooldown < 0.0 And numAsteroids > 0) Then
            GetGame().AddAsteroid()
            mAsteroidCooldown = InitCooldown
        End If
    End Sub
End Class