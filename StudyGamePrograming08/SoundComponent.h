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
	void SetPlayable(bool value) { mPlayable = value; }
	bool GetPlayable() const { return mPlayable; }


private:
	Mix_Chunk* mChunk;
	int mChannel;		// 0�`15, -1�͋󂢂Ă���Ƃ�����g���B
	std::string mControl;	// play,pause,halt,resume,replay
	int mRepeat;			// 0:1�񂾂��B-1:�������[�v
	bool mPlayable;
	bool mIsPlaying;		// true:playing, false:not playing
};

