Imports OpenTK.Mathematics
Imports OpenTK.Windowing.GraphicsLibraryFramework
Public Class ClearPict
    Inherits Actor
	'public
	Sub New(ByRef game As Game)
		MyBase.New(game)
		SetState(State.EPaused)
		SetPosition(Vector3.Zero)
		SetScale(3.0)

		sc = New SpriteComponent(Me, 100)
		sc.SetTexture(game.GetRenderer.GetTexture("Assets/ClearPict.png"))
		sc.SetVisible(False)
	End Sub
	Public Function GetSprite() As SpriteComponent
		Return sc
	End Function
	Private sc As SpriteComponent
End Class
