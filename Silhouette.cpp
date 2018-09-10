#include "Silhouette.h"

std::string S_SHAPE_TEMPLATE[4][5] = {
	{
		"     ",
		"     ",
		"  OO ",
		" OO  ",
		"     "},
	{
		"     ",
		"  O  ",
		"  OO ",
		"   O ",
		"     " },
		{},
		{}
};
std::string Z_SHAPE_TEMPLATE[4][5] = {
	{
		"     ",
		"     ",
		" OO  ",
		"  OO ",
		"     "},
	{
		"     ",
		"   O ",
		"  OO ",
		"  O  ",
		"     " },
		{},
		{}
};
std::string I_SHAPE_TEMPLATE[4][5] = {
	{
		"  O  ",
		"  O  ",
		"  O  ",
		"  O  ",
		"     " },
	{
		"     ",
		"     ",
		"OOOO ",
		"     ",
		"     " },
		{},
		{}
};
std::string O_SHAPE_TEMPLATE[4][5] = {
	{
		"     ",
		"     ",
		" OO  ",
		" OO  ",
		"     " },
		{},
		{},
		{}
};
std::string J_SHAPE_TEMPLATE[4][5] = {
	{
		"     ",
		"     ",
		" O   ",
		" OOO ",
		"     " },
	{
		"     ",
		"  OO ",
		"  O  ",
		"  O  ",
		"     " }, 
	{
		"     ",
		"     ",
		" OOO ",
		"   O ",
		"     " },
	{
		"     ",
		"  O  ",
		"  O  ",
		" OO  ",
		"     " }
};
std::string L_SHAPE_TEMPLATE[4][5] = {
	{
		"     ",
		"     ",
		"   O ",
		" OOO ",
		"     " },
	{
		"     ",
		"  O  ",
		"  O  ",
		"  OO ",
		"     " }, 
	{
		"     ",
		"     ",
		" OOO ",
		" O   ",
		"     " },
	{
		"     ",
		" OO  ",
		"  O  ",
		"  O  ",
		"     " }
};
std::string T_SHAPE_TEMPLATE[4][5] = {
	{
		"     ",
		"  O  ",
		" OOO ",
		"     ",
		"     " },
	{
		"     ",
		"  O  ",
		"  OO ",
		"  O  ",
		"     " }, 
	{
		"     ",
		"     ",
		" OOO ",
		"  O  ",
		"     " },
	{
		"     ",
		"  O  ",
		" OO  ",
		"  O  ",
		"     " }
};


Silhouette::Silhouette(SDL_Renderer& r, ResourceManager& rm, Grid& grid) : 
	renderer(r), resourceManager(rm), grid(grid) {
	this->tileX = MAX_COLUMNS / 2 - 2;
	this->tileY = 0;
	this->offset = 0;
	this->lastDropped = -1;
	this->validity = false;

	// Setup valid pieces
	//this->seed = std::chrono::system_clock::now().time_since_epoch().count();

	resetPieces();
	this->shape = getNextPiece();

	//this->shape = 'J'; // for testing
	this->updateShapeArray();

	moveToLowestPosition();

	
}

Silhouette::~Silhouette() {

}

void Silhouette::setTileX(int x) {
	this->tileX = x;
}
void Silhouette::setTileY(int y) {
	this->tileY = y;
}

int Silhouette::getTileX() {
	return this->tileX;
}
int Silhouette::getTileY() {
	return this->tileY;
}
bool Silhouette::getValidity() {
	return this->validity;
}

