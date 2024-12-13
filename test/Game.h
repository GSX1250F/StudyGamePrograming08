#pragma once
#include <SDL.h>

class Game
{
public:
	Game();
	bool Initialize();
	void RunLoop();
	void Shutdown();
		
	void SetRunning(bool isrunning) { mIsRunning = isrunning; }
		
private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();

	Uint32 mTicksCount;
	bool mIsRunning;	
};

