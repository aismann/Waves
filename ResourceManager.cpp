#include "ResourceManager.h"

ResourceManager::ResourceManager(SDL_Renderer& r) : renderer(r) {

}

void ResourceManager::setTexture(Texture& t, int num) {
	this->textures[num] = t;
}

void ResourceManager::clearTexture(int num) {
	textures[num].free();
}

void ResourceManager::clearAllTextures() {
	for (int i = 0; i < TextureTotal; i++) {
		clearTexture(i);
	}
}

void ResourceManager::clearFonts() {
	for (int i = 0; i < FontTotal; i++) {
		FC_FreeFont(this->fonts[i]);
	}
}

void ResourceManager::initializeBlockTileClips() {
	blockClips[0].x = 0;
	blockClips[0].y = 0;
	blockClips[0].w = 0;
	blockClips[0].h = 0;
	blockClips[1].x = 16;
	blockClips[1].y = 0;
	blockClips[1].w = 16;
	blockClips[1].h = 16;
	blockClips[2].x = 32;
	blockClips[2].y = 0;
	blockClips[2].w = 16;
	blockClips[2].h = 16;
	blockClips[3].x = 48;
	blockClips[3].y = 0;
	blockClips[3].w = 16;
	blockClips[3].h = 16;
	blockClips[4].x = 16;
	blockClips[4].y = 16;
	blockClips[4].w = 16;
	blockClips[4].h = 16;
	blockClips[5].x = 32;
	blockClips[5].y = 16;
	blockClips[5].w = 16;
	blockClips[5].h = 16;
	blockClips[6].x = 48;
	blockClips[6].y = 16;
	blockClips[6].w = 16;
	blockClips[6].h = 16;
	blockClips[7].x = 64;
	blockClips[7].y = 0;
	blockClips[7].w = 16;
	blockClips[7].h = 16;
	blockClips[8].x = 64;
	blockClips[8].y = 16;
	blockClips[8].w = 16;
	blockClips[8].h = 16;
	blockClips[9].x = 80;
	blockClips[9].y = 0;
	blockClips[9].w = 16;
	blockClips[9].h = 16;
	blockClips[10].x = 80;
	blockClips[10].y = 16;
	blockClips[10].w = 16;
	blockClips[10].h = 16;
}

void ResourceManager::initializeSilhouetteTileClips() {
	int index = 0;
	for (int j = 0; j < 64; j += 16) {
		for (int i = 0; i < 192; i += 16) {
			silhouetteClips[index].x = i;
			silhouetteClips[index].y = j;
			silhouetteClips[index].w = 16;
			silhouetteClips[index].h = 16;
			index++;
		}
	}
}
void ResourceManager::initializeSilhouetteInvalidTileClips() {
	int index = 0;
	for (int j = 0; j < 64; j+=16) {
		for (int i = 0; i < 192; i+=16) {
			silhouetteInvalidClips[index].x = i;
			silhouetteInvalidClips[index].y = j;
			silhouetteInvalidClips[index].w = 16;
			silhouetteInvalidClips[index].h = 16;
			index++;
		}
	}
}
void ResourceManager::initializeWaveMeterClips() {
	for (int i = 0; i < 45; i++) {
		waveMeterClips[i].x = 0;
		waveMeterClips[i].y = i * 19;
		waveMeterClips[i].w = 48;
		waveMeterClips[i].h = 19;
	}
}

void ResourceManager::initializeWaterSurfaceClips() {
	for (int i = 0; i < 7; i++) {
		waterSurfaceClips[i].x = i * 32;
		waterSurfaceClips[i].y = 0;
		waterSurfaceClips[i].w = 32;
		waterSurfaceClips[i].h = 8;
	}
	for (int i = 7; i < 12; i++) {
		waterSurfaceClips[i].x = 192 - (i-6) * 32;
		waterSurfaceClips[i].y = 0;
		waterSurfaceClips[i].w = 32;
		waterSurfaceClips[i].h = 8;
	}
}

void ResourceManager::initializeFonts() {
	FC_Font* fontDef24 = FC_CreateFont();
	FC_LoadFont(fontDef24, &renderer, "./Assets/wobbles.ttf", 24, 
		FC_MakeColor(255, 255, 255, 255),	TTF_STYLE_NORMAL);
	this->fonts[FontDef24] = fontDef24;

	FC_Font* fontDef12 = FC_CreateFont();
	FC_LoadFont(fontDef12, &renderer, "./Assets/wobbles.ttf", 12,
		FC_MakeColor(255, 255, 255, 255), TTF_STYLE_NORMAL);
	this->fonts[FontDef12] = fontDef12;
}