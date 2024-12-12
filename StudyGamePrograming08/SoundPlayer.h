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
		int Channel;		// 0�`15, -1�͋󂢂Ă���Ƃ�����g���B
		Mix_Chunk* Chunk;
		std::string Control;	// play,pause,halt,resume,replay
		int Repeat;			// 0:1�񂾂��B-1:�������[�v
	};
	struct MusicControl
	{
		Mix_Music* Music;
		std::string Control;	// play,pause,halt,resume,replay
		int Repeat;			// 0:1�񂾂��B-1:�������[�v
	};

	// ���ʉ��̃}�b�v
	std::unordered_map<std::string, Mix_Chunk*> mChunks;
	
	// ���y�̃}�b�v
	std::unordered_map<std::string, Mix_Music*> mMusics;

	// �Đ��҂����ʉ��̔z��
	std::vector<ChunkControl> mChunkControls;

	// �Đ��҂����y�̔z��
	std::vector<MusicControl> mMusicControls;
};