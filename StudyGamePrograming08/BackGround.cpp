#include "BackGround.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Renderer.h"

BackGround::BackGround(Game* game) :Actor(game)
{
	//1‚Â–Ú‚Ì”wŒi
	Actor* bgactor = new Actor(game);
	bgactor->SetPosition(Vector3::Zero);
	SpriteComponent* sc = new SpriteComponent(bgactor, 5);
	sc->SetTexture(game->GetRenderer()->GetTexture("Assets/Farback01.png"));
	MoveComponent* mc = new MoveComponent(bgactor);
	mc->SetVelocity(-10.0f * Vector3::UnitX);
	mBGs.emplace_back(bgactor);

	//2‚Â–Ú‚Ì”wŒi
	bgactor = new Actor(game);
	bgactor->SetPosition(game->mWindowWidth * Vector3::UnitX);
	sc = new SpriteComponent(bgactor, 5);
	sc->SetTexture(game->GetRenderer()->GetTexture("Assets/Farback02.png"));
	mc = new MoveComponent(bgactor);
	mc->SetVelocity(-10.0f * Vector3::UnitX);
	mBGs.emplace_back(bgactor);

	//3‚Â–Ú‚Ì”wŒi
	bgactor = new Actor(game);
	bgactor->SetPosition(Vector3::Zero);
	sc = new SpriteComponent(bgactor, 10);
	sc->SetTexture(game->GetRenderer()->GetTexture("Assets/Stars.png"));
	mc = new MoveComponent(bgactor);
	mc->SetVelocity(-20.0f * Vector3::UnitX);
	mBGs.emplace_back(bgactor);

	//4‚Â–Ú‚Ì”wŒi
	bgactor = new Actor(game);
	bgactor->SetPosition(game->mWindowWidth * Vector3::UnitX);
	sc = new SpriteComponent(bgactor, 10);
	sc->SetTexture(game->GetRenderer()->GetTexture("Assets/Stars.png"));
	mc = new MoveComponent(bgactor);
	mc->SetVelocity(-20.0f * Vector3::UnitX);
	mBGs.emplace_back(bgactor);
}

void BackGround::UpdateActor(float deltaTime)
{
	//ƒ‰ƒbƒsƒ“ƒOˆ—
	for (auto bg : mBGs)
	{
		if (bg->GetPosition().x < -GetGame()->mWindowWidth)
		{
			bg->SetPosition(bg->GetPosition() + 2.0f * GetGame()->mWindowWidth * Vector3::UnitX);
		}
		else if (bg->GetPosition().x > GetGame()->mWindowWidth)
		{
			bg->SetPosition(bg->GetPosition() - 2.0f * GetGame()->mWindowWidth * Vector3::UnitX);
		}
		if (bg->GetPosition().y < -GetGame()->mWindowHeight)
		{
			bg->SetPosition(bg->GetPosition() + 2.0f * GetGame()->mWindowHeight * Vector3::UnitY);
		}
		else if (bg->GetPosition().y > GetGame()->mWindowHeight)
		{
			bg->SetPosition(bg->GetPosition() - 2.0f * GetGame()->mWindowHeight * Vector3::UnitY);
		}
	}
}