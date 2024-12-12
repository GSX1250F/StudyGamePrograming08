#include "SoundPlayer.h"
#include <SDL.h>
#include <SDL_mixer.h>
#include <algorithm>

SoundPlayer::SoundPlayer(Game* game)
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
    for (auto iter = mChunks.begin(); iter != mChunks.end(); iter++)
    {
        Mix_FreeChunk(iter->second);
    }
    mChunks.clear();
    for (auto iter = mMusics.begin(); iter != mMusics.end(); iter++)
    {
        Mix_FreeMusic(iter->second);
    }
    mMusics.clear();
}

void SoundPlayer::Shutdown()
{
    Mix_CloseAudio();               // SDL_Mixer�N���[�Y 
}

void SoundPlayer::Play()
{
    // ���ʉ��𐧌�
    for (auto cc : mChunkControls)
    {
        if (cc.Control == "replay")
        {
            // �`�����l���w��̏ꍇ�͈�x�Đ����~�߂čĐ�����B
            if (cc.Channel >= 0) { Mix_HaltChannel(cc.Channel); }
            Mix_PlayChannel(cc.Channel, cc.Chunk, cc.Repeat);
        }
        if (cc.Control == "pause")
        {
            // �ꎞ��~����B�`�����l����-1�̂Ƃ��͑S�`�����l�����ꎞ��~�B
            Mix_Pause(cc.Channel);
        }
        if (cc.Control == "stop")
        {
            // ��~����B�`�����l����-1�̂Ƃ��͑S�`�����l�����~�B
            Mix_HaltChannel(cc.Channel);
        }
        if (cc.Control == "resume")
        {
            // �ĊJ����B�`�����l����-1�̂Ƃ��͑S�`�����l�����ĊJ�B
            Mix_Resume(cc.Channel);
        }
        if (cc.Control == "play")
        {
            // �`�����l���w��̏ꍇ�́A�Đ����͉������Ȃ��B
            if ((cc.Channel >= 0 && Mix_Playing(cc.Channel) <= 0) || (cc.Channel < 0))
            { 
                Mix_PlayChannel(cc.Channel, cc.Chunk, cc.Repeat);
            }
        }
    }
    // �����A�z����N���A
    mChunkControls.clear();

    // �Đ��҂����y���Đ�
    for (auto mc : mMusicControls)
    {
        if (mc.Control == "replay")
        {
            // ��x�Đ����~�߂čĐ�����B
            Mix_HaltMusic(); 
            Mix_PlayMusic(mc.Music, mc.Repeat);
        }
        if (mc.Control == "pause")
        {
            // �ꎞ��~����B
            Mix_PauseMusic();
        }
        if (mc.Control == "stop")
        {
            // ��~����B
            Mix_HaltMusic();
        }
        if (mc.Control == "resume")
        {
            // �ĊJ����B
            Mix_ResumeMusic();
        }
        if (mc.Control == "play")
        {
            // �Đ����͉������Ȃ��B
            if (Mix_PlayingMusic() <= 0)
            {
                Mix_PlayMusic(mc.Music, mc.Repeat);
            }
        }
    }
    // �����A�z����N���A
    mMusicControls.clear();
}

void SoundPlayer::AddChunk(const std::string& filename)
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

void SoundPlayer::AddMusic(const std::string& filename)
{
    // ���łɃ��[�h�ς݂Ȃ�A�������Ȃ�
    auto iter = mMusics.find(filename);
    if (iter == mMusics.end())
    {
        // �t�@�C�����烍�[�h
        Mix_Music* music = Mix_LoadMUS(filename.c_str());
        if (!music)
        {
            SDL_Log("���y�t�@�C���̓ǂݍ��݂Ɏ��s���܂��� %s", filename.c_str());
        }
        mMusics.emplace(filename.c_str(), music);
    }
}

void SoundPlayer::SetChunkControl(const int& channel, const std::string& filename, const std::string& control, const int& repeat)
{
    ChunkControl cc;
    cc.Channel = channel;
    cc.Chunk = mChunks.find(filename)->second;
    cc.Control = control;
    cc.Repeat = repeat;
    mChunkControls.emplace_back(cc);
}

void SoundPlayer::SetMusicControl(const std::string& filename, const std::string& control, const int& repeat)
{
    MusicControl mc;
    mc.Music = mMusics.find(filename)->second;
    mc.Control = control;
    mc.Repeat = repeat;
    mMusicControls.emplace_back(mc);
}