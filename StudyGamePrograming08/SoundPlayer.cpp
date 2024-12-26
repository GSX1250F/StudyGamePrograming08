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
    // SDL������
    if (SDL_Init(SDL_INIT_AUDIO) < 0)
    {
        SDL_Log("SDL�̏������Ɏ��s���܂���: %s", SDL_GetError());
        return false;
    }
    // SDL_mixer������
    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) < 0)
    {
        SDL_Log("SDL_mixer�̏������Ɏ��s���܂���: %s", SDL_GetError());
        return false;
    }
    // ������������16�ɐݒ�
    if (Mix_AllocateChannels(16) < 0)
    {
        SDL_Log("�����������̐ݒ�Ɏ��s���܂���: %s", SDL_GetError());
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
    Mix_CloseAudio();               // SDL_Mixer�N���[�Y 
}

void SoundPlayer::Play()
{
    // ���ʉ����Đ�
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
        // �t�@�C�����烍�[�h
        Mix_Chunk* chunk = Mix_LoadWAV(fileName.c_str());
        if (!chunk)
        {
            SDL_Log("���ʉ��t�@�C���̓ǂݍ��݂Ɏ��s���܂��� %s", fileName.c_str());
        }
        mChunks.emplace(fileName.c_str(), chunk);
    }
    return c;
}