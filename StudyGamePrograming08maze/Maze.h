#pragma once
#include <vector>
#include "Actor.h"
#include <unordered_map>

class Maze : public Actor
{
public:
	Maze(class Game* game, int mapWidth, int mapHeight);
	void ActorInput(const SDL_Event& event) override;
	void UpdateActor(float deltaTime) override;

	int GetGameStart() { return gameStart; }
	void SetGameClear(bool value) { gameClear = value; }

	// �}�b�v���i�āj�\��
	void GenerateMap();

	Vector3 GetTilePos(int i, int j);

	class Tile* GetStartTile();
	class Tile* GetGoalTile();

	std::vector<std::vector<class Tile*>> GetTiles() { return mTiles; }
	class Treasure* GetTreasure() { return mTreasure; }

private:
	// �Q�[���X�^�[�g
	bool gameStart;
	// �Q�[���N���A
	bool gameClear;
	// ���Z�b�g�J�n
	bool resetStart;
	// ���Z�b�g��
	bool resetPending;
	// ���Z�b�g����
	bool resetEnd;
	
	std::vector<std::vector<int>> mMapIndex;
	int mMapWidth;
	int mMapHeight;
	
	//�X�^�[�g�ʒu
	int starti;
	int startj;
	//�S�[���ʒu
	int goali;
	int goalj;
	// �^�C���T�C�Y
	float mTileSize;
	
	class Tile* mTile;
	std::vector<std::vector<class Tile*>> mTiles;
	class Brave* mBrave;
	class Shadow* mShadow;
	class ClearPict* mClearPict;
	class Treasure* mTreasure;

	// �o�H�T���p
	void MakeGraphNodes(std::vector<std::vector<class Tile*>> &tiles);
	bool FindPath(class Tile* start, class Tile* goal);
};

