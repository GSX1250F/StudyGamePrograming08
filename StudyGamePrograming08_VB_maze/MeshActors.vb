Imports System.Drawing
Imports OpenTK.Mathematics
Imports OpenTK.Windowing.GraphicsLibraryFramework

Public Class MeshActors
	Inherits Actor
	Sub New(ByRef game As Game)
		MyBase.New(game)
	End Sub
End Class

Public Class Brave
	Inherits Actor
	Sub New(ByRef game As Game)
		MyBase.New(game)
		speed = 1000.0
		SetScale(0.85)
		SetRadius(150.0)

		'CircleComponent作成
		cc = New CircleComponent(Me, 100)

		'MoveComponent作成
		mc = New MoveComponent(Me, 10)

		'スポットライト
		Dim sl As SpotLight
		sl.mPosition = GetPosition()
		sl.mDirection = GetForward()
		Dim Color = New Vector3(0.3, 0.3, 0.2)
		sl.mDiffuseColor = Color
		sl.mSpecColor = Color
		sl.mAttenuation = 3.0
		sl.mCornAngle = Math.PI / 100.0
		sl.mFalloff = 10.0
		game.GetRenderer().AddSpotLight(sl)
	End Sub
	Public Overrides Sub ActorInput(ByVal keyState As KeyboardState)
		If (GetGame().GetMaze().GetGameStart()) Then
			Dim forwardSpeed As Double = 0.0
			Dim angularSpeed As Double = 0.0
			If (keyState.IsKeyDown(Keys.Up)) Then
				forwardSpeed = speed
			ElseIf (keyState.IsKeyDown(Keys.Down)) Then
				forwardSpeed = -speed
			ElseIf (keyState.IsKeyDown(Keys.Left)) Then
				angularSpeed = Math.PI
			ElseIf (keyState.IsKeyDown(Keys.Right)) Then
				angularSpeed = -Math.PI
			End If
			mc.SetVelocity(forwardSpeed * GetForward())
			mc.SetRotSpeed(angularSpeed * GetUpward())
		End If
	End Sub
	Public Overrides Sub UpdateActor(ByVal deltaTime As Double)
		If (GetGame().GetMaze().GetGameStart()) Then
			If (Intersect(cc, GetGame().GetMaze().GetTreasure().GetCircle())) Then
				' ゴール
				GetGame().GetMaze().SetGameClear(True)
				'sc.SetVisible(True)
			End If
		End If

		For Each tilecol In GetGame().GetMaze().GetTiles()
			For Each tile In tilecol
				If (tile.GetTileState() = Tile.TileState.EWall) Then
					'壁に衝突していたら離す。
					If (Intersect(cc, tile.GetCircle())) Then
						'２つの中心を結ぶ線分上で、２つの半径だけ離す
						Dim diff As Vector3 = Vector3.Normalize(Me.GetPosition() - tile.GetPosition())
						Me.SetPosition(tile.GetPosition() + diff * (Me.GetRadius() + tile.GetRadius()))
					End If
				End If
			Next
		Next

		Dim cameraPos As Vector3 = GetPosition()
		Dim cameraTarget As Vector3 = GetPosition() + GetForward() * 300.0
		Dim cameraUp As Vector3 = GetUpward()
		Dim view = Matrix4.LookAt(cameraPos, cameraTarget, cameraUp)
		GetGame().GetRenderer().SetViewMatrix(view)

		'スポットライトの位置と方向を更新
		Dim sl As SpotLight
		Dim sls As List(Of SpotLight) = GetGame().GetRenderer().GetSpotLights()
		sl = sls(0)
		sl.mPosition = GetPosition()
		sl.mDirection = GetForward()
		sls(0) = sl
		GetGame().GetRenderer().SetSpotLights(sls)
	End Sub

	Public Function GetSprite() As SpriteComponent
		Return sc
	End Function
	Public Function GetCircle() As CircleComponent
		Return cc
	End Function
	Public Function GetSpeed() As Double
		Return speed
	End Function

	Private sc As SpriteComponent
	Private cc As CircleComponent
	Private mc As MoveComponent
	Private speed As Double
End Class

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

Public Class Tile
	Inherits Actor

	Public Enum TileState
		EDefault
		EWall
		EStart
		EGoal
	End Enum

	Sub New(ByRef game As Game)
		MyBase.New(game)
		mTileState = TileState.EDefault
		mParent = Nothing
		''スプライトコンポーネントを作成
		'sc = New SpriteComponent(Me, 10)
		'sc.SetTexture(game.GetRenderer().GetTexture("Assets/Wall.png"))
		'mTexSize = sc.GetTexWidth()

		mc = New MeshComponent(Me)
		mc.SetMesh(game.GetRenderer.GetMesh("Assets/Wall.gpmesh"))

		SetScale(150.0)

		'CircleComponent作成
		cc = New CircleComponent(Me, 100)
	End Sub
	Public Overrides Sub UpdateActor(deltaTime As Double)
		UpdateTexture()
	End Sub
	Public Sub UpdateTexture()
		Select Case mTileState
			Case TileState.EWall
				'sc.SetVisible(True)
				mc.SetVisible(True)
			Case Else
				'sc.SetVisible(False)
				mc.SetVisible(False)
		End Select
	End Sub
	Public Function GetSprite() As SpriteComponent
		Return sc
	End Function
	Public Function GetMeshComp() As MeshComponent
		Return mc
	End Function
	Public Function GetCircle() As CircleComponent
		Return cc
	End Function
	Public Function GetTileState() As TileState
		Return mTileState
	End Function
	Public Function GetTexSize() As Double
		Return mTexSize
	End Function
	Public Sub SetTileState(ByVal state As TileState)
		mTileState = state
		UpdateTexture()
	End Sub
	Public Sub SetAdjacent(ByRef tile As Tile)
		mAdjacent.Add(tile)
	End Sub
	Public Function GetAdjacent() As List(Of Tile)
		Return mAdjacent
	End Function
	Public Sub SetParent(ByRef tile As Tile)
		mParent = tile
	End Sub
	Public Function GetParent() As Tile
		Return mParent
	End Function
	Public Sub ClearAdjacent()
		mAdjacent.Clear()
	End Sub

	Private mAdjacent As New List(Of Tile)
	Private mParent As Tile
	Private sc As SpriteComponent
	Private mc As MeshComponent
	Private cc As CircleComponent
	Private mTileState As TileState
	Private mTexSize As Double

End Class

Public Class Treasure
	Inherits Actor
	Sub New(ByRef game As Game)
		MyBase.New(game)
		''スプライトコンポーネント作成、テクスチャ設定
		'sc = New SpriteComponent(Me, 20)
		'sc.SetTexture(game.GetRenderer().GetTexture("Assets/Treasure.png"))
		SetScale(100.0)
		mc = New MeshComponent(Me)
		mc.SetMesh(game.GetRenderer.GetMesh("Assets/Treasure.gpmesh"))

		'CircleComponent作成
		cc = New CircleComponent(Me, 100)
	End Sub
	Public Function GetSprite() As SpriteComponent
		Return sc
	End Function
	Public Function GetMeshComp() As MeshComponent
		Return mc
	End Function

	Public Function GetCircle() As CircleComponent
		Return cc
	End Function

	Private sc As SpriteComponent
	Private cc As CircleComponent
	Private mc As MeshComponent
End Class

