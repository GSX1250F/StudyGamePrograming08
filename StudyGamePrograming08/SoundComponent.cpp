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
            // �`�����l���w��̏ꍇ�͈�x�Đ����~�߂čĐ�����B
            if (mChannel >= 0) { Mix_HaltChannel(mChannel); }
            Mix_PlayChannel(mChannel, mChunk, mRepeat);
        }
        if (mControl == pause)
        {
            // �ꎞ��~����B�`�����l����-1�̂Ƃ��͑S�`�����l�����ꎞ��~�B
            Mix_Pause(mChannel);
        }
        if (mControl == halt)
        {
            // ��~����B�`�����l����-1�̂Ƃ��͑S�`�����l�����~�B
            Mix_HaltChannel(mChannel);
        }
        if (mControl == resume)
        {
            // �ĊJ����B�`�����l����-1�̂Ƃ��͑S�`�����l�����ĊJ�B
            Mix_Resume(mChannel);
        }
        if (mControl == play)
        {
            // �`�����l���w��̏ꍇ�A�Đ����łȂ����ƁB
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
