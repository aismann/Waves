#pragma once
#include "ResourceManager.h"
#include "Constants.h"

struct Tile {
	int tile = 0;
	int offset = 0;
	Uint32 lastDropped = -1;
	int reservedForCat = 0;
};

class Grid {
public:
	Grid(SDL_Renderer& renderer, ResourceManager& rm);
	~Grid();

	int getFrameDelta(Uint32 lastTime);

	void renderTiles(int cameraOffset);
	void updateAdjacentTiles(int column, int row);
	void updateAllTiles();
	void updateTile(int column, int row);

	void shiftTilesLeft();
	void removeBottomRow();
	void moveTileToLowest(int x, int y);

	int isTileAt(int x, int y);
	int isOccupied(int x, int y);
	int isNotOccupied(int x, int y);
	void setTile(int column, int row, int value);
	int getOffset(int x, int y);

	bool isTileWet(int x, int y);
	bool isTileStable(int x, int y);
	void dropUnstableTiles();
	void setStable(int x, int y, bool (&stableArray)[MAX_COLUMNS][MAX_ROWS]);

private:
	Uint32 timerStartTime;

	Tile tiles[VIEW_WIDTH/16][VIEW_HEIGHT/16];
	SDL_Renderer& renderer;
	ResourceManager& resourceManager;
};