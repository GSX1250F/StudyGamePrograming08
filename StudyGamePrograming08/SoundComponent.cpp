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
            // チャンネル指定の場合は一度再生を止めて再生する。
            if (mChannel >= 0) { Mix_HaltChannel(mChannel); }
            Mix_PlayChannel(mChannel, mChunk, mRepeat);
        }
        if (mControl == "pause")
        {
            // 一時停止する。チャンネルが-1のときは全チャンネルを一時停止。
            Mix_Pause(mChannel);
        }
        if (mControl == "stop")
        {
            // 停止する。チャンネルが-1のときは全チャンネルを停止。
            Mix_HaltChannel(mChannel);
        }
        if (mControl == "resume")
        {
            // 再開する。チャンネルが-1のときは全チャンネルを再開。
            Mix_Resume(mChannel);
        }
        if (mControl == "play")
        {
            // チャンネル指定の場合は、再生中は何もしない。
            if ((mChannel >= 0 && Mix_Playing(mChannel) <= 0) || (mChannel < 0))
            {
                Mix_PlayChannel(mChannel, mChunk, mRepeat);
            }
        }
    }

}

void ChunkComponent::SetChunk(std::string& fileName)
{
    // すでにロード済みなら、何もしない
    auto iter = mChunks.find(filename);
    if (iter == mChunks.end())
    {
        // ファイルからロード
        Mix_Chunk* chunk = Mix_LoadWAV(filename.c_str());
        if (!chunk)
        {
            SDL_Log("効果音ファイルの読み込みに失敗しました %s", filename.c_str());
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