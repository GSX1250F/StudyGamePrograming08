#pragma once
#include "Component.h"
#include <string>
#include <SDL.h>
#include <SDL_mixer.h>

class SoundComponent : public Component
{
public:
	SoundComponent(class Actor* owner);
	~SoundComponent();

	virtual void Play();

	virtual void SetChunk(Mix_Chunk* chunk);
	Mix_Chunk* GetChunk() const { return mChunk; }

	void SetChunkControl(const int& channel, const std::string& control, const int& repeat);
	bool IsPlaying() { return mIsPlaying; }

private:
	int mChannel;		// 0�`15, -1�͋󂢂Ă���Ƃ�����g���B
	Mix_Chunk* mChunk;
	std::string mControl;	// play,pause,halt,resume,replay
	int mRepeat;			// 0:1�񂾂��B-1:�������[�v
	bool mIsPlaying;		// true:playing, false:not playing
};