int Silhouette::moveUp() {
	int lastPosition = this->tileY;
	int yCounter = this->tileY;
	while (!isOffscreenVertically()) {
		this->tileY--;
		yCounter = tileY;
		moveToLowestPosition();
		if (tileY != lastPosition && isValidPosition())
			break;
		else tileY = yCounter;
	}
	moveToLowestPosition();
	if (tileY == lastPosition || !isValidPosition())
		moveToHighestPosition();
	return 1;
}
int Silhouette::moveDown() {
	this->tileY++;
	while (!isValidPosition() && this->tileY < MAX_ROWS)
		this->tileY++;
	if (this->tileY >= MAX_ROWS)
		this->resetToTop();
	
	moveToLowestPosition();
	return 1;
}
int Silhouette::moveLeft() {
	this->tileX--;
	if (isOffscreenSideways()) {
		this->tileX++;
		return -1;
	}

	while (!isValidPosition() && this->tileY >= 0)
		this->tileY--;
	moveToLowestPosition();
	return 1;
}
int Silhouette::moveRight() {
	this->tileX++;
	if (isOffscreenSideways()) {
		this->tileX--;
		return -1;
	}

	while (!isValidPosition() && this->tileY >= 0)
		this->tileY--;
	moveToLowestPosition();
	return 1;
}
int Silhouette::rotateLeft() {
	int oldIndex = this->rotationIndex;
	this->rotationIndex++;

	if (rotationIndex > 3 || this->shapeTemplate[rotationIndex][0] == "") {
		rotationIndex = 0;
	}
	updateShapeArray();
	
	if (!isValidPosition()) // try moving up one
		this->tileY--;
	if (!isValidPosition()) // try moving up one and left one
		this->tileX--;
	if (!isValidPosition()) // try moving up one and right one
		this->tileX += 2;

	
	if (!isValidPosition()) {
		this->tileY++;
		this->tileX--;
		this->rotationIndex = oldIndex;
		updateShapeArray();
		return -1;
	}

	//this->rotationIndex++;

	//updateShapeArray();
	moveToLowestPosition();
	return 1;
}
int Silhouette::rotateRight() {
	int oldIndex = this->rotationIndex;
	this->rotationIndex--;

	if (this->rotationIndex < 0) {
		rotationIndex = 4;
		do {
			rotationIndex--;
		} while (this->shapeTemplate[rotationIndex][0] == "");
	}
	updateShapeArray();

	if (!isValidPosition()) // try moving up one
		this->tileY--;
	if (!isValidPosition()) // try moving up one and left one
		this->tileX--;
	if (!isValidPosition()) // try moving up one and right one
		this->tileX += 2;

	if (!isValidPosition()) {
		this->tileY++;
		this->tileX--;
		this->rotationIndex = oldIndex;
		updateShapeArray();
		return -1;
	}

	moveToLowestPosition();
	return 1;
}

void Silhouette::resetToTop() {
	this->tileY = -4;
}

void Silhouette::moveToLowestPosition() {
	//this->tileY = 0;
	while (isValidPosition() && this->tileY < MAX_ROWS)
		this->tileY++;
	this->tileY--;
	this->updateValidity();
}

void Silhouette::moveToHighestPosition() {
	this->tileY = MAX_ROWS;
	while (!isValidPosition() && this->tileY > 0)
		this->tileY--;
	this->updateValidity();
}

int Silhouette::isOffscreenSideways() {
	int isOffscreen = 0;
	std::string(*s)[5] = this->shapeTemplate;
	if ((*s) == NULL) return isOffscreen;
	for (int i = 0; i < (*s)->length(); i++) { // i is the row as a string
		if ((*s) == NULL) continue;
		for (int j = 0; j < (*s)[i].length(); j++) { // j is the character in the string
			if ((*s)[i][j] != ' ') { // there is an O in the string
									 // if there is a block at this location of the grid
				//if (i + this->tileY < 1)
				//	isOffscreen = -2;
				if (this->tileX + j < 0)
					isOffscreen = -1;
				else if (this->tileX + j >= MAX_COLUMNS)
					isOffscreen = 1;
			}
		}
	}
	return isOffscreen;
}

int Silhouette::isOffscreenVertically() {
	bool isOffscreen = 0;
	std::string(*s)[5] = this->shapeTemplate;
	if ((*s) == NULL) return isOffscreen;
	for (int i = 0; i < (*s)->length(); i++) { // i is the row as a string
		if ((*s) == NULL) continue;
		for (int j = 0; j < (*s)[i].length(); j++) { // j is the character in the string
			if ((*s)[i][j] != ' ') { // there is an O in the string
									 // if there is a block at this location of the grid
									 //if (i + this->tileY < 1)
									 //	isOffscreen = -2;
				if (this->tileY + i < 2)
					isOffscreen = -1;
				else if (this->tileY + i >= MAX_ROWS)
					isOffscreen = 1;
			}
		}
	}
	return isOffscreen;
}

