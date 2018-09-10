#pragma once
#include <vector>
#include <random>
#include <chrono>
#include "ResourceManager.h"
#include "Grid.h"

class Silhouette {
public:
	Silhouette(SDL_Renderer& renderer, ResourceManager& rm, Grid& grid);
	~Silhouette();

	void setTileX(int x);
	void setTileY(int y);

	int getTileX();
	int getTileY();
	bool getValidity();

	void resetToTop();
	void moveToLowestPosition();
	void moveToHighestPosition();
	int isOffscreenSideways();
	int isOffscreenVertically();
	bool isValidPosition();
	bool canPlaceHere();
	void updateValidity();

	int moveUp();
	int moveDown();
	int moveLeft();
	int moveRight();
	int rotateLeft();
	int rotateRight();

	void placeTile();

	void resetPieces();
	char getNextPiece();
	void updateShapeArray();

	int getFrameDelta(Uint32 lastTime);
	Uint32 getLastCheckedValidity();
	bool isNeedToCheckValidity();

	int determineTextureIndex(int i, int j);

	void render(int cameraOffset);

private:
	int tileX;
	int tileY;
	int rotationIndex;
	int offset;
	bool validity;
	Uint32 lastDropped = -1;
	bool needToCheckValidity = false;
	Uint32 lastCheckedValidity = -1;
	char shape;
	std::string(*shapeTemplate)[5];
	std::vector<char> piecesAllowed;

	//unsigned seed; // seed used for random numbers

	SDL_Renderer& renderer;
	ResourceManager& resourceManager;
	Grid& grid;
};