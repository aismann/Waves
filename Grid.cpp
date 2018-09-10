#include "Grid.h"

Grid::Grid(SDL_Renderer& r, ResourceManager& rm) :
	renderer(r), resourceManager(rm) {
	// initialize tiles to empty
	//this->resourceManager = rm;

	for (int i = 0; i < VIEW_WIDTH; i += 16) {
		for (int j = 0; j < VIEW_HEIGHT; j += 16) {
			tiles[i / 16][j / 16].tile = 0;
		}
	}

	tiles[2][MAX_ROWS - 1].tile = 1;
	tiles[2][MAX_ROWS - 2].tile = 1;
	tiles[2][MAX_ROWS - 3].tile = 1;
	tiles[2][MAX_ROWS - 4].tile = 1;
	tiles[2][MAX_ROWS - 5].tile = 1;
	tiles[2][MAX_ROWS - 6].tile = 1;
	tiles[3][MAX_ROWS - 1].tile = 1;
	tiles[3][MAX_ROWS - 2].tile = 1;
	tiles[3][MAX_ROWS - 3].tile = 1;
	tiles[3][MAX_ROWS - 4].tile = 1;
	tiles[3][MAX_ROWS - 5].tile = 1;
	tiles[3][MAX_ROWS - 6].tile = 1;
	tiles[4][MAX_ROWS - 1].tile = 1;
	tiles[4][MAX_ROWS - 2].tile = 1;
	tiles[4][MAX_ROWS - 3].tile = 1;
	tiles[4][MAX_ROWS - 4].tile = 1;
	tiles[4][MAX_ROWS - 5].tile = 1;
	tiles[4][MAX_ROWS - 6].tile = 1;

	updateAllTiles();
}

Grid::~Grid() {

}

int Grid::getFrameDelta(Uint32 lastTime) {
	if (lastTime > SDL_GetTicks()) return 0;
	Uint32 tickDelta = SDL_GetTicks() - lastTime;
	return tickDelta / MILLISECONDS_PER_FRAME;
}

void Grid::renderTiles(int cameraOffset) {
	for (int i = 0; i < VIEW_WIDTH; i+=16) {
		for (int j = 0; j < VIEW_HEIGHT; j+=16) {
			if (tiles[i / 16][j / 16].offset < 0) {
				// Update tile offset
				int pixelsDown = 16 * (getFrameDelta(tiles[i/16][j/16].lastDropped) / (FRAMES_PER_SECOND * 0.25));
				if (pixelsDown > 0) {
					tiles[i / 16][j / 16].offset += pixelsDown;
					tiles[i / 16][j / 16].lastDropped = SDL_GetTicks();
					if (tiles[i / 16][j / 16].offset > 0)
						tiles[i / 16][j / 16].offset = 0;
				}
				
			}
			resourceManager.textures[TextureBlock].render(i + cameraOffset, j + tiles[i/16][j/16].offset,
				&(resourceManager.blockClips[tiles[i/16][j/16].tile]));
		}
	}
}

void Grid::updateAllTiles() {
	for (int i = 0; i < VIEW_WIDTH / 16; i++) {
		for (int j = 0; j < VIEW_HEIGHT / 16; j++) {
			updateTile(i, j);
		}
	}
}

void Grid::updateAdjacentTiles(int column, int row) {
	if (column > 0)
		updateTile(column - 1, row);
	if (column < MAX_COLUMNS - 1)
		updateTile(column + 1, row);
	if (row > 0)
		updateTile(column, row - 1);
	if (row < MAX_ROWS - 1)
		updateTile(column, row + 1);
	if (column > 0 && row > 0)
		updateTile(column - 1, row - 1);
	if (column < MAX_COLUMNS - 1 && row > 0)
		updateTile(column + 1, row - 1);
	if (column > 0 && row < MAX_ROWS - 1)
		updateTile(column - 1, row + 1);
	if (column < MAX_COLUMNS - 1 && row < MAX_ROWS - 1)
		updateTile(column + 1, row + 1);
}