bool Silhouette::isValidPosition() {
	bool isValid = true;
	//std::string(*s)[5] = this->shapeTemplate[this->rotationIndex];
	std::string(*s)[5] = this->shapeTemplate;
	//if ((*s) == NULL) return isValid;
	for (int i = 0; i < (*s)->length(); i++) { // i is row of shape as string
		//if ((*s) == NULL) continue;
		for (int j = 0; j < (*s)[i].length(); j++) { // j is col of shape string
			if ((*s)[i][j] != ' ') { // there is an O in the string
				// if there is a block at this location of the grid
				if (this->grid.isTileAt(this->tileX + j, this->tileY + i) == 1)
					isValid = false;
				if (this->tileY + i >= MAX_ROWS)
					isValid = false;
			}
		}
	}
	return isValid;
}

bool Silhouette::canPlaceHere() {
	bool isValid = true;
	std::string(*s)[5] = this->shapeTemplate;
	//if ((*s) == NULL) return isValid;
	for (int i = 0; i < (*s)->length(); i++) { // i is row of shape as string
		//if ((*s) == NULL) continue;
		for (int j = 0; j < (*s)[i].length(); j++) { // j is col of shape string
			if ((*s)[i][j] != ' ') {
				// if there is anything at this location of the grid
				if (this->grid.isOccupied(this->tileX + j, this->tileY + i) == 1)
					isValid = false;
			}

		}
	}
	return isValid;
}

void Silhouette::updateValidity() {
	this->validity = true;
	this->needToCheckValidity = false;

	if (!canPlaceHere())
		this->validity = false;
	if (isOffscreenSideways() || isOffscreenVertically())
		this->validity = false;
	
	// Check if on top of unstable tile
	std::string(*s)[5] = this->shapeTemplate;

	for (int i = 0; i < (*s)->length(); i++) { 
		for (int j = 0; j < (*s)[i].length(); j++) { 
			if ((*s)[i][j] != ' ') { 
				if (this->tileY + i + 1 < MAX_ROWS) {
					if (grid.isTileAt(this->tileX + j, this->tileY + i + 1) &&
						grid.getOffset(this->tileX + j, this->tileY + i + 1) < 0) {
						this->validity = false;
						this->needToCheckValidity = true;
					}	
				}	
			}
		}
	}
	this->lastCheckedValidity = SDL_GetTicks();
}


void Silhouette::placeTile() {
	if (this->validity == false)
		return;

	std::string(*s)[5] = this->shapeTemplate;

	for (int i = 0; i < (*s)->length(); i++) { // i is the row as a string
		for (int j = 0; j < (*s)[i].length(); j++) { // j is the character in the string
			if ((*s)[i][j] != ' ') { // there is an O in the string representing part of the tile
				this->resourceManager.textures[TextureBlockShadow].render((this->tileX + j)*BLOCK_SIZE, (this->tileY + i)*BLOCK_SIZE);
				this->grid.setTile(this->tileX + j, this->tileY + i, 1);
			}
		}
	}
	// Get next piece
	this->shape = getNextPiece();
	this->rotationIndex = 0;
	updateShapeArray();
	this->resetToTop();
	while (isOffscreenSideways() > 0)
		this->tileX--;
	while (isOffscreenSideways() < 0)
		this->tileX++;
	this->moveToLowestPosition();
}

void Silhouette::resetPieces() {
	this->piecesAllowed.insert(piecesAllowed.end(), 'I');
	this->piecesAllowed.insert(piecesAllowed.end(), 'J');
	this->piecesAllowed.insert(piecesAllowed.end(), 'L');
	this->piecesAllowed.insert(piecesAllowed.end(), 'O');
	this->piecesAllowed.insert(piecesAllowed.end(), 'S');
	this->piecesAllowed.insert(piecesAllowed.end(), 'T');
	this->piecesAllowed.insert(piecesAllowed.end(), 'Z');
}

char Silhouette::getNextPiece() {
	if (piecesAllowed.empty())
		resetPieces();
	// Generate next piece based on random number
	std::minstd_rand0 generator(std::chrono::system_clock::now().time_since_epoch().count()); // make generator
	unsigned int randomNumber = generator(); // get number
	randomNumber %= piecesAllowed.size(); // assign number relative to vector
	char nextPiece = piecesAllowed[randomNumber];
	// remove from options and return it
	piecesAllowed.erase(piecesAllowed.begin() + randomNumber); 
	return nextPiece;
}

