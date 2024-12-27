#include "SoundComponent.h"
#include "Actor.h"
#include "Game.h"
#include "SoundPlayer.h"

SoundComponent::SoundComponent(Actor* owner)
	: Component(owner)
	, mChunk(nullptr)
    , mQueued(false)
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
    if (mQueued)
    {
        if (mControl == replay)
        {
            // チャンネル指定の場合は一度再生を止めて再生する。
            if (mChannel >= 0) { Mix_HaltChannel(mChannel); }
            Mix_PlayChannel(mChannel, mChunk, mRepeat);
        }
        if (mControl == pause)
        {
            // 一時停止する。チャンネルが-1のときは全チャンネルを一時停止。
            Mix_Pause(mChannel);
        }
        if (mControl == halt)
        {
            // 停止する。チャンネルが-1のときは全チャンネルを停止。
            Mix_HaltChannel(mChannel);
        }
        if (mControl == resume)
        {
            // 再開する。チャンネルが-1のときは全チャンネルを再開。
            Mix_Resume(mChannel);
        }
        if (mControl == play)
        {
            // チャンネル指定の場合、再生中でないこと。
            if ((mChannel >= 0 && Mix_Playing(mChannel) <= 0) || (mChannel < 0))
            {
                Mix_PlayChannel(mChannel, mChunk, mRepeat);
            }
        }
        mQueued = false;
    }
}

void SoundComponent::SetChunk(Mix_Chunk* chunk)
{
    mChunk = chunk;
}

void SoundComponent::SetChunkInQueue(const int& channel, PlayControl control, const int& repeat)
{
    mChannel = channel;
    mControl = control;
    mRepeat = repeat;
    mQueued = true;
}