void Grid::updateTile(int column, int row) {
	if (column >= MAX_COLUMNS || column < 0 || row >= MAX_ROWS || row < 0) {
		return;
	}

	if (tiles[column][row].tile < 0)
		return;

	int c = 0, d = 0;
	// Valid tile in grid, so update accordingly
	if (column != 0 && column != MAX_COLUMNS - 1) c += 1;
	else if (column == 0) c += 2;
	else if (column == MAX_COLUMNS - 1)	c += 3;

	if (row != 0 && row != MAX_ROWS - 1) c += 10;
	else if (row == 0) c += 20;
	else if (row == MAX_ROWS - 1) c += 30;

	// Determine if there are blocks on the sides of the current block
	switch (c) {
	case 11: // A center tile
		//if (tiles[column - 1][row - 1] != 0) d += 100000000;
		if (tiles[column][row - 1].tile > 0) d += 010000000;
		//if (tiles[column + 1][row - 1] != 0) d += 001000000;
		if (tiles[column - 1][row].tile > 0) d += 000100000;
		if (tiles[column + 1][row].tile > 0) d += 000001000;
		//if (tiles[column - 1][row + 1] != 0) d += 000000100;
		if (tiles[column][row + 1].tile > 0) d += 000000010;
		//if (tiles[column + 1][row + 1] != 0) d += 000000001;
		break;
	case 12: // Left border
		if (tiles[column][row - 1].tile > 0) d += 010000000;
		//if (tiles[column + 1][row - 1] != 0) d += 001000000;
		if (tiles[column + 1][row].tile > 0) d += 000001000;
		if (tiles[column][row + 1].tile > 0) d += 000000010;
		//if (tiles[column + 1][row + 1] != 0) d += 000000001;
		break;
	case 13: // Right border
		//if (tiles[column - 1][row - 1] != 0) d += 100000000;
		if (tiles[column][row - 1].tile > 0) d += 010000000;
		if (tiles[column - 1][row].tile > 0) d += 000100000;
		//if (tiles[column - 1][row + 1] != 0) d += 000000100;
		if (tiles[column][row + 1].tile > 0) d += 000000010;
		break;
	case 21: // Top border
		if (tiles[column - 1][row].tile > 0) d += 000100000;
		if (tiles[column + 1][row].tile > 0) d += 000001000;
		//if (tiles[column - 1][row + 1] != 0) d += 000000100;
		if (tiles[column][row + 1].tile > 0) d += 000000010;
		//if (tiles[column + 1][row + 1] != 0) d += 000000001;
		break;
	case 22: // Top left corner
		if (tiles[column + 1][row].tile > 0) d += 000001000;
		if (tiles[column][row + 1].tile > 0) d += 000000010;
		//if (tiles[column + 1][row + 1] != 0) d += 000000001;
		break;
	case 23: // Top right corner
		if (tiles[column - 1][row].tile > 0) d += 000100000;
		if (tiles[column][row + 1].tile > 0) d += 000000010;
		//if (tiles[column + 1][row + 1] != 0) d += 000000001;
		break;
	case 31: // Bottom border
		//if (tiles[column - 1][row - 1] != 0) d += 100000000;
		if (tiles[column][row - 1].tile > 0) d += 010000000;
		//if (tiles[column + 1][row - 1] != 0) d += 001000000;
		if (tiles[column - 1][row].tile > 0) d += 000100000;
		if (tiles[column + 1][row].tile > 0) d += 000001000;
		break;
	case 32: // Bottom left corner
		if (tiles[column][row - 1].tile > 0) d += 010000000;
		//if (tiles[column + 1][row - 1] != 0) d += 001000000;
		if (tiles[column + 1][row].tile > 0) d += 000001000;
		break;
	case 33: // bottom right corner
		//if (tiles[column - 1][row - 1] != 0) d += 100000000;
		if (tiles[column][row - 1].tile > 0) d += 010000000;
		if (tiles[column - 1][row].tile > 0) d += 000100000;
		break;
	}
	if (tiles[column][row].tile > 0) d += 000010000;

	// Change graphic depending on adjacent blocks
	switch (d) {
	
	case 000010000: tiles[column][row].tile = BlockSingleTop; break;

	case 000110000: tiles[column][row].tile = BlockTopRight; break;
	case 000011000: tiles[column][row].tile = BlockTopLeft; break;
	case 010010000: tiles[column][row].tile = BlockSingleBot; break;
	case 000010010: tiles[column][row].tile = BlockSingleTop; break;

	case 010110000: tiles[column][row].tile = BlockBottomRightCorner; break;
	case 010011000: tiles[column][row].tile = BlockBottomLeftCorner; break;
	case 000011010: tiles[column][row].tile = BlockTopLeft; break;
	case 000110010: tiles[column][row].tile = BlockTopRight; break;
	case 000111000: tiles[column][row].tile = BlockTopCenter; break;
	case 010010010: tiles[column][row].tile = BlockSingleBot; break;

	case 010111000: tiles[column][row].tile = BlockCenter; break;
	case 000111010: tiles[column][row].tile = BlockTopCenter; break;
	case 010011010: tiles[column][row].tile = BlockLeft; break;
	case 010110010: tiles[column][row].tile = BlockRight; break;

	case 010111010: tiles[column][row].tile = BlockCenter; break;
	default: //tiles[column][row] = BlockEmpty; 
		break;
	}
}

void Grid::shiftTilesLeft() {
	for (int i = 0; i < VIEW_WIDTH / 16 - 1; i++) {
		for (int j = 0; j < VIEW_HEIGHT / 16; j++) {
			tiles[i][j] = tiles[i + 1][j];
		}
	}
	// Right column
	for (int j = 0; j < VIEW_HEIGHT / 16; j++) {
		tiles[VIEW_WIDTH / 16 - 1][j] = Tile();
	}
}