void Silhouette::updateShapeArray() {

	switch (this->shape) {
	case 'I': this->shapeTemplate = &(I_SHAPE_TEMPLATE[this->rotationIndex]); break;
	case 'J': this->shapeTemplate = &J_SHAPE_TEMPLATE[this->rotationIndex]; break;
	case 'L': this->shapeTemplate = &L_SHAPE_TEMPLATE[this->rotationIndex]; break;
	case 'O': this->shapeTemplate = &O_SHAPE_TEMPLATE[this->rotationIndex]; break;
	case 'S': this->shapeTemplate = &S_SHAPE_TEMPLATE[this->rotationIndex]; break;
	case 'T': this->shapeTemplate = &T_SHAPE_TEMPLATE[this->rotationIndex]; break;
	case 'Z': this->shapeTemplate = &Z_SHAPE_TEMPLATE[this->rotationIndex]; break;
	}
}

int Silhouette::getFrameDelta(Uint32 lastTime) {
	if (lastTime > SDL_GetTicks()) return 0;
	Uint32 tickDelta = SDL_GetTicks() - lastTime;
	return tickDelta / MILLISECONDS_PER_FRAME;
}

Uint32 Silhouette::getLastCheckedValidity() {
	return this->lastCheckedValidity;
}

bool Silhouette::isNeedToCheckValidity() {
	return this->needToCheckValidity;
}

