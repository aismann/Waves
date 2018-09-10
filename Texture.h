#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <string>

class Texture {
public:
	// Basic constructor sans renderer
	Texture();
	// Init vars
	Texture(SDL_Renderer &renderer);
	// Dealloc memory
	~Texture();
	// Load image from file path
	bool loadFromFile(std::string path);
	// Dealloc texture
	void free();
	// Set color modulation
	void setColor(Uint8 red, Uint8 green, Uint8 blue);
	// Set blending
	void setBlendMode(SDL_BlendMode blending);
	// Set alpha mod
	void setAlpha(Uint8 alpha);
	// Render texture at given point
	void render(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0,
		SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
	// Get image dimensions
	int getWidth();
	int getHeight();
private:
	// Actual texture in memory
	SDL_Texture* texture;
	// Image dimensions
	int width, height;

	// The window renderer
	SDL_Renderer* renderer;
};