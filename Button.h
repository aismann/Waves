#pragma once
#include "ResourceManager.h"
#include <SDL.h>

enum State {
	NORMAL = 0,
	CLICKED
};

class Button {
public:
	Button(SDL_Renderer& r, ResourceManager& rm, ButtonType t);
	~Button();

	int getX();
	int getY();
	ButtonType getButtonType();
	void setPosition(int x, int y);
	bool isClicked();

	void render();
	bool checkClicked(int mouseX, int mouseY);

private:
	int x, y;
	int height, width;
	int state;
	ButtonType buttonType;

	SDL_Renderer& renderer;
	ResourceManager& resourceManager;
};