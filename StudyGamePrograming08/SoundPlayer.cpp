#include "SoundPlayer.h"
#include <SDL.h>
#include <SDL_mixer.h>
#include <algorithm>
#include "SoundComponent.h"

SoundPlayer::SoundPlayer(Game* game)
    : mGame(game)
{}

SoundPlayer::~SoundPlayer()
{}

bool SoundPlayer::Initialize()
{
    // SDL初期化
    if (SDL_Init(SDL_INIT_AUDIO) < 0)
    {
        SDL_Log("SDLの初期化に失敗しました: %s", SDL_GetError());
        return false;
    }
    // SDL_mixer初期化
    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) < 0)
    {
        SDL_Log("SDL_mixerの初期化に失敗しました: %s", SDL_GetError());
        return false;
    }
    // 同時発声数を16に設定
    if (Mix_AllocateChannels(16) < 0)
    {
        SDL_Log("同時発声数の設定に失敗しました: %s", SDL_GetError());
        return false;
    }

	return true;
}

void SoundPlayer::UnloadData()
{
    for (auto i : mChunks)
    {
        Mix_FreeChunk(i.second);
        //delete i.second;
    }
    mChunks.clear();
}

void SoundPlayer::Shutdown()
{
    Mix_CloseAudio();               // SDL_Mixerクローズ 
}

void SoundPlayer::Play()
{
    // 効果音を再生
    for (auto sound : mSounds)
    {
        if (sound->GetPlayable())
        {
            sound->Play();
        }
    }
}

void SoundPlayer::AddSound(SoundComponent* sound)
{
    mSounds.emplace_back(sound);
}

void SoundPlayer::RemoveSound(SoundComponent* sound)
{
    auto iter = std::find(mSounds.begin(), mSounds.end(), sound);
    mSounds.erase(iter);
}

Mix_Chunk* SoundPlayer::GetChunk(const std::string& fileName)
{
    Mix_Chunk* c = nullptr;
    auto iter = mChunks.find(fileName);
    if (iter != mChunks.end())
    {
        c = iter->second;
    }
    else
    {
        // ファイルからロード
        Mix_Chunk* chunk = Mix_LoadWAV(fileName.c_str());
        if (!chunk)
        {
            SDL_Log("効果音ファイルの読み込みに失敗しました %s", fileName.c_str());
        }
        mChunks.emplace(fileName.c_str(), chunk);
    }
    return c;
}