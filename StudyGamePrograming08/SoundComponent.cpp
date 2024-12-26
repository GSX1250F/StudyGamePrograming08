#include "SoundComponent.h"
#include "Actor.h"
#include "Game.h"
#include "SoundPlayer.h"

SoundComponent::SoundComponent(Actor* owner)
	: Component(owner)
	, mChannel(-1)
	, mChunk(nullptr)
	, mControl(nullptr)
	, mRepeat(0)
{
	mOwner->GetGame()->GetSoundPlayer()->AddSound(this);
}

SoundComponent::~SoundComponent()
{
	mOwner->GetGame()->GetSoundPlayer()->RemoveSound(this);
}

void SoundComponent::Play()
{
    if (!mIsPlaying)
    {
        if (mControl == "replay")
        {
            // �`�����l���w��̏ꍇ�͈�x�Đ����~�߂čĐ�����B
            if (mChannel >= 0) { Mix_HaltChannel(mChannel); }
            Mix_PlayChannel(mChannel, mChunk, mRepeat);
        }
        if (mControl == "pause")
        {
            // �ꎞ��~����B�`�����l����-1�̂Ƃ��͑S�`�����l�����ꎞ��~�B
            Mix_Pause(mChannel);
        }
        if (mControl == "stop")
        {
            // ��~����B�`�����l����-1�̂Ƃ��͑S�`�����l�����~�B
            Mix_HaltChannel(mChannel);
        }
        if (mControl == "resume")
        {
            // �ĊJ����B�`�����l����-1�̂Ƃ��͑S�`�����l�����ĊJ�B
            Mix_Resume(mChannel);
        }
        if (mControl == "play")
        {
            // �`�����l���w��̏ꍇ�́A�Đ����͉������Ȃ��B
            if ((mChannel >= 0 && Mix_Playing(mChannel) <= 0) || (mChannel < 0))
            {
                Mix_PlayChannel(mChannel, mChunk, mRepeat);
            }
        }
    }

}

void ChunkComponent::SetChunk(std::string& fileName)
{
    // ���łɃ��[�h�ς݂Ȃ�A�������Ȃ�
    auto iter = mChunks.find(filename);
    if (iter == mChunks.end())
    {
        // �t�@�C�����烍�[�h
        Mix_Chunk* chunk = Mix_LoadWAV(filename.c_str());
        if (!chunk)
        {
            SDL_Log("���ʉ��t�@�C���̓ǂݍ��݂Ɏ��s���܂��� %s", filename.c_str());
        }
        mChunks.emplace(filename.c_str(), chunk);
    }

}

void ChunkComponent::SetControl(const int& channel, const std::string& control, const int& repeat)
{
    mChannel = channel;
    mControl = control;
    mRepeat = repeat;
}