#include "MeshActors.h"
#include "MeshComponent.h"
#include "InputComponent.h"
#include "CircleComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "Actor.h"
#include "Maze.h"
#include "InputSystem.h"

MeshActors::MeshActors(Game* game) :Actor(game){}

Brave::Brave(Game* game)
	: Actor(game)
	, speed(1000.0f)
{
	SetScale(0.85f);
	SetRadius(150.0f);
	//CircleComponent作成
	cc = new CircleComponent(this);
	
	//InputComponent作成
	ic = new InputComponent(this);
	ic->SetMaxForwardVelocity(400);
	ic->SetMaxRotSpeed(4);
	ic->SetKeyConfig(Forward, SDL_SCANCODE_UP);
	ic->SetKeyConfig(Backward, SDL_SCANCODE_DOWN);
	ic->SetKeyConfig(Clockwise, SDL_SCANCODE_RIGHT);
	ic->SetKeyConfig(CounterClockwise, SDL_SCANCODE_LEFT);
	ic->SetInputDeviceConfig(Forward, Mouse_ScrollUp, 1.0);
	ic->SetInputDeviceConfig(Backward, Mouse_ScrollDown, 1.0);
	ic->SetInputDeviceConfig(Forward, Controller_L_Stick_TiltUp, 1.0);
	ic->SetInputDeviceConfig(Backward, Controller_L_Stick_TiltDown, 1.0);
	ic->SetInputDeviceConfig(Clockwise, Controller_L_Stick_TiltRight, 1.0);
	ic->SetInputDeviceConfig(CounterClockwise, Controller_L_Stick_TiltLeft, 1.0);

	//スポットライト
	SpotLight sl;
	sl.mPosition = GetPosition();
	sl.mDirection = GetForward();
	Vector3 color = Vector3(0.3f, 0.3f, 0.2f);
	sl.mDiffuseColor = color;
	sl.mSpecColor = color;
	sl.mAttenuation = 3.0f;
	sl.mCornAngle = Math::Pi / 100.0f;
	sl.mFalloff = 10.0f;
	game->GetRenderer()->AddSpotLight(sl);

	//カメラ方向初期化
	mLookAt = GetForward();
}

void Brave::ActorInput(const InputState& state) {
	// マウス移動、コントローラ右スティックでカメラ方向移動（相対）
	float x = state.Mouse.GetPosition().x + state.Controller.GetRightStick().x;
	float z = -(state.Mouse.GetPosition().y + state.Controller.GetRightStick().y);
	float y = Math::Sqrt(1 - x*x - z*z);
	Vector3 v = Vector3(x, y, z);

}

void Brave::UpdateActor(float deltaTime) {
	if (GetGame()->GetMaze()->GetGameStart()) {
		if (Intersect(*cc, *GetGame()->GetMaze()->GetTreasure()->GetCircle())) {
			// ゴール
			GetGame()->GetMaze()->SetGameClear(true);
		}

		for (auto tilecol : GetGame()->GetMaze()->GetTiles())
		{
			for (auto tile : tilecol)
			{
				if (tile->GetTileState() == Tile::EWall) {
					//壁に衝突していたら離す。
					if (Intersect(*cc, *tile->GetCircle())) {
						//２つの中心を結ぶ線分上で、２つの半径だけ離す
						Vector3 diff = Vector3::Normalize(this->GetPosition() - tile->GetPosition());
						this->SetPosition(tile->GetPosition() + diff * (this->GetRadius() + tile->GetRadius()));
					}
				}
			}
		}

		Vector3 cameraPos = GetPosition();
		Vector3 cameraTarget = GetPosition() + GetForward() * 300.0f;
		Vector3 cameraUp = -1.0 * GetUpward();

		Matrix4 view = Matrix4::CreateLookAt(cameraPos, cameraTarget, cameraUp);
		GetGame()->GetRenderer()->SetViewMatrix(view);

		//スポットライトの位置と方向を更新
		SpotLight& sl = GetGame()->GetRenderer()->GetSpotLights()[0];
		sl.mPosition = GetPosition();
		sl.mDirection = GetForward();
	}
}

Plane::Plane(Game* game)
	: Actor(game)
{
	SetScale(3.0f);
	MeshComponent* mc = new MeshComponent(this);
	mc->SetMesh(game->GetRenderer()->GetMesh("Assets/Plane.gpmesh"));
}

Tile::Tile(class Game* game)
	:Actor(game)
	, mTileState(EDefault)
	, mParent(nullptr)
{
	/*
	//スプライトコンポーネントを作成
	new SpriteComponent(this, 10);
	sc->SetTexture(game->GetRenderer()->GetTexture("Assets/Wall.png"));
	mTexSize = sc->GetTexWidth();
	*/
	mc = new MeshComponent(this);
	mc->SetMesh(game->GetRenderer()->GetMesh("Assets/Wall.gpmesh"));

	SetScale(150.0f);

	//CircleComponent作成
	cc = new CircleComponent(this);
}

void Tile::UpdateActor(float deltaTime)
{
	UpdateTexture();
}

void Tile::SetTileState(TileState state)
{
	mTileState = state;
	UpdateTexture();
}

void Tile::UpdateTexture()
{
	switch (mTileState)
	{
	case EWall:
		//sc->SetVisible(true);
		mc->SetVisible(true);
		break;
	default:
		//sc->SetVisible(false);
		mc->SetVisible(false);
	}
}

Treasure::Treasure(Game* game) : Actor(game) {
	/*
	//スプライトコンポーネント作成、テクスチャ設定
	sc = new SpriteComponent(this, 20);
	sc->SetTexture(game->GetRenderer()->GetTexture("Assets/Treasure.png"));
	*/
	SetScale(100.0f);
	mc = new MeshComponent(this);
	mc->SetMesh(game->GetRenderer()->GetMesh("Assets/Treasure.gpmesh"));

	//CircleComponent作成
	cc = new CircleComponent(this);
}
