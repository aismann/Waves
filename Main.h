#pragma once
#include <vector>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include "ResourceManager.h"
#include "Constants.h"
#include "Texture.h"
#include "Grid.h"
#include "Silhouette.h"
#include "Cat.h"
#include "Wavemaker.h"
#include "EyeCandyManager.h"

#include "Button.h"

enum screens {
	STARTSCREEN = 0,
	GAMESCREEN,
	SHOPSCREEN,
	RETRYSCREEN
};

int getFrameDelta(Uint32 timerStartTime);

// Starts SDL and creates a window
bool init();

// Sets up window
SDL_Window* setupWindow();
// Sets up renderer
SDL_Renderer* setupRenderer(SDL_Window&);

// Loads media
bool loadMedia(ResourceManager& rm, SDL_Renderer& renderer);

// Frees media and shuts down SDL
void close(SDL_Renderer* renderer, SDL_Window* window, ResourceManager& rm);

// Game screens
int startScreen(SDL_Renderer* renderer, SDL_Window* window, ResourceManager& rm);
int shopScreen(SDL_Renderer* renderer, SDL_Window* window, ResourceManager& rm);
int gameScreen(SDL_Renderer* renderer, SDL_Window* window, ResourceManager& rm);
int retryScreen(SDL_Renderer* renderer, SDL_Window* window, ResourceManager& rm);

// Initial setup
int main(int argc, char* args[]);