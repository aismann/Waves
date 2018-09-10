#pragma once
#include <cmath>
#include <SDL.h>

#include "Constants.h"
#include "ResourceManager.h"

class WaveMaker {
public:
	WaveMaker(SDL_Renderer& r, ResourceManager& rm);
	~WaveMaker();

	Uint32 getWaveDelta();
	Uint32 getWaveTimerStartTime();
	Uint32 getWaveTimerFinishTime();
	bool waveHasArrived();

	void startWaveCounter();

	void updateWaveDelta(int score);

	void render();


private:
	SDL_Renderer& renderer;
	ResourceManager& resourceManager;

	Uint32 waveTimerStartTime;
	Uint32 waveTimerFinishTime;
	Uint32 waveDelta;
	Uint32 initialWaveDelta;
	/*
	Wave Delta function of following format
		-Atan^-1(Bx-C)+D

		Sample function
		-8tan^-1(.004x-1)+14

		X :    0 :   50 :  100 :  200 :  400 : 1000 : 2000 : 4000 
		----------------------------------------------------------
		Y : 20.3 : 19.4 : 18.3 : 15.6 : 9.72 : 4.00 : 2.57 : 1.97

		Revised function
		-7tan^-1(.01x-2)+12.5

		X :    0 :   50 :  100 :  200 :  400 : 1000 : 2000 : 4000 
		----------------------------------------------------------
		Y : 20.3 : 19.4 : 18.0 : 12.5 : 4.75 : 2.37 : 1.89 : 1.69
	*/

};