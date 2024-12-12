#include "Maze.h"
#include "MazeCreator.h"
#include "Game.h"
#include "Renderer.h"
#include "CircleComponent.h"
#include "SpriteComponent.h"
#include "MeshComponent.h"
#include "MeshActors.h"
#include "SpriteActors.h"
#include <queue>
#include <unordered_map>
#include <algorithm>
#include <vector>

Maze::Maze(Game* game, int mapWidth, int mapHeight)
	: Actor(game)
	, mMapWidth(mapWidth)
	, mMapHeight(mapHeight)
	, gameStart(false)
	, gameClear(false)
	, resetStart(true)
	, resetPending(false)
	, resetEnd(false)
{
	//�����A�c�����Ƃ���7�ȏ�̊�ɂ���B
	while (mMapWidth < 7 || mMapWidth % 2 == 0) { mMapWidth++;}
	while (mMapHeight < 7 || mMapHeight % 2 == 0) { mMapHeight++; }

	//Tile���C���X�^���X, mTiles�z���mapIndex�z��𓯂��v�f���ɂ���
	mTiles.resize(mMapWidth);
	mMapIndex.resize(mMapWidth);
	for (int i = 0; i < mTiles.size(); i++)
	{
		mTiles[i].resize(mMapHeight);
		mMapIndex[i].resize(mMapHeight);

		for (int j = 0; j < mTiles[i].size(); j++)
		{
			mTiles[i][j] = new Tile(game);
		}
	}
	//mTileSize = mTiles[0][0]->GetTexSize();
	mTileSize = 150.0f;

	//���b�V���A�N�^�[
	mBrave = new Brave(game);			//�v���C���[
	mTreasure = new Treasure(game);		//�S�[��
	mClearPict = new ClearPict(game);	//�Q�[���N���A�摜

	//��
	for (int i = 0; i < mMapWidth; i++) 
	{
		for (int j = 0; j < mMapHeight; j++) 
		{
			Actor* floor = new Plane(game);
			floor->SetPosition(Vector3(GetTilePos(i, j).x, GetTilePos(i,j).y, mTileSize * 0.5f));
			Quaternion q = Quaternion(Vector3::UnitX, Math::Pi);
			floor->SetRotation(q);
			floor = new Plane(game);
			floor->SetPosition(Vector3(GetTilePos(i, j).x, GetTilePos(i, j).y, -mTileSize * 0.5f));
		}
	}

	// ����
	// ����	
	game->GetRenderer()->SetAmbientLight(Vector3(0.01f, 0.01f, 0.01f));
}

void Maze::ActorInput(const SDL_Event& event)
{
	if (event.type == SDL_KEYUP)
	{
		// �L�[�������ꂽ�Ƃ�
		if (event.key.keysym.sym == SDLK_r)
		{
			resetStart = true;
		}
	}
}

void Maze::UpdateActor(float deltaTime){
	if (gameClear == true) 
	{
		mClearPict->GetSprite()->SetVisible(true);
	}
	if (resetStart == true)
	{
		mBrave->SetState(EPaused);
		//mBrave->GetSprite()->SetVisible(true);
		mTreasure->SetState(EPaused);
		//mTreasure->GetSprite()->SetVisible(false);
		mTreasure->GetMeshComp()->SetVisible(false);
		mClearPict->GetSprite()->SetVisible(false);
		for (int i = 0; i < mTiles.size(); i++)
		{
			mTiles[i].resize(mMapHeight);
			mMapIndex[i].resize(mMapHeight);

			for (int j = 0; j < mTiles[i].size(); j++)
			{
				mTiles[i][j]->SetState(EPaused);
				//mTiles[i][j]->GetSprite()->SetVisible(false);
				mTiles[i][j]->GetMeshComp()->SetVisible(false);
				mTiles[i][j]->ClearAdjacent();
				mTiles[i][j]->SetParent(nullptr);
				mMapIndex[i][j] = 0;
			}
		}
		resetStart = false;
		resetPending = true;
		gameStart = false;
		gameClear = false;
	}
	else if (resetPending == true)
	{
		GenerateMap();
		resetPending = false;
		resetEnd = true;
	}
	else if (resetEnd == true)
	{
		mBrave->SetState(EActive);
		mBrave->SetPosition(GetTilePos(starti, startj));
		mTreasure->SetState(EActive);
		//mTreasure->GetSprite()->SetVisible(true);
		mTreasure->GetMeshComp()->SetVisible(true);
		Vector3 v = GetTilePos(goali, goalj);
		mTreasure->SetPosition(Vector3(v.x, v.y, 30.0f));
		for (auto ctiles : mTiles) {
			for (auto tile : ctiles) {
				if (tile->GetTileState() == Tile::EWall)
				{	
					tile->SetState(EActive);
					//tile->GetSprite()->SetVisible(true);
					tile->GetMeshComp()->SetVisible(true);
				}				
			}
		}
		
		//�אڃm�[�h�쐬
		MakeGraphNodes(mTiles);
		// �����T�� (�t��)
		if (FindPath(GetGoalTile(), GetStartTile())) 
		{ }
		
		resetEnd = false;
		gameStart = true;
	}
}

