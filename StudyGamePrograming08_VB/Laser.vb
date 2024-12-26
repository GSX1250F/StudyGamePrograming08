Imports OpenTK.Mathematics
Public Class Laser
	Inherits Actor

	Private mCircle As CircleComponent
	Private mDeathTime As Double
	Private mLaserSpeed As Double

	Sub New(ByRef game As Game)
		MyBase.New(game)
		'スプライトコンポーネント作成、テクスチャ設定
		mDeathTime = 2.0
		mLaserSpeed = 900.0
		Dim sc As New SpriteComponent(Me, 50)
		sc.SetTexture(game.GetRenderer().GetTexture("Assets\Laser.png"))

		'CircleComponent作成
		mCircle = New CircleComponent(Me, 10)



	End Sub
	Protected Overrides Sub Dispose(ByVal disposing As Boolean)
		If Not Me.disposed Then
			If disposing Then
				'*** アンマネージリソースの開放
			End If
			'*** マネージドリソースの開放

		End If
		MyBase.Dispose(disposing)
	End Sub

	Public Overrides Sub UpdateActor(ByVal deltaTime As Double)
		'DeathTimeが0になるか位置が画面外になったら消去する。
		mDeathTime -= deltaTime
		If (mDeathTime <= 0.0 Or
			GetPosition().X < GetGame().mWindowWidth * (-0.5) - 2.0 * GetRadius() Or
			GetPosition().X > GetGame().mWindowWidth * 0.5 + 2.0 * GetRadius() Or
			GetPosition().Y < GetGame().mWindowHeight * (-0.5) - 2.0 * GetRadius() Or
			GetPosition().Y > GetGame().mWindowHeight * 0.5 + 2.0 * GetRadius()) Then

			SetState(State.EDead)
		Else
			'小惑星に当たったとき
			For Each ast In GetGame().GetAsteroids()
				If Intersect(mCircle, ast.GetCircle()) Then

					'レーザーも消去するなら次を実行
					SetState(State.EDead)
					'小惑星を消去
					ast.SetState(State.EDead)
					Exit For
				End If
			Next
		End If
	End Sub

	Public Sub Shot()
		'MoveComponent作成
		Dim mc As New MoveComponent(Me, 10)
		mc.SetVelocity(mLaserSpeed * GetForward())
	End Sub



End Class