int Silhouette::determineTextureIndex(int row, int column) {
	// Optimization note: change int to a binary rep.

	// Determine location in shape template to avoid out of bounds checks
	if (column >= 5 || column < 0 || row >= 5 || row < 0) {
		return -1;
	}

	int c = 0, d = 0;
	// Valid tile in grid, so update accordingly
	if (row != 0 && row != 4) c += 1;
	else if (row == 0) c += 2;
	else if (row == 4)	c += 3;

	if (column != 0 && column != 4) c += 10;
	else if (column == 0) c += 20;
	else if (column == 4) c += 30;

	std::string(*s)[5] = this->shapeTemplate;

	// Determine if there are blocks on the sides of the current block
	switch (c) {
	case 11: // A center tile
		if ((*s)[row - 1][column - 1] != ' ') d += 0100000000;
		if ((*s)[row][column - 1] != ' ') d += 000100000;
		if ((*s)[row + 1][column - 1] != ' ') d += 000000100;
		if ((*s)[row - 1][column] != ' ') d += 0010000000;
		if ((*s)[row + 1][column] != ' ') d += 000000010;
		if ((*s)[row - 1][column + 1] != ' ') d += 001000000;
		if ((*s)[row][column + 1] != ' ') d += 000001000;
		if ((*s)[row + 1][column + 1] != ' ') d += 000000001;
		break;
	case 12: // 
		if ((*s)[row][column - 1] != ' ') d += 000100000;
		if ((*s)[row + 1][column - 1] != ' ') d += 000000100;
		if ((*s)[row + 1][column] != ' ') d += 000000010;
		if ((*s)[row][column + 1] != ' ') d += 000001000;
		if ((*s)[row + 1][column + 1] != ' ') d += 000000001;
		break;
	case 13: // 
		if ((*s)[row - 1][column - 1] != ' ') d += 0100000000;
		if ((*s)[row][column - 1] != ' ') d += 000100000;
		if ((*s)[row - 1][column] != ' ') d += 010000000;
		if ((*s)[row - 1][column + 1] != ' ') d += 001000000;
		if ((*s)[row][column + 1] != ' ') d += 000001000;
		break;
	case 21: // 
		if ((*s)[row - 1][column] != ' ') d += 010000000;
		if ((*s)[row + 1][column] != ' ') d += 000000010;
		if ((*s)[row - 1][column + 1] != ' ') d += 001000000;
		if ((*s)[row][column + 1] != ' ') d += 000001000;
		if ((*s)[row + 1][column + 1] != ' ') d += 000000001;
		break;
	case 22: // 
		if ((*s)[row + 1][column] != ' ') d += 000000010;
		if ((*s)[row][column + 1] != ' ') d += 000001000;
		if ((*s)[row + 1][column + 1] != ' ') d += 000000001;
		break;
	case 23: // 
		if ((*s)[row - 1][column] != ' ') d += 010000000;
		if ((*s)[row][column + 1] != ' ') d += 000001000;
		if ((*s)[row - 1][column - 1] != ' ') d += 0100000000;
		break;
	case 31: // 
		if ((*s)[row - 1][column - 1] != ' ') d += 0100000000;
		if ((*s)[row][column - 1] != ' ') d += 000100000;
		if ((*s)[row + 1][column - 1] != ' ') d += 000000100;
		if ((*s)[row - 1][column] != ' ') d += 010000000;
		if ((*s)[row + 1][column] != ' ') d += 000000010;
		break;
	case 32: // 
		if ((*s)[row][column - 1] != ' ') d += 000100000;
		if ((*s)[row + 1][column - 1] != ' ') d += 000000100;
		if ((*s)[row + 1][column] != ' ') d += 000000010;
		break;
	case 33: // 
		if ((*s)[row - 1][column - 1] != ' ') d += 0100000000;
		if ((*s)[row][column - 1] != ' ') d += 000100000;
		if ((*s)[row - 1][column] != ' ') d += 010000000;
		break;
	}
	if ((*s)[row][column] != ' ') d += 000010000;

	// Change graphic depending on adjacent blocks
	switch (d) {
	case 0000010000: return TileSingle; break;
	case 0000010001: return TileSingle; break;
	case 0000010010: return TilePipeTop; break;
	case 0000010011: return TilePipeTop; break;
	case 0000010100: return TileSingle; break;
	case 0000010101: return TileSingle; break;
	case 0000010110: return TilePipeTop; break;
	case 0000010111: return TilePipeTop; break;
	case 0000011000: return TilePipeLeft; break;
	case 0000011001: return TilePipeLeft; break;
	case 0000011010: return TilePipeTopLeft; break;
	case 0000011011: return TileTopLeft; break;
	case 0000011100: return TilePipeLeft; break;
	case 0000011101: return TilePipeLeft; break;
	case 0000011110: return TilePipeTopLeft; break;
	case 0000011111: return TileTopLeft; break;
	case 0000110000: return TilePipeRight; break;
	case 0000110001: return TilePipeRight; break;
	case 0000110010: return TilePipeTopRight; break;
	case 0000110011: return TilePipeTopRight; break;
	case 0000110100: return TilePipeRight; break;
	case 0000110101: return TilePipeRight; break;
	case 0000110110: return TileTopRight; break;
	case 0000110111: return TileTopRight; break;
	case 0000111000: return TilePipeHorizontal; break;
	case 0000111001: return TilePipeHorizontal; break;
	case 0000111010: return TilePipeTTop; break;
	case 0000111011: return TilePipeTopRight; break;
	case 0000111100: return TilePipeHorizontal; break;
	case 0000111101: return TilePipeHorizontal; break;
	case 0000111110: return TilePipeTopLeft; break;
	case 0000111111: return TileTopCenter; break;
	case 0001010000: return TileSingle; break;
	case 0001010001: return TileSingle; break;
	case 0001010010: return TilePipeTop; break;
	case 0001010011: return TilePipeTop; break;
	case 0001010100: return TileSingle; break;
	case 0001010101: return TileSingle; break;
	case 0001010110: return TilePipeTop; break;
	case 0001010111: return TilePipeTop; break;
	case 0001011000: return TilePipeLeft; break;
	case 0001011001: return TilePipeLeft; break;
	case 0001011010: return TilePipeTopLeft; break;
	case 0001011011: return TileTopLeft; break;
	case 0001011100: return TilePipeLeft; break;
	case 0001011101: return TilePipeLeft; break;
	case 0001011110: return TilePipeTopLeft; break;
	case 0001011111: return TileTopLeft; break;
	case 0001110000: return TilePipeRight; break;
	case 0001110001: return TilePipeRight; break;
	case 0001110010: return TilePipeTopRight; break;
	case 0001110011: return TilePipeTopRight; break;
	case 0001110100: return TilePipeRight; break;
	case 0001110101: return TilePipeRight; break;
	case 0001110110: return TileTopRight; break;
	case 0001110111: return TileTopRight; break;
	case 0001111000: return TilePipeHorizontal; break;
	case 0001111001: return TilePipeHorizontal; break;
	case 0001111010: return TilePipeTTop; break;
	case 0001111011: return TilePipeTopRight; break;
	case 0001111100: return TilePipeHorizontal; break;
	case 0001111101: return TilePipeHorizontal; break;
	case 0001111110: return TilePipeTopLeft; break;
	case 0001111111: return TileTopCenter; break;
	case 0010010000: return TilePipeBot; break;
	case 0010010001: return TilePipeBot; break;
	case 0010010010: return TilePipeVertical; break;
	case 0010010011: return TilePipeVertical; break;
	case 0010010100: return TilePipeBot; break;
	case 0010010101: return TilePipeBot; break;
	case 0010010110: return TilePipeVertical; break;
	case 0010010111: return TilePipeVertical; break;
	case 0010011000: return TilePipeBotLeft; break;
	case 0010011001: return TilePipeBotLeft; break;
	case 0010011010: return TilePipeTLeft; break;
	case 0010011011: return TileLeftVerticalPipeBot; break;
	case 0010011100: return TilePipeTLeft; break;
	case 0010011101: return TilePipeBotLeft; break;
	case 0010011110: return TilePipeTLeft; break;
	case 0010011111: return TileLeftVerticalPipeBot; break;
	case 0010110000: return TilePipeBotRight; break;
	case 0010110001: return TilePipeBotRight; break;
	case 0010110010: return TilePipeTRight; break;
	case 0010110011: return TilePipeTRight; break;
	case 0010110100: return TilePipeBotRight; break;
	case 0010110101: return TilePipeBotRight; break;
	case 0010110110: return TileRightVerticalPipeBot; break;
	case 0010110111: return TileRightVerticalPipeBot; break;
	case 0010111000: return TilePipeTBot; break;
	case 0010111001: return TilePipeTBot; break;
	case 0010111010: return TileInnerFour; break;
	case 0010111011: return TileInnerThreeTL; break;
	case 0010111100: return TilePipeTBot; break;
	case 0010111101: return TilePipeTBot; break;
	case 0010111110: return TileInnerThreeTR; break;
	case 0010111111: return TileInnerTwoTop; break;
	case 0011010000: return TilePipeBot; break;
	case 0011010001: return TilePipeBot; break;
	case 0011010010: return TilePipeVertical; break;
	case 0011010011: return TilePipeVertical; break;
	case 0011010100: return TilePipeBot; break;
	case 0011010101: return TilePipeBot; break;
	case 0011010110: return TilePipeVertical; break;
	case 0011010111: return TilePipeVertical; break;
	case 0011011000: return TileBotLeft; break;
	case 0011011001: return TileBotLeft; break;
	case 0011011010: return TileLeftVerticalPipeBot; break;
	case 0011011011: return TileLeft; break;
	case 0011011100: return TileBotLeft; break;
	case 0011011101: return TileBotLeft; break;
	case 0011011110: return TileLeftVerticalPipeTop; break;
	case 0011011111: return TileLeft; break;
	case 0011110000: return TilePipeBotRight; break;
	case 0011110001: return TilePipeBotRight; break;
	case 0011110010: return TilePipeTRight; break;
	case 0011110011: return TilePipeTRight; break;
	case 0011110100: return TilePipeBotRight; break;
	case 0011110101: return TilePipeBotRight; break;
	case 0011110110: return TileRightVerticalPipeBot; break;
	case 0011110111: return TileRightVerticalPipeBot; break;
	case 0011111000: return TileBotHorizontalPipeRight; break;
	case 0011111001: return TileBotHorizontalPipeRight; break;
	case 0011111010: return TileInnerThreeBL; break;
	case 0011111011: return TileInnerTwoLeft; break;
	case 0011111100: return TileBotHorizontalPipeRight; break;
	case 0011111101: return TileBotHorizontalPipeRight; break;
	case 0011111110: return TileInnerDiagDown; break;
	case 0011111111: return TileInnerCornerBotLeft; break;
	case 0100010000: return TileSingle; break;
	case 0100010001: return TileSingle; break;
	case 0100010010: return TilePipeTop; break;
	case 0100010011: return TilePipeTop; break;
	case 0100010100: return TileSingle; break;
	case 0100010101: return TileSingle; break;
	case 0100010110: return TilePipeTop; break;
	case 0100010111: return TilePipeTop; break;
	case 0100011000: return TilePipeLeft; break;
	case 0100011001: return TilePipeLeft; break;
	case 0100011010: return TilePipeTopLeft; break;
	case 0100011011: return TileTopLeft; break;
	case 0100011100: return TilePipeLeft; break;
	case 0100011101: return TilePipeLeft; break;
	case 0100011110: return TilePipeTopLeft; break;
	case 0100011111: return TileTopLeft; break;
	case 0100110000: return TilePipeRight; break;
	case 0100110001: return TilePipeRight; break;
	case 0100110010: return TilePipeTopRight; break;
	case 0100110011: return TilePipeTopRight; break;
	case 0100110100: return TilePipeRight; break;
	case 0100110101: return TilePipeRight; break;
	case 0100110111: return TileTopRight; break;
	case 0100111000: return TilePipeHorizontal; break;
	case 0100111001: return TilePipeHorizontal; break;
	case 0100111010: return TilePipeTTop; break;
	case 0100111011: return TileTopHorizontalPipeRight; break;
	case 0100111100: return TilePipeHorizontal; break;
	case 0100111101: return TilePipeHorizontal; break;
	case 0100111110: return TileTopHorizontalPipeLeft; break;
	case 0100111111: return TileTopCenter; break;
	case 0101010000: return TileSingle; break;
	case 0101010001: return TileSingle; break;
	case 0101010010: return TilePipeTop; break;
	case 0101010011: return TilePipeTop; break;
	case 0101010100: return TileSingle; break;
	case 0101010101: return TileSingle; break;
	case 0101010110: return TilePipeTop; break;
	case 0101010111: return TilePipeTop; break;
	case 0101011000: return TilePipeLeft; break;
	case 0101011001: return TilePipeLeft; break;
	case 0101011010: return TilePipeTopLeft; break;
	case 0101011011: return TileTopLeft; break;
	case 0101011100: return TilePipeLeft; break;
	case 0101011101: return TilePipeLeft; break;
	case 0101011110: return TilePipeTopLeft; break;
	case 0101011111: return TileTopLeft; break;
	case 0101110000: return TilePipeRight; break;
	case 0101110001: return TilePipeRight; break;
	case 0101110010: return TilePipeTopRight; break;
	case 0101110011: return TilePipeTopRight; break;
	case 0101110100: return TilePipeRight; break;
	case 0101110101: return TilePipeRight; break;
	case 0101110110: return TileTopRight; break;
	case 0101110111: return TileTopRight; break;
	case 0101111000: return TilePipeHorizontal; break;
	case 0101111001: return TilePipeHorizontal; break;
	case 0101111010: return TilePipeTTop; break;
	case 0101111011: return TileTopHorizontalPipeRight; break;
	case 0101111100: return TilePipeHorizontal; break;
	case 0101111101: return TilePipeHorizontal; break;
	case 0101111110: return TileTopHorizontalPipeLeft; break;
	case 0101111111: return TileTopCenter; break;
	case 0110010000: return TilePipeBot; break;
	case 0110010001: return TilePipeBot; break;
	case 0110010010: return TilePipeVertical; break;
	case 0110010011: return TilePipeVertical; break;
	case 0110010100: return TilePipeBot; break;
	case 0110010101: return TilePipeBot; break;
	case 0110010110: return TilePipeVertical; break;
	case 0110010111: return TilePipeVertical; break;
	case 0110011000: return TilePipeBotLeft; break;
	case 0110011001: return TilePipeBotLeft; break;
	case 0110011010: return TilePipeTLeft; break;
	case 0110011011: return TileLeftVerticalPipeBot; break;
	case 0110011100: return TilePipeBotLeft; break;
	case 0110011101: return TilePipeBotLeft; break;
	case 0110011110: return TilePipeTLeft; break;
	case 0110011111: return TileLeftVerticalPipeBot; break;
	case 0110110000: return TileBotRight; break;
	case 0110110001: return TileBotRight; break;
	case 0110110010: return TileRightVerticalPipeTop; break;
	case 0110110011: return TileRightVerticalPipeTop; break;
	case 0110110100: return TileBotRight; break;
	case 0110110101: return TileBotRight; break;
	case 0110110110: return TileRight; break;
	case 0110110111: return TileRight; break;
	case 0110111000: return TileBotHorizontalPipeLeft; break;
	case 0110111001: return TileBotHorizontalPipeLeft; break;
	case 0110111010: return TileInnerThreeTR; break;
	case 0110111011: return TileInnerDiagUp; break;
	case 0110111100: return TileBotHorizontalPipeLeft; break;
	case 0110111101: return TileBotHorizontalPipeLeft; break;
	case 0110111110: return TileInnerTwoRight;
	case 0110111111: return TileInnerCornerBotLeft; break;
	case 0111010000: return TilePipeBot; break;
	case 0111010001: return TilePipeBot; break;
	case 0111010010: return TilePipeVertical; break;
	case 0111010011: return TilePipeVertical; break;
	case 0111010100: return TilePipeBot; break;
	case 0111010101: return TilePipeBot; break;
	case 0111010110: return TilePipeVertical; break;
	case 0111010111: return TilePipeVertical; break;
	case 0111011000: return TileBotLeft; break;
	case 0111011001: return TileBotLeft; break;
	case 0111011010: return TileLeftVerticalPipeTop; break;
	case 0111011011: return TileLeft; break;
	case 0111011100: return TileBotLeft; break;
	case 0111011101: return TileBotLeft; break;
	case 0111011110: return TileLeftVerticalPipeTop; break;
	case 0111011111: return TileLeft; break;
	case 0111110000: return TileBotRight; break;
	case 0111110001: return TileBotRight; break;
	case 0111110010: return TileRightVerticalPipeTop; break;
	case 0111110011: return TileRightVerticalPipeTop; break;
	case 0111110100: return TileBotRight; break;
	case 0111110101: return TileBotRight; break;
	case 0111110110: return TileRight; break;
	case 0111110111: return TileRight; break;
	case 0111111000: return TileBot; break;
	case 0111111001: return TileBot; break;
	case 0111111010: return TileInnerTwoBot; break;
	case 0111111011: return TileInnerCornerTopRight; break;
	case 0111111100: return TileBot; break;
	case 0111111101: return TileBot; break;
	case 0111111110: return TileInnerCornerTopLeft; break;
	case 0111111111: return TileCenter; break;

	default: return TileCenter; break;
		/*
	case 000010000: (*s)[column][row].tile = BlockSingleTop; break;

	case 000110000: (*s)[column][row].tile = BlockTopRight; break;
	case 000011000: (*s)[column][row].tile = BlockTopLeft; break;
	case 010010000: (*s)[column][row].tile = BlockSingleBot; break;
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
	*/
	}
}

void Silhouette::render(int cameraOffset) {
	//std::string temp[4][5] = this->shapeTemplate;

	std::string(*s)[5] = this->shapeTemplate;

	if (this->offset < 0) {
		// Update tile offset
		int pixelsDown = 16 * (getFrameDelta(this->lastDropped) / (FRAMES_PER_SECOND * 0.25));
		if (pixelsDown > 0) {
			this->offset += pixelsDown;
			this->lastDropped = SDL_GetTicks();
			if (this->offset > 0)
				this->offset = 0;
		}

	}

	for (int i = 0; i < (*s)->length(); i++) { 
		for (int j = 0; j < (*s)[i].length(); j++) { 
			if ((*s)[i][j] != ' ') { // there is an O in the string representing part of the tile
				int index = determineTextureIndex(i, j);
				if (index < 0) continue;

				if (getValidity()) {
					this->resourceManager.textures[TextureSilhouetteSheet].render((this->tileX + j)*BLOCK_SIZE + cameraOffset,
						(this->tileY + i)*BLOCK_SIZE + this->offset, &(resourceManager.silhouetteClips[index]));
				}
				else {
					this->resourceManager.textures[TextureSilhouetteInvalidSheet].render((this->tileX + j)*BLOCK_SIZE + cameraOffset,
						(this->tileY + i)*BLOCK_SIZE + this->offset, &(resourceManager.silhouetteClips[index]));

				}
				
				//resourceManager.textures[TextureBlock].render(i + cameraOffset, j + tiles[i / 16][j / 16].offset,
					//&(resourceManager.blockClips[tiles[i / 16][j / 16].tile]));
			}
		}
	}

	/*std::string(*s)[5] = this->shapeTemplate[this->rotationIndex];
	std::string(*ss)[5] = (this->shapeTemplate[this->rotationIndex + 1]);
	std::string(*t)[5] = &(J_SHAPE_TEMPLATE[0]);
	std::string(*tt)[5] = &(J_SHAPE_TEMPLATE[1]);
	
	if ((*s) == NULL) return;
	for (int i = 0; i < (*s)->length(); i++) { // i is the row as a string
		//if ((*s)[i] == NULL) return;
		for (int j = 0; j < (*s)[i].length(); j++) { // j is the character in the string
			if ((*s)[i][j] != ' ') { // there is an O in the string
				this->resourceManager.textures[TextureBlockShadow].render((this->tileX + j)*BLOCK_SIZE, (this->tileY + i)*BLOCK_SIZE);

			}
		}
	}*/
}