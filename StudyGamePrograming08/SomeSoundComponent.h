#pragma once
#include "SoundComponent.h"
#include <vector>
#include <string>

class SomeSoundComponent : public SoundComponent
{
public:
	SomeSoundComponent(class Actor* owner);

	void SetSomeChunks(std::vector<std::string> chunkfiles);
	void SelectChunk(std::string filename);

	std::vector<std::string> ChunkFiles;
};