void Grid::removeBottomRow() {
	// Remove bottom row
	for (int i = 0; i < VIEW_WIDTH / 16; i++) {
		tiles[i][VIEW_HEIGHT / 16 - 1] = Tile();
	}
	// Shift upper rows down
	for (int i = VIEW_HEIGHT / 16 - 1; i > 0; i--) {
		for (int j = 0; j < VIEW_WIDTH / 16; j++) {
			tiles[j][i] = tiles[j][i - 1];
			tiles[j][i].offset = -16;
			tiles[j][i].lastDropped = SDL_GetTicks();
		}
	}
	// Top row
	for (int j = 0; j < VIEW_WIDTH / 16; j++) {
		tiles[j][0] = Tile();
	}

}

void Grid::moveTileToLowest(int x, int y) {
	int tilesFallen = 0;
	while (y < MAX_ROWS - 1 && tiles[x][y+1].tile == 0 && tiles[x][y].tile > 0) {
		tiles[x][y + 1] = tiles[x][y];
		tiles[x][y] = Tile();
		y++;
		tilesFallen++;
	}
	tiles[x][y].offset += tilesFallen * -16;
	tiles[x][y].lastDropped = SDL_GetTicks();
}

int Grid::isTileAt(int x, int y) {
	if (x < 0 || x > MAX_COLUMNS)
		return false;
	if (y < 0 || y > MAX_COLUMNS)
		return false;
	if (this->tiles[x][y].tile > 0)
		return true;
	return false;
}

int Grid::isOccupied(int x, int y) {
	if (x < 0 || x > MAX_COLUMNS)
		return 1;
	if (y < 0 || y > MAX_COLUMNS)
		return 1;
	if (this->tiles[x][y].tile != 0) // TODO Need more checks for cat etc
		return 1;
	return -1;
}

int Grid::isNotOccupied(int x, int y) {
	if (isOccupied(x, y) == 1)
		return -1;
	return 1;
}

void Grid::setTile(int column, int row, int value) {
	if (column < 0 || column >= MAX_COLUMNS || 
		row < 0 || row >= MAX_ROWS)
		return;

	this->tiles[column][row].tile = value;
	updateTile(column, row);
	updateAdjacentTiles(column, row);
}

int Grid::getOffset(int x, int y) {
	return tiles[x][y].offset;
}

bool Grid::isTileWet(int x, int y) {
	if (y < MAX_ROWS - 4)
		return false;
	return true;
}

bool Grid::isTileStable(int x, int y) {
	if (y = VIEW_HEIGHT / 16 - 1) return true;
	if (y < VIEW_HEIGHT / 16 && tiles[x][y + 1].tile > 0)
		return true;
	return false;
}

void Grid::dropUnstableTiles() {
	// Uses flow graph starting at seafloor to trace stability upwards
	// After stability flow created via DFS, drop unstable tiles
	// Setup flow graph
	bool stableArray[MAX_COLUMNS][MAX_ROWS] = {};
	for (int i = 0; i < MAX_COLUMNS; i++) {
		setStable(i, MAX_ROWS - 1, stableArray);
	}
	// Drop unstable tiles
	for (int i = 0; i < MAX_COLUMNS; i++) {
		for (int j = MAX_ROWS - 1; j >= 0; j--) {
			if (tiles[i][j].tile > 0 && stableArray[i][j] != 1)
				moveTileToLowest(i, j);
		}
	}
	// Set timer for beginning of fall animation
	this->timerStartTime = SDL_GetTicks();
}

void Grid::setStable(int x, int y, bool(&stableArray)[MAX_COLUMNS][MAX_ROWS]) {
	// If out of bounds
	if (x < 0 || x >= MAX_COLUMNS || y < 0 || y >= MAX_ROWS) return;
	// If not a tile
	if (tiles[x][y].tile <= 0) return;
	// If already declared stable
	if (stableArray[x][y] == 1) return;
	
	
	// if in water
	if (isTileWet(x, y)) {
		if (stableArray[x][y + 1] == 1 || y == MAX_ROWS - 1)
			stableArray[x][y] = 1;
		if (isTileAt(x, y - 1)) {
			setStable(x, y - 1, stableArray);
		}
	}
	// else in air
	else {
		stableArray[x][y] = 1;
		if (isTileAt(x - 1, y)) setStable(x - 1, y, stableArray);
		if (isTileAt(x, y - 1)) setStable(x, y - 1, stableArray);
		if (isTileAt(x + 1, y)) setStable(x + 1, y, stableArray);
		if (isTileAt(x, y + 1) && !isTileWet(x, y + 1)) setStable(x, y + 1, stableArray);
	}
	
}