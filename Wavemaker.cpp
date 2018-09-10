#include "Wavemaker.h"

WaveMaker::WaveMaker(SDL_Renderer& r, ResourceManager& rm) : renderer(r), resourceManager(rm) {
	this->waveDelta = (-8 * std::atan((0.004 * 0) - 1) + 14) * 1000;
}
WaveMaker::~WaveMaker() {

}

Uint32 WaveMaker::getWaveDelta() {
	return this->waveDelta;
}
Uint32 WaveMaker::getWaveTimerStartTime() {
	return this->waveTimerStartTime;
}
Uint32 WaveMaker::getWaveTimerFinishTime() {
	return this->waveTimerFinishTime;
}
bool WaveMaker::waveHasArrived() {
	return SDL_GetTicks() > waveTimerFinishTime;
}

void WaveMaker::startWaveCounter() {
	Uint32 currTime = SDL_GetTicks();
	this->waveTimerStartTime = currTime;
	this->waveTimerFinishTime = currTime + this->waveDelta;
}

void WaveMaker::updateWaveDelta(int score) {
	this->waveDelta = (-7 * std::atan((0.01 * score) - 2) + 12.5) * 1000;
	
		//Sample function
		//- 8tan^-1(.004x - 1) + 14

}

void WaveMaker::render() {
	int frame = round(((SDL_GetTicks() - getWaveTimerStartTime()) / (1.0 * getWaveTimerFinishTime() - getWaveTimerStartTime())) * 44);
	this->resourceManager.textures[TextureWaveMeterSheet].render(VIEW_WIDTH - 49,
		VIEW_HEIGHT - 20, &(resourceManager.waveMeterClips[frame]));
}