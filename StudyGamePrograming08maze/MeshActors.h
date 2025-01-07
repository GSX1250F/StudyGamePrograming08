#pragma once
#include <string>
#include <vector>
#include "Actor.h"
class MeshActors : public Actor
{
public:
	MeshActors(class Game* game);
};

class Brave : public Actor
{
public:
	Brave(Game* game);
	void UpdateActor(float deltaTime) override;
	void ActorInput(const InputState& state) override;

	//class SpriteComponent* GetSprite() { return sc; }
	class CircleComponent* GetCircle() { return cc; }
	float GetSpeed() { return speed; }
	Vector3 GetLookAt() { return mLookAt; }

private:
	//class SpriteComponent* sc;
	class CircleComponent* cc;
	class MoveComponent* mc;
	class InputComponent* ic;

	float speed;
	Vector3 mLookAt;
};

class Plane : public Actor
{
public:
	Plane(Game* game);
};

class Tile : public Actor
{
public:
	enum TileState
	{
		EDefault,
		EWall,
		EStart,
		EGoal
	};

	Tile(class Game* game);
	void UpdateActor(float deltaTime) override;

	void UpdateTexture();

	//class SpriteComponent* GetSprite() { return sc; }
	class MeshComponent* GetMeshComp() { return mc; }
	class CircleComponent* GetCircle() { return cc; }
	TileState GetTileState() { return mTileState; }
	float GetTexSize() { return mTexSize; }
	void SetTileState(TileState);
	void SetAdjacent(class Tile* tile) { mAdjacent.push_back(tile); }
	void ClearAdjacent() { mAdjacent.clear(); }
	std::vector<class Tile*> GetAdjacent() { return mAdjacent; }
	void SetParent(class Tile* tile) { mParent = tile; }
	class Tile* GetParent() { return mParent; }


private:
	//class SpriteComponent* sc;
	class MeshComponent* mc;
	class CircleComponent* cc;

	TileState mTileState;
	float mTexSize;

	std::vector<class Tile*> mAdjacent;	//隣接ノード配列
	Tile* mParent;	//親ノード

};

class Treasure : public Actor
{
public:
	Treasure(class Game* game);

	class CircleComponent* GetCircle() { return cc; }
	//class SpriteComponent* GetSprite() { return sc; }
	class MeshComponent* GetMeshComp() { return mc; }


private:
	//class SpriteComponent* sc;
	class MeshComponent* mc;
	class CircleComponent* cc;
};
