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
	//CircleComponent�쐬
	cc = new CircleComponent(this);
	
	//InputComponent�쐬
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

	//�X�|�b�g���C�g
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

	//�J��������������
	mLookAt = GetForward();
}

void Brave::ActorInput(const InputState& state) {
	// �}�E�X�ړ��A�R���g���[���E�X�e�B�b�N�ŃJ���������ړ��i���΁j
	float x = state.Mouse.GetPosition().x + state.Controller.GetRightStick().x;
	float z = -(state.Mouse.GetPosition().y + state.Controller.GetRightStick().y);
	float y = Math::Sqrt(1 - x*x - z*z);
	Vector3 v = Vector3(x, y, z);

}

void Brave::UpdateActor(float deltaTime) {
	if (GetGame()->GetMaze()->GetGameStart()) {
		if (Intersect(*cc, *GetGame()->GetMaze()->GetTreasure()->GetCircle())) {
			// �S�[��
			GetGame()->GetMaze()->SetGameClear(true);
		}

		for (auto tilecol : GetGame()->GetMaze()->GetTiles())
		{
			for (auto tile : tilecol)
			{
				if (tile->GetTileState() == Tile::EWall) {
					//�ǂɏՓ˂��Ă����痣���B
					if (Intersect(*cc, *tile->GetCircle())) {
						//�Q�̒��S�����Ԑ�����ŁA�Q�̔��a��������
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

		//�X�|�b�g���C�g�̈ʒu�ƕ������X�V
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
	//�X�v���C�g�R���|�[�l���g���쐬
	new SpriteComponent(this, 10);
	sc->SetTexture(game->GetRenderer()->GetTexture("Assets/Wall.png"));
	mTexSize = sc->GetTexWidth();
	*/
	mc = new MeshComponent(this);
	mc->SetMesh(game->GetRenderer()->GetMesh("Assets/Wall.gpmesh"));

	SetScale(150.0f);

	//CircleComponent�쐬
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
	//�X�v���C�g�R���|�[�l���g�쐬�A�e�N�X�`���ݒ�
	sc = new SpriteComponent(this, 20);
	sc->SetTexture(game->GetRenderer()->GetTexture("Assets/Treasure.png"));
	*/
	SetScale(100.0f);
	mc = new MeshComponent(this);
	mc->SetMesh(game->GetRenderer()->GetMesh("Assets/Treasure.gpmesh"));

	//CircleComponent�쐬
	cc = new CircleComponent(this);
}
