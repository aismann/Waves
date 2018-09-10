#pragma once
#include <SDL.h>

#include "Constants.h"
#include "ResourceManager.h"

class EyeCandyManager {
	int SURFACE_WATER_ANIMATION_TIME_PER_FRAME = 2000 / 12;

public:
	EyeCandyManager(SDL_Renderer& r, ResourceManager& rm);
	~EyeCandyManager();

	void renderSurfaceWater(bool isForeground = false);

private:
	Uint32 lastFrameStart;
	Uint32 lastFrameNumber;
	SDL_Renderer& renderer;
	ResourceManager& resourceManager;
};