#include "SoundComponent.h"
#include "Actor.h"
#include "Game.h"
#include "SoundPlayer.h"

SoundComponent::SoundComponent(Actor* owner)
	: Component(owner)
	, mChannel(-1)
	, mChunk(nullptr)
    , mControl("halt")
	, mRepeat(0)
    , mIsPlaying(false)
    , mPlayable(true)
{
	mOwner->GetGame()->GetSoundPlayer()->AddSound(this);
}

SoundComponent::~SoundComponent()
{
	mOwner->GetGame()->GetSoundPlayer()->RemoveSound(this);
}

void SoundComponent::Play()
{
    if (mControl == "replay")
    {
        // チャンネル指定の場合は一度再生を止めて再生する。
        if (mChannel >= 0) { Mix_HaltChannel(mChannel); }
        Mix_PlayChannel(mChannel, mChunk, mRepeat);
        mIsPlaying = true;
    }
    if (mControl == "pause" && mIsPlaying == true)
    {
        // 一時停止する。チャンネルが-1のときは全チャンネルを一時停止。
        Mix_Pause(mChannel);
        mIsPlaying = false;
    }
    if (mControl == "stop" && mIsPlaying == true)
    {
        // 停止する。チャンネルが-1のときは全チャンネルを停止。
        Mix_HaltChannel(mChannel);
        mIsPlaying = false;
    }
    if (mControl == "resume" && mIsPlaying == false)
    {
        // 再開する。チャンネルが-1のときは全チャンネルを再開。
        Mix_Resume(mChannel);
        mIsPlaying = true;
    }
    if (mControl == "play" && mIsPlaying == false)
    {
        // チャンネル指定の場合、再生中でないこと。
        if ((mChannel >= 0 && Mix_Playing(mChannel) <= 0) || (mChannel < 0))
        {
            Mix_PlayChannel(mChannel, mChunk, mRepeat);
            mIsPlaying = true;
        }
    }
}

void SoundComponent::SetChunk(Mix_Chunk* chunk)
{
    mChunk = chunk;
}

void SoundComponent::SetChunkControl(const int& channel, const std::string& control, const int& repeat)
{
    mChannel = channel;
    mControl = control;
    mRepeat = repeat;
}