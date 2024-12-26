#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "Math.h"
#include <SDL.h>
#include <SDL_mixer.h>

class SoundPlayer
{
public:	
	SoundPlayer(class Game* game);
	~SoundPlayer();

	bool Initialize();
	void UnloadData();
	void Shutdown();

	void Play();

	void AddSound(class SoundComponent* sound);
	void RemoveSound(class SoundComponent* sound);

	class Mix_Chunk* GetChunk(const std::string& fileName);

private:
	// 効果音のマップ
	std::unordered_map<std::string, class Mix_Chunk*> mChunks;

	// サウンドコンポーネントの配列
	std::vector<class SoundComponent*> mSounds;

	// Game
	class Game* mGame;
};