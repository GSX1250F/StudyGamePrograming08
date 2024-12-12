#pragma once
#include <string>
#include <vector>
#include <unordered_map>
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

	void AddChunk(const std::string& filename);
	void AddMusic(const std::string& filename);

	void SetChunkControl(const int& channel,const std::string& filename,const std::string& control,const int& repeat);
	void SetMusicControl(const std::string& filename,const std::string& control,const int& repeat);

private:
	struct ChunkControl
	{
		int Channel;		// 0～15, -1は空いているところを使う。
		Mix_Chunk* Chunk;
		std::string Control;	// play,pause,halt,resume,replay
		int Repeat;			// 0:1回だけ。-1:無限ループ
	};
	struct MusicControl
	{
		Mix_Music* Music;
		std::string Control;	// play,pause,halt,resume,replay
		int Repeat;			// 0:1回だけ。-1:無限ループ
	};

	// 効果音のマップ
	std::unordered_map<std::string, Mix_Chunk*> mChunks;
	
	// 音楽のマップ
	std::unordered_map<std::string, Mix_Music*> mMusics;

	// 再生待ち効果音の配列
	std::vector<ChunkControl> mChunkControls;

	// 再生待ち音楽の配列
	std::vector<MusicControl> mMusicControls;
};