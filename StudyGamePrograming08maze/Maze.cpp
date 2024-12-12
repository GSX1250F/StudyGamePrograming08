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
	//横幅、縦幅をともに7以上の奇数にする。
	while (mMapWidth < 7 || mMapWidth % 2 == 0) { mMapWidth++;}
	while (mMapHeight < 7 || mMapHeight % 2 == 0) { mMapHeight++; }

	//Tileをインスタンス, mTiles配列とmapIndex配列を同じ要素数にする
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

	//メッシュアクター
	mBrave = new Brave(game);			//プレイヤー
	mTreasure = new Treasure(game);		//ゴール
	mClearPict = new ClearPict(game);	//ゲームクリア画像

	//床
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

	// 光源
	// 環境光	
	game->GetRenderer()->SetAmbientLight(Vector3(0.01f, 0.01f, 0.01f));
}

void Maze::ActorInput(const SDL_Event& event)
{
	if (event.type == SDL_KEYUP)
	{
		// キーが離されたとき
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
		
		//隣接ノード作成
		MakeGraphNodes(mTiles);
		// 道順探索 (逆順)
		if (FindPath(GetGoalTile(), GetStartTile())) 
		{ }
		
		resetEnd = false;
		gameStart = true;
	}
}

void Maze::GenerateMap()
{
	//迷路作成
	bool mazeNG = true;
	while (mazeNG)
	{
		mMapIndex = MazeCreate(mMapWidth, mMapHeight);		
		for (int i = 0; i < mMapWidth; i++) {
			for (int j = 0; j < mMapHeight; j++) {
				switch (mMapIndex[i][j]) {
					case 2:		//スタート
						starti = i; startj = j;
						break;
					case 3:		//ゴール
						goali = i; goalj = j;
						break;
				}
			}
		}
		//簡単すぎないかチェック
		if ((goali > static_cast<int>(mMapWidth / 2)) && (goalj > static_cast<int>(mMapHeight / 2))) { mazeNG = false; }
	}
	for (int i = 0; i < mMapWidth; i++) {
		for (int j = 0; j < mMapHeight; j++) {
			switch (mMapIndex[i][j]) {
			case 0:	//通路
				mTiles[i][j]->SetTileState(Tile::EDefault);
				break;
			case 1: //壁
				mTiles[i][j]->SetTileState(Tile::EWall);
				break;
			case 2: //スタート
				mTiles[i][j]->SetTileState(Tile::EStart);
				break;
			case 3: //ゴール
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
	// 幅優先経路探索（BFS）
	std::unordered_map <Tile*, Tile* > outMap;	// ノードから親へのマップ
	bool pathFound = false;	//経路を見つけたか？
	std::queue<Tile*> q;	// 検討するノード
	// 最初のノードをキューにいれる。
	q.emplace(start);
	while (!q.empty())
	{
		// ノードを１つキューから出す
		Tile* current = q.front();
		q.pop();
		if (current == goal)
		{
			pathFound = true;
			break;
		}
		// まだキューに入っていない隣接ノードをエンキューする。
		for (Tile* node : current->GetAdjacent())
		{
			// 検討中のノードの隣接ノードのすべてについて親があるかを調べる。
			Tile* parent = outMap[node];	// outMapは、ノードと親の関係マップ
			// もし親がnullptrなら、まだキューに追加されていない。(ただし開始ノードは例外)
			if (parent == nullptr && node != start)
			{
				// このノードのエンキューと親の設定をする。
				outMap[node] = current;
				node->SetParent(current);
				q.emplace(node);
			}
		}		
	}
	return pathFound;
}