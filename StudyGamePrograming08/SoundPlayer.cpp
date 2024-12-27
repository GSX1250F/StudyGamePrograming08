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
    Mix_CloseAudio();               // SDL_Mixerクローズ 
}

void SoundPlayer::Play()
{
    // 効果音を制御
    for (auto cc : mChunkControls)
    {
        if (cc.Control == "replay")
        {
            // チャンネル指定の場合は一度再生を止めて再生する。
            if (cc.Channel >= 0) { Mix_HaltChannel(cc.Channel); }
            Mix_PlayChannel(cc.Channel, cc.Chunk, cc.Repeat);
        }
        if (cc.Control == "pause")
        {
            // 一時停止する。チャンネルが-1のときは全チャンネルを一時停止。
            Mix_Pause(cc.Channel);
        }
        if (cc.Control == "stop")
        {
            // 停止する。チャンネルが-1のときは全チャンネルを停止。
            Mix_HaltChannel(cc.Channel);
        }
        if (cc.Control == "resume")
        {
            // 再開する。チャンネルが-1のときは全チャンネルを再開。
            Mix_Resume(cc.Channel);
        }
        if (cc.Control == "play")
        {
            // チャンネル指定の場合は、再生中は何もしない。
            if ((cc.Channel >= 0 && Mix_Playing(cc.Channel) <= 0) || (cc.Channel < 0))
            { 
                Mix_PlayChannel(cc.Channel, cc.Chunk, cc.Repeat);
            }
        }
    }
    // 制御後、配列をクリア
    mChunkControls.clear();

    // 再生待ち音楽を再生
    for (auto mc : mMusicControls)
    {
        if (mc.Control == "replay")
        {
            // 一度再生を止めて再生する。
            Mix_HaltMusic(); 
            Mix_PlayMusic(mc.Music, mc.Repeat);
        }
        if (mc.Control == "pause")
        {
            // 一時停止する。
            Mix_PauseMusic();
        }
        if (mc.Control == "stop")
        {
            // 停止する。
            Mix_HaltMusic();
        }
        if (mc.Control == "resume")
        {
            // 再開する。
            Mix_ResumeMusic();
        }
        if (mc.Control == "play")
        {
            // 再生中は何もしない。
            if (Mix_PlayingMusic() <= 0)
            {
                Mix_PlayMusic(mc.Music, mc.Repeat);
            }
        }
    }
    // 制御後、配列をクリア
    mMusicControls.clear();
}

void SoundPlayer::AddChunk(const std::string& filename)
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

void SoundPlayer::AddMusic(const std::string& filename)
{
    // すでにロード済みなら、何もしない
    auto iter = mMusics.find(filename);
    if (iter == mMusics.end())
    {
        // ファイルからロード
        Mix_Music* music = Mix_LoadMUS(filename.c_str());
        if (!music)
        {
            SDL_Log("音楽ファイルの読み込みに失敗しました %s", filename.c_str());
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