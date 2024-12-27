#pragma once
#include "Component.h"
#include <string>
#include <SDL.h>
#include <SDL_mixer.h>

enum PlayControl
{
	play,
	pause,
	halt,
	resume,
	replay
};

class SoundComponent : public Component
{
public:
	SoundComponent(class Actor* owner);
	~SoundComponent();

	virtual void Play();

	virtual void SetChunk(Mix_Chunk* chunk);
	Mix_Chunk* GetChunk() const { return mChunk; }
	// channel: 0�`15, -1�͋󂢂Ă���Ƃ�����g���B
	// repeat: 0:1�񂾂��B-1:�������[�v
	void SetChunkInQueue(const int& channel = -1, PlayControl control = play, const int& repeat = 0);
	
	void SetPlayable(bool value) { mPlayable = value; }
	bool GetPlayable() const { return mPlayable; }

private:
	Mix_Chunk* mChunk;
	PlayControl mControl;
	int mChannel;
	int mRepeat;
	bool mPlayable;
	bool mQueued;
};