void Maze::GenerateMap()
{
	//���H�쐬
	bool mazeNG = true;
	while (mazeNG)
	{
		mMapIndex = MazeCreate(mMapWidth, mMapHeight);		
		for (int i = 0; i < mMapWidth; i++) {
			for (int j = 0; j < mMapHeight; j++) {
				switch (mMapIndex[i][j]) {
					case 2:		//�X�^�[�g
						starti = i; startj = j;
						break;
					case 3:		//�S�[��
						goali = i; goalj = j;
						break;
				}
			}
		}
		//�ȒP�����Ȃ����`�F�b�N
		if ((goali > static_cast<int>(mMapWidth / 2)) && (goalj > static_cast<int>(mMapHeight / 2))) { mazeNG = false; }
	}
	for (int i = 0; i < mMapWidth; i++) {
		for (int j = 0; j < mMapHeight; j++) {
			switch (mMapIndex[i][j]) {
			case 0:	//�ʘH
				mTiles[i][j]->SetTileState(Tile::EDefault);
				break;
			case 1: //��
				mTiles[i][j]->SetTileState(Tile::EWall);
				break;
			case 2: //�X�^�[�g
				mTiles[i][j]->SetTileState(Tile::EStart);
				break;
			case 3: //�S�[��
				mTiles[i][j]->SetTileState(Tile::EGoal);
				break;
			}
			mTiles[i][j]->SetPosition(GetTilePos(i, j));
		}
	}
}

Vector3 Maze::GetTilePos(int i, int j)
{
	Vector2 pos = mTileSize * (Vector2((i + 1) * 1.0f, (j + 1) * 1.0f) + Vector2(-mMapWidth/2-1,-mMapHeight/2-1));
	return Vector3(pos.x, pos.y, 0.0f);
}

Tile* Maze::GetStartTile()
{
	return mTiles[starti][startj];
}

Tile* Maze::GetGoalTile()
{
	return mTiles[goali][goalj];
}

void Maze::MakeGraphNodes(std::vector<std::vector<class Tile*>> &tiles)
{
	for (int i = 0; i < tiles.size(); i++) {
		for (int j = 0; j < tiles[i].size(); j++) {
			if (tiles[i][j]->GetTileState() != Tile::EWall) {
				if (i > 0) {
					if (tiles[i - 1][j]->GetTileState() != Tile::EWall) {
						tiles[i][j]->SetAdjacent(tiles[i - 1][j]);
					}
				}
				if (i + 1 <= tiles.size() - 1) {
					if (tiles[i + 1][j]->GetTileState() != Tile::EWall) {
						tiles[i][j]->SetAdjacent(tiles[i + 1][j]);
					}
				}
				if (j > 0){
					if (tiles[i][j - 1]->GetTileState() != Tile::EWall) {
						tiles[i][j]->SetAdjacent(tiles[i][j - 1]);
					}
				}
				if (j + 1 <= tiles[i].size() - 1){
					if (tiles[i][j + 1]->GetTileState() != Tile::EWall) {
						tiles[i][j]->SetAdjacent(tiles[i][j + 1]);
					}
				}				
			}
		}
	}
}

bool Maze::FindPath(Tile* start, Tile* goal)
{
	// ���D��o�H�T���iBFS�j
	std::unordered_map <Tile*, Tile* > outMap;	// �m�[�h����e�ւ̃}�b�v
	bool pathFound = false;	//�o�H�����������H
	std::queue<Tile*> q;	// ��������m�[�h
	// �ŏ��̃m�[�h���L���[�ɂ����B
	q.emplace(start);
	while (!q.empty())
	{
		// �m�[�h���P�L���[����o��
		Tile* current = q.front();
		q.pop();
		if (current == goal)
		{
			pathFound = true;
			break;
		}
		// �܂��L���[�ɓ����Ă��Ȃ��אڃm�[�h���G���L���[����B
		for (Tile* node : current->GetAdjacent())
		{
			// �������̃m�[�h�̗אڃm�[�h�̂��ׂĂɂ��Đe�����邩�𒲂ׂ�B
			Tile* parent = outMap[node];	// outMap�́A�m�[�h�Ɛe�̊֌W�}�b�v
			// �����e��nullptr�Ȃ�A�܂��L���[�ɒǉ�����Ă��Ȃ��B(�������J�n�m�[�h�͗�O)
			if (parent == nullptr && node != start)
			{
				// ���̃m�[�h�̃G���L���[�Ɛe�̐ݒ������B
				outMap[node] = current;
				node->SetParent(current);
				q.emplace(node);
			}
		}		
	}
	return pathFound;
}