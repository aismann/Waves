#include "Button.h"

Button::Button(SDL_Renderer& r, ResourceManager& rm, ButtonType t) :
	resourceManager(rm), renderer(r) {
	this->state = NORMAL;
	this->buttonType = t;
	switch (t) {
	case ButtonStart: 
		this->height = rm.textures[TextureButtonStart].getHeight();
		this->width = rm.textures[TextureButtonStart].getWidth();
	case ButtonRetry:
		this->height = rm.textures[TextureButtonRetry].getHeight();
		this->width = rm.textures[TextureButtonRetry].getWidth();
	case ButtonBack:
		this->height = rm.textures[TextureButtonBack].getHeight();
		this->width = rm.textures[TextureButtonBack].getWidth();
	case ButtonShop:
		this->height = rm.textures[TextureButtonShop].getHeight();
		this->width = rm.textures[TextureButtonShop].getWidth();
	}
}

Button::~Button() {

}

int Button::getX() {
	return x;
}
int Button::getY() {
	return y;
}
void Button::setPosition(int x, int y) {
	this->x = x; this->y = y;
}
bool Button::isClicked() {
	return this->state == CLICKED;
}
ButtonType Button::getButtonType() {
	return this->buttonType;
}

bool Button::checkClicked(int mouseX, int mouseY) {
	if (mouseX < this->x || mouseX > this->x + this->width) 
		this->state = NORMAL;
	else if (mouseY < this->y || mouseY > this->y + this->height) 
		this->state = NORMAL;
	else this->state = CLICKED;
	return this->state;
}

void Button::render() {
	switch (buttonType) {
	case ButtonStart:
		resourceManager.textures[TextureButtonStart].render(x, y); break;
	case ButtonRetry:
		resourceManager.textures[TextureButtonRetry].render(x, y); break;
	case ButtonBack:
		resourceManager.textures[TextureButtonBack].render(x, y); break;
	case ButtonShop:
		resourceManager.textures[TextureButtonShop].render(x, y); break;
	}
}