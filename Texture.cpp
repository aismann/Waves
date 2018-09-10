#include "Texture.h"

Texture::Texture() {

}

Texture::Texture(SDL_Renderer& renderer) {
	// Initialize
	texture = NULL;
	width = 0;
	height = 0;

	this->renderer = &renderer;
}

Texture::~Texture() {
	// Deallocate
	free();
}

bool Texture::loadFromFile(std::string path) {
	// Free preexisting texture
	free();
	// End texture
	SDL_Texture* newTexture = NULL;
	// Load image at path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL) {
		printf("Unable to load image %s!\nSDL_image Error: %s\n",
			path.c_str(), IMG_GetError());
		SDL_FreeSurface(loadedSurface); // free useless surface
		return texture != NULL; // Will be 0 here
	}

	// Color key image
	SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format,
		0xFF, 0xFF, 0xFF));
	// Create texture from surface pixels
	newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
	if (newTexture == NULL) {
		printf("Unable to create texture from %s!\nSDL_Error %s\n",
			path.c_str(), SDL_GetError());
		SDL_FreeSurface(loadedSurface); // free useless surface
		return texture != NULL; // Will be 0 here
	}
	// Get image dimensions
	width = loadedSurface->w;
	height = loadedSurface->h;
	// Get rid of old loadedSurface
	SDL_FreeSurface(loadedSurface);
	// Return success
	texture = newTexture;
	return texture != NULL; // Will be 1 here
}

void Texture::free() {
	// Free texture if it exists
	if (texture != NULL) {
		SDL_DestroyTexture(texture);
		texture = NULL;
		width = 0;
		height = 0;
	}
}

void Texture::setColor(Uint8 red, Uint8 green, Uint8 blue) {
	// Modulate texture
	SDL_SetTextureColorMod(texture, red, green, blue);
}

void Texture::setBlendMode(SDL_BlendMode blending) {
	// Set blending function
	SDL_SetTextureBlendMode(texture, blending);
}

void Texture::setAlpha(Uint8 alpha) {
	// Modulate texture alpha
	SDL_SetTextureAlphaMod(texture, alpha);
}

void Texture::render(int x, int y, SDL_Rect* clip, double angle,
	SDL_Point* center, SDL_RendererFlip flip) {
	// Set rendering space and render to screen
	SDL_Rect renderQuad = { x,y,width,height };
	// Set clip rendering dimensions
	if (clip != NULL) {
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}
	SDL_RenderCopyEx(renderer, texture, clip, &renderQuad, angle, center, flip);
}

int Texture::getWidth() {
	return width;
}

int Texture::getHeight() {
	return height;
}

