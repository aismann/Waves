#pragma once
#include <SDL.h>
#include <chrono>
#include "ResourceManager.h"
#include "Grid.h"

enum MovementState {
	STOPPED = 0,
	WALKING = 1,
	SCALING = 2,
	CLIMBING = 3,
	JUMPING = 4,
	FALLING = 5
};



class Cat {
public:
	std::map<MovementState, Uint32> MovementSpeeds;

public:
	Cat(SDL_Renderer& renderer, ResourceManager& rm, Grid& grid);
	~Cat();

	int getTileX();
	int getTileY();
	int getNewTileX();
	int getNewTileY();
	int getMovementState();
	int getAutoMovement();
	int getDirection();
	Uint32 getTileTransitionStartTime();
	Uint32 getTileTransitionFinishTime();
	bool isTileTransitionComplete();
	bool isWet();

	void fall();
	void shiftLeft();
	void setTileX(int x);
	void setTileY(int y);
	void toggleAutoMovement();
	void toggleDirection();
	void setMovementState(int state);

	bool isPath(bool toRight);
	int getPathType(bool toRight);

	void moveToPosition(int column);
	void updateToNewPosition();

	void render(int cameraOffset);

private:
	int tileX;
	int tileY;
	int newTileX;
	int newTileY;
	int movementState;
	bool autoMovement = 1;
	bool direction = 1;

	Uint32 tileTransitionStartTime;
	Uint32 tileTransitionFinishTime;

	ResourceManager& resourceManager;
	SDL_Renderer& renderer;
	Grid& grid;
};