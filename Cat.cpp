#include "Cat.h"
#include <iostream>

Cat::Cat(SDL_Renderer& r, ResourceManager& rm, Grid& grid) :
	resourceManager(rm), renderer(r), grid(grid) {
	this->tileX = 3;
	this->tileY = 8;
	this->newTileX = -1;
	this->newTileY = -1;
	grid.setTile(this->tileX, this->tileY, -2);
	grid.setTile(this->tileX, this->tileY - 1, -2);

	Cat::MovementSpeeds = {
		{ FALLING, 250 }, // may seem off because not performing the animation currently
		{ WALKING, 1000 },
		{ SCALING, 2000 },
		{ CLIMBING, 4000 },
		{ JUMPING, 6000 }
	};

}

Cat::~Cat() {

}

int Cat::getTileX() {
	return this->tileX;
}
int Cat::getTileY() {
	return this->tileY;
}
int Cat::getNewTileX() {
	return this->newTileX;
}
int Cat::getNewTileY() {
	return this->newTileY;
}
int Cat::getMovementState() {
	return this->movementState;
}
int Cat::getAutoMovement() {
	return this->autoMovement;
}
int Cat::getDirection() {
	return this->direction;
}
Uint32 Cat::getTileTransitionStartTime() {
	return this->tileTransitionStartTime;
}
Uint32 Cat::getTileTransitionFinishTime() {
	return this->tileTransitionFinishTime;
}

bool Cat::isTileTransitionComplete() {
	return SDL_GetTicks() > this->tileTransitionFinishTime;
}

bool Cat::isWet() {
	if (grid.isTileWet(getTileX(), getTileY()))
		return true;
	return false;
}

void Cat::fall() {
	if (!grid.isTileAt(this->tileX, this->tileY + 1)) {
		this->setMovementState(FALLING);

		this->tileTransitionStartTime = SDL_GetTicks();
		this->tileTransitionFinishTime =
			tileTransitionStartTime + MovementSpeeds[FALLING];
		this->newTileX = this->tileX;
		this->newTileY = this->tileY + 1;
		grid.setTile(this->newTileX, this->newTileY, -2);
		grid.setTile(this->newTileX, this->newTileY - 1, -2);
	}
	
	

	//while (!(grid.isTileAt(this->tileX, this->tileY + 1))) {
	//	this->tileY++;
	//	this->newTileY++;
	//}
}
void Cat::shiftLeft() {
	this->tileX--;
	this->newTileX--;
}

void Cat::setTileX(int x) {
	this->tileX = x;
}
void Cat::setTileY(int y) {
	this->tileY = y;
}
void Cat::toggleAutoMovement() {
	this->autoMovement = ((this->autoMovement + 1) % 2);
}
void Cat::toggleDirection() {
	this->setMovementState(STOPPED); 
	this->direction = ((this->direction + 1) % 2);
}
void Cat::setMovementState(int state) {
	this->movementState = state;

	if (state == STOPPED) {
		switch (getPathType(this->direction)) {
		case JUMPING:
			grid.setTile(this->tileX, this->tileY - 4, 0);
		case CLIMBING:
			grid.setTile(this->tileX, this->tileY - 3, 0);
		case SCALING:
			grid.setTile(this->tileX, this->tileY - 2, 0);
		case WALKING:
			grid.setTile(this->newTileX, this->newTileY, 0);
			grid.setTile(this->newTileX, this->newTileY - 1, 0);
			break;
		}
	}

}

bool Cat::isPath(bool toRight) {
	if (getPathType(toRight) != -1)
		return true;
	return false;
}
int Cat::getPathType(bool toRight) {
	if (toRight) {
		// Walk to the right
		if (!grid.isTileAt(this->tileX + 1, this->tileY) &&
			grid.isTileAt(this->tileX + 1, this->tileY + 1) &&
			!grid.isTileAt(this->tileX + 1, this->tileY - 1))
			return WALKING; 
		// Stair step upward
		else if (!grid.isTileAt(this->tileX, this->tileY - 2) &&
			!grid.isTileAt(this->tileX + 1, this->tileY - 1) &&
			grid.isTileAt(this->tileX + 1, this->tileY) &&
			!grid.isTileAt(this->tileX + 1, this->tileY - 2))
			return SCALING;
		// Jump upward
		else if (!grid.isTileAt(this->tileX, this->tileY - 2) && 
			!grid.isTileAt(this->tileX, this->tileY - 3) &&
			!grid.isTileAt(this->tileX + 1, this->tileY - 2) &&
			grid.isTileAt(this->tileX + 1, this->tileY - 1) &&
			!grid.isTileAt(this->tileX + 1, this->tileY - 3))
			return CLIMBING;	
		// Long leap upward
		else if (!grid.isTileAt(this->tileX, this->tileY - 2) && 
			!grid.isTileAt(this->tileX, this->tileY - 3) &&
			!grid.isTileAt(this->tileX, this->tileY - 4) &&
			!grid.isTileAt(this->tileX + 1, this->tileY - 3) &&
			grid.isTileAt(this->tileX + 1, this->tileY - 2) &&
			!grid.isTileAt(this->tileX + 1, this->tileY - 4))
			return JUMPING;
	}
	else { // toLeft
		if (this->tileX <= 1)
			return -1;
		   // Walk to the left
		if (!grid.isTileAt(this->tileX - 1, this->tileY) &&
			grid.isTileAt(this->tileX - 1, this->tileY + 1) &&
			!grid.isTileAt(this->tileX - 1, this->tileY - 1))
			return WALKING;
		// Stair step upward
		else if (!grid.isTileAt(this->tileX, this->tileY - 2) && 
			!grid.isTileAt(this->tileX - 1, this->tileY - 1) &&
			grid.isTileAt(this->tileX - 1, this->tileY) &&
			!grid.isTileAt(this->tileX - 1, this->tileY - 2))
			return SCALING;
		// Jump upward
		else if (!grid.isTileAt(this->tileX, this->tileY - 2) &&
			!grid.isTileAt(this->tileX, this->tileY - 3) &&
			!grid.isTileAt(this->tileX - 1, this->tileY - 2) &&
			grid.isTileAt(this->tileX - 1, this->tileY - 1) &&
			!grid.isTileAt(this->tileX - 1, this->tileY - 3))
			return CLIMBING;
		// Long leap upward
		else if (!grid.isTileAt(this->tileX, this->tileY - 2) && 
			!grid.isTileAt(this->tileX, this->tileY - 3) &&
			!grid.isTileAt(this->tileX, this->tileY - 4) &&
			!grid.isTileAt(this->tileX - 1, this->tileY - 3) &&
			grid.isTileAt(this->tileX - 1, this->tileY - 2) &&
			!grid.isTileAt(this->tileX - 1, this->tileY - 4))
			return JUMPING;
	}
	
	return -1;
}

void Cat::moveToPosition(int column) {
	if (column > this->tileX) {
		switch (getPathType(true)) { // Going right
		case WALKING:
			this->setMovementState(WALKING);
			this->tileTransitionStartTime = SDL_GetTicks();
			this->tileTransitionFinishTime =
				tileTransitionStartTime + MovementSpeeds[WALKING];
			this->newTileX = this->tileX + 1;
			this->newTileY = this->tileY;
			grid.setTile(this->newTileX, this->newTileY, -2);
			grid.setTile(this->newTileX, this->newTileY - 1, -2);
			break;
		case SCALING:
			this->setMovementState(SCALING);
			this->tileTransitionStartTime = SDL_GetTicks();
			this->tileTransitionFinishTime =
				tileTransitionStartTime + MovementSpeeds[SCALING];
			this->newTileX = this->tileX + 1;
			this->newTileY = this->tileY - 1;
			grid.setTile(this->newTileX, this->newTileY, -2);
			grid.setTile(this->newTileX, this->newTileY - 1, -2);
			grid.setTile(this->tileX, this->tileY - 2, -2);
			break;
		case CLIMBING:
			this->setMovementState(CLIMBING);
			this->tileTransitionStartTime = SDL_GetTicks();
			this->tileTransitionFinishTime =
				tileTransitionStartTime + MovementSpeeds[CLIMBING];
			this->newTileX = this->tileX + 1;
			this->newTileY = this->tileY - 2;
			grid.setTile(this->newTileX, this->newTileY, -2);
			grid.setTile(this->newTileX, this->newTileY - 1, -2);
			grid.setTile(this->tileX, this->tileY - 2, -2);
			grid.setTile(this->tileX, this->tileY - 3, -2);
			break;
		case JUMPING:
			this->setMovementState(JUMPING);
			this->tileTransitionStartTime = SDL_GetTicks();
			this->tileTransitionFinishTime =
				tileTransitionStartTime + MovementSpeeds[JUMPING];
			this->newTileX = this->tileX + 1;
			this->newTileY = this->tileY - 3;
			grid.setTile(this->newTileX, this->newTileY, -2);
			grid.setTile(this->newTileX, this->newTileY - 1, -2);
			grid.setTile(this->tileX, this->tileY - 2, -2);
			grid.setTile(this->tileX, this->tileY - 3, -2);
			grid.setTile(this->tileX, this->tileY - 4, -2);
			break;
		}
	}
	else if (column < this->tileX) { // Going left
		switch (getPathType(false)) {
		case WALKING:
			this->setMovementState(WALKING);
			this->tileTransitionStartTime = SDL_GetTicks();
			this->tileTransitionFinishTime =
				tileTransitionStartTime + MovementSpeeds[WALKING];
			this->newTileX = this->tileX - 1;
			this->newTileY = this->tileY;
			grid.setTile(this->newTileX, this->newTileY, -2);
			grid.setTile(this->newTileX, this->newTileY - 1, -2);
			break;
		case SCALING:
			this->setMovementState(SCALING);
			this->tileTransitionStartTime = SDL_GetTicks();
			this->tileTransitionFinishTime =
				tileTransitionStartTime + MovementSpeeds[SCALING];
			this->newTileX = this->tileX - 1;
			this->newTileY = this->tileY - 1;
			grid.setTile(this->newTileX, this->newTileY, -2);
			grid.setTile(this->newTileX, this->newTileY - 1, -2);
			grid.setTile(this->tileX, this->tileY - 2, -2);
			break;
		case CLIMBING:
			this->setMovementState(CLIMBING);
			this->tileTransitionStartTime = SDL_GetTicks();
			this->tileTransitionFinishTime =
				tileTransitionStartTime + MovementSpeeds[CLIMBING];
			this->newTileX = this->tileX - 1;
			this->newTileY = this->tileY - 2;
			grid.setTile(this->newTileX, this->newTileY, -2);
			grid.setTile(this->newTileX, this->newTileY - 1, -2);
			grid.setTile(this->tileX, this->tileY - 2, -2);
			grid.setTile(this->tileX, this->tileY - 3, -2);
			break;
		case JUMPING:
			this->setMovementState(JUMPING);
			this->tileTransitionStartTime = SDL_GetTicks();
			this->tileTransitionFinishTime =
				tileTransitionStartTime + MovementSpeeds[JUMPING];
			this->newTileX = this->tileX - 1;
			this->newTileY = this->tileY - 3;
			grid.setTile(this->newTileX, this->newTileY, -2);
			grid.setTile(this->newTileX, this->newTileY - 1, -2);
			grid.setTile(this->tileX, this->tileY - 2, -2);
			grid.setTile(this->tileX, this->tileY - 3, -2);
			grid.setTile(this->tileX, this->tileY - 4, -2);
			break;
		}
	}
	
}
void Cat::updateToNewPosition() {
	setMovementState(STOPPED);

	grid.setTile(this->tileX, this->tileY, 0);
	grid.setTile(this->tileX, this->tileY - 1, 0);
	grid.setTile(this->newTileX, this->newTileY, -2);
	grid.setTile(this->newTileX, this->newTileY - 1, -2);
	
	setTileX(this->newTileX);
	setTileY(this->newTileY);
}

void Cat::render(int cameraOffset) {
	this->resourceManager.textures[TextureCat].render((this->tileX)*BLOCK_SIZE + cameraOffset, 
		(this->tileY)*BLOCK_SIZE);
}