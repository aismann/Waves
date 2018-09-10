#include "EyeCandyManager.h"

EyeCandyManager::EyeCandyManager(SDL_Renderer& r, ResourceManager& rm) :
	resourceManager(rm), renderer(r) {
	lastFrameStart = -1;
}

EyeCandyManager::~EyeCandyManager() {

}

void EyeCandyManager::renderSurfaceWater(bool isForeground) {

	int framesToAdvance = floor((SDL_GetTicks() - lastFrameStart) / ((1.0) * SURFACE_WATER_ANIMATION_TIME_PER_FRAME));
	int nextFrame = (lastFrameNumber + framesToAdvance) % 12;
	if (framesToAdvance != 0) {
		lastFrameStart = SDL_GetTicks();
		lastFrameNumber = nextFrame;
	}
	if (!isForeground) {
		for (int i = 0; i <= SCREEN_WIDTH; i += 32) {
			this->resourceManager.textures[TextureWaterSurfaceSheet].render(i,
				VIEW_HEIGHT - 16 * 4 - 8, &(resourceManager.waterSurfaceClips[nextFrame]));
		}
	}
	else {
		for (int i = 0; i <= SCREEN_WIDTH; i += 32) {
			this->resourceManager.textures[TextureWaterSurfaceTransparentSheet].render(i,
				VIEW_HEIGHT - 16 * 4 - 8, &(resourceManager.waterSurfaceClips[nextFrame]));
		}
	}
	
}