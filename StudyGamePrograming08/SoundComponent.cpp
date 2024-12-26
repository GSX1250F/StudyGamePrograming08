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
        // �`�����l���w��̏ꍇ�͈�x�Đ����~�߂čĐ�����B
        if (mChannel >= 0) { Mix_HaltChannel(mChannel); }
        Mix_PlayChannel(mChannel, mChunk, mRepeat);
        mIsPlaying = true;
    }
    if (mControl == "pause" && mIsPlaying == true)
    {
        // �ꎞ��~����B�`�����l����-1�̂Ƃ��͑S�`�����l�����ꎞ��~�B
        Mix_Pause(mChannel);
        mIsPlaying = false;
    }
    if (mControl == "stop" && mIsPlaying == true)
    {
        // ��~����B�`�����l����-1�̂Ƃ��͑S�`�����l�����~�B
        Mix_HaltChannel(mChannel);
        mIsPlaying = false;
    }
    if (mControl == "resume" && mIsPlaying == false)
    {
        // �ĊJ����B�`�����l����-1�̂Ƃ��͑S�`�����l�����ĊJ�B
        Mix_Resume(mChannel);
        mIsPlaying = true;
    }
    if (mControl == "play" && mIsPlaying == false)
    {
        // �`�����l���w��̏ꍇ�A�Đ����łȂ����ƁB
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