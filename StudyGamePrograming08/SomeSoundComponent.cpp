#include "SomeSoundComponent.h"
#include "Actor.h"
#include "Game.h"
#include "SoundPlayer.h"

SomeSoundComponent::SomeSoundComponent(Actor* owner)
	: SoundComponent(owner)
{}

void SomeSoundComponent::SetSomeChunks(std::vector<std::string> chunkfiles)
{
	for (auto file : chunkfiles)
	{
		SetChunk(mOwner->GetGame()->GetSoundPlayer()->GetChunk(file));
	}
}

void SomeSoundComponent::SelectChunk(std::string filename)
{
	SetChunk(mOwner->GetGame()->GetSoundPlayer()->GetChunk(filename));
}
