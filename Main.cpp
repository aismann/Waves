#include "Main.h"

int getFrameDelta(Uint32 timerStartTime) {
	if (timerStartTime > SDL_GetTicks()) return 0;
	Uint32 tickDelta = SDL_GetTicks() - timerStartTime;
	return tickDelta / MILLISECONDS_PER_FRAME;
}

bool init() {
	
	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		printf("SDL could not initialize!\nSDL Error:%s\n", SDL_GetError());
		return false;
	}

	// Initialize PNG Loading
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) &imgFlags)) {
		printf("SDL_image could not initialize!\nSDL_Image Error:%s\n",
			IMG_GetError());
		return false;
	}

	// Initialize SDL_ttf
	if (TTF_Init() == -1) {
		printf("SDL_ttf could not initialize!\nSDL_ttf Error: %s\n",
			TTF_GetError());
		return false;
	}
	
	// Initialize SDL_mixer
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		printf("SDL_mixer could not initialize!\nSDL_Mixer Error: %s\n",
			Mix_GetError());
		return false;
	}

	return true;
}

SDL_Window* setupWindow() {
	// Create the window
	SDL_Window* w = SDL_CreateWindow("Waves", SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (w == NULL) {
		printf("Window could not be created!\nSDL Error:%s\n", SDL_GetError());
	}
	return w;
}

SDL_Renderer* setupRenderer(SDL_Window& window) {
	// Create vsynced renderer for window
	SDL_Renderer* renderer = SDL_CreateRenderer(&window, -1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL) {
		printf("Renderer could not be created!\nSDL Error: %s\n", SDL_GetError());
	}
	// Initialize renderer color
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	return renderer;
}

bool loadMedia(ResourceManager& rm, SDL_Renderer& renderer) {
	// Loading success flag
	bool success = true;

	// Load silhouette textures
	Texture* silhouetteSheet = new Texture(renderer);
	if (!silhouetteSheet->loadFromFile("Assets/SilhouetteTileset.png")) {
		printf("Failed to load Silhouette Tilesheet!\n");
		success = false;
	}
	rm.setTexture(*silhouetteSheet, TextureSilhouetteSheet);
	rm.initializeSilhouetteTileClips();
	// Load silhouette_invalid textures
	Texture* silhouetteInvalidSheet = new Texture(renderer);
	if (!silhouetteInvalidSheet->loadFromFile("Assets/SilhouetteTileset2.png")) {
		printf("Failed to load Silhouette Tilesheet 2!\n");
		success = false;
	}
	rm.setTexture(*silhouetteInvalidSheet, TextureSilhouetteInvalidSheet);
	rm.initializeSilhouetteInvalidTileClips();

	Texture* blockShadowTexture = new Texture(renderer);
	if (!blockShadowTexture->loadFromFile("Assets/Block.png")) {
		printf("Failed to load Block Shadow Texture!\n");
		success = false;
	}
	rm.setTexture(*blockShadowTexture, TextureBlockShadow);

	// Load block spritesheet texture
	Texture* blockTexture = new Texture(renderer);
	if (!blockTexture->loadFromFile("Assets/BlockTiles.png")) {
		printf("Failed to load Block Tiles Texture!\n");
		success = false;
	}
	else {
		rm.setTexture(*blockTexture, TextureBlock);
		rm.initializeBlockTileClips();
	}
	// Load block falling texture
	Texture* blockFallingTexture = new Texture(renderer);
	if (!blockFallingTexture->loadFromFile("Assets/BlockFalling.png")) {
		printf("Failed to load Block Falling Tile Texture!\n");
		success = false;
	}
	else {
		rm.setTexture(*blockFallingTexture, TextureBlockFalling);
	}

	// Load water texture
	Texture* waterTexture = new Texture(renderer);
	if (!waterTexture->loadFromFile("Assets/Water.png")) {
		printf("Failed to load Water Texture!\n");
		success = false;
	}
	else rm.setTexture(*waterTexture, TextureWater);
	Texture* waterTransparentTexture = new Texture(renderer);
	if (!waterTransparentTexture->loadFromFile("Assets/Water.png")) {
		printf("Failed to load Transparent Water Texture!\n");
		success = false;
	}
	else {
		rm.setTexture(*waterTransparentTexture, TextureWaterTransparent);
		rm.textures[TextureWaterTransparent].setBlendMode(SDL_BLENDMODE_BLEND);
		rm.textures[TextureWaterTransparent].setAlpha(96);
	}

	// Load sky texture
	Texture* skyTexture = new Texture(renderer);
	if (!skyTexture->loadFromFile("Assets/Sky.png")) {
		printf("Failed to load Sky Texture!\n");
		success = false;
	}
	else rm.setTexture(*skyTexture, TextureSky);

	// Load placeholder cat texture
	Texture* catTexture = new Texture(renderer);
	if (!catTexture->loadFromFile("Assets/Cat.png")) {
		printf("Failed to load Cat Texture!\n");
		success = false;
	}
	else rm.setTexture(*catTexture, TextureCat);

	// Load wave meter sheet
	Texture* waveMeterTexture = new Texture(renderer);
	if (!waveMeterTexture->loadFromFile("Assets/waveMeter.png")) {
		printf("Failed to load Wave Meter Texture!\n");
		success = false;
	}
	else {
		rm.setTexture(*waveMeterTexture, TextureWaveMeterSheet);
		rm.initializeWaveMeterClips();
	}
	// Load water surface sheet
	Texture* waterSurfaceTexture = new Texture(renderer);
	if (!waterSurfaceTexture->loadFromFile("Assets/SurfaceWaterAnimation.png")) {
		printf("Failed to load Water Surface Texture!\n");
		success = false;
	}
	else {
		rm.setTexture(*waterSurfaceTexture, TextureWaterSurfaceSheet);
		rm.initializeWaterSurfaceClips();
	}
	Texture* waterSurfaceTransparentTexture = new Texture(renderer);
	if (!waterSurfaceTransparentTexture->loadFromFile("Assets/SurfaceWaterAnimation.png")) {
		printf("Failed to load Water Surface Transparent Texture!\n");
		success = false;
	}
	else {
		rm.setTexture(*waterSurfaceTransparentTexture, TextureWaterSurfaceTransparentSheet); 
		rm.textures[TextureWaterSurfaceTransparentSheet].setBlendMode(SDL_BLENDMODE_BLEND);
		rm.textures[TextureWaterSurfaceTransparentSheet].setAlpha(96);
	}

	// Load buttons
	Texture* buttonStartTexture = new Texture(renderer);
	if (!buttonStartTexture->loadFromFile("Assets/buttonStart.png")) {
		printf("Failed to load start button texture!\n");
		success = false;
	}
	else rm.setTexture(*buttonStartTexture, TextureButtonStart);
	
	Texture* buttonBackTexture = new Texture(renderer);
	if (!buttonBackTexture->loadFromFile("Assets/buttonBack.png")) {
		printf("Failed to load back button texture!\n");
		success = false;
	}
	else rm.setTexture(*buttonBackTexture, TextureButtonBack);
	
	Texture* buttonRetryTexture = new Texture(renderer);
	if (!buttonRetryTexture->loadFromFile("Assets/buttonRetry.png")) {
		printf("Failed to load retry button texture!\n");
		success = false;
	}
	else rm.setTexture(*buttonRetryTexture, TextureButtonRetry);

	Texture* buttonShopTexture = new Texture(renderer);
	if (!buttonShopTexture->loadFromFile("Assets/buttonShop.png")) {
		printf("Failed to load shop button texture!\n");
		success = false;
	}
	else rm.setTexture(*buttonShopTexture, TextureButtonShop);

	rm.initializeFonts();

	return success;
}

void close(SDL_Renderer* renderer, SDL_Window* window, ResourceManager& rm) {
	// Free data structures

	// Free loaded images
	rm.clearAllTextures();
	// Free fonts
	rm.clearFonts();
	// Free sounds

	// Destroy window
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	// Quit SDL Systems
	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

int startScreen(SDL_Renderer* renderer, SDL_Window* window, ResourceManager& rm) {
	// Create objects
	EyeCandyManager ecm = EyeCandyManager(*renderer, rm);
	
	// Create buttons
	std::vector<Button> buttons;
	Button startButton(*renderer, rm, ButtonStart);
	startButton.setPosition(
		VIEW_WIDTH / 2 - rm.textures[TextureButtonStart].getWidth() / 2,
		VIEW_HEIGHT / 2 - rm.textures[TextureButtonStart].getHeight() / 2);
	buttons.push_back(startButton);
	Button shopButton(*renderer, rm, ButtonShop);
	shopButton.setPosition(
		VIEW_WIDTH - rm.textures[TextureButtonShop].getWidth() - 2,
		VIEW_HEIGHT - rm.textures[TextureButtonShop].getHeight() - 2
	);
	buttons.push_back(shopButton);
	
	
	// Main Loop Flag
	bool quit = false;
	// Event handler
	SDL_Event e;

	// While app is running
	while (!quit) {
		// Handle events
		while (SDL_PollEvent(&e) != 0) {
			// User quits
			if (e.type == SDL_QUIT) {
				quit = true;
			}
			// User presses a key
			else if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.sym) {
				case SDLK_SPACE: break;
				}
			}
			else if (e.type == SDL_MOUSEBUTTONDOWN) {
				int x, y;
				SDL_GetMouseState(&x, &y);
				for (Button& b : buttons) {
					b.checkClicked(x*SCALE_FACTOR_HORIZONTAL, 
						y*SCALE_FACTOR_VERTICAL);
				}
			}
			else if (e.type == SDL_MOUSEBUTTONUP) {
				int x, y;
				SDL_GetMouseState(&x, &y);
				for (Button& b : buttons) {
					if (b.isClicked() && (b.checkClicked(
						x*SCALE_FACTOR_HORIZONTAL,y*SCALE_FACTOR_VERTICAL))) {
						switch (b.getButtonType()) {
						case ButtonStart: return GAMESCREEN; break;
						case ButtonShop: return SHOPSCREEN; break;
						}
					}
				}
			}
		}

		// Clear Screen
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(renderer);

		// Render objects
		for (int i = 0; i < VIEW_WIDTH / rm.textures[TextureSky].getWidth(); i++) {
			rm.textures[TextureSky].render(16 * i, 0);
			rm.textures[TextureWater].render(16 * i,
				VIEW_HEIGHT - rm.textures[TextureWater].getHeight());
		}
		ecm.renderSurfaceWater();

		// Draw UI Elements
		//FC_DrawAlign(rm.fonts[FontDef12], renderer, VIEW_WIDTH / 2, 0, FC_ALIGN_CENTER, "Score");
		/*rm.textures[TextureButtonStart].render(
			VIEW_WIDTH/2 - rm.textures[TextureButtonStart].getWidth() / 2, 
			VIEW_HEIGHT/2 - rm.textures[TextureButtonStart].getHeight() / 2);
		rm.textures[TextureButtonShop].render(
			VIEW_WIDTH - rm.textures[TextureButtonShop].getWidth() - 2,
			VIEW_HEIGHT - rm.textures[TextureButtonShop].getHeight() - 2);*/
		for (Button& b : buttons) {
			b.render();
		}

		// Update Screen
		SDL_RenderPresent(renderer);
	}
	return -1;
}

int retryScreen(SDL_Renderer* renderer, SDL_Window* window, ResourceManager& rm) {
	return -1;
}

int shopScreen(SDL_Renderer* renderer, SDL_Window* window, ResourceManager& rm) {
	// Create objects
	EyeCandyManager ecm = EyeCandyManager(*renderer, rm);
	// Create buttons
	std::vector<Button> buttons;
	Button startButton(*renderer, rm, ButtonStart);
	startButton.setPosition(
		VIEW_WIDTH - rm.textures[TextureButtonStart].getWidth() - 2,
		VIEW_HEIGHT - rm.textures[TextureButtonStart].getHeight() - 2);
	buttons.push_back(startButton);
	Button backButton(*renderer, rm, ButtonBack);
	backButton.setPosition(
		2,
		VIEW_HEIGHT - rm.textures[TextureButtonBack].getHeight() - 2
	);
	buttons.push_back(backButton);
	
	// Main Loop Flag
	bool quit = false;
	// Event handler
	SDL_Event e;

	// While app is running
	while (!quit) {
		// Handle events
		while (SDL_PollEvent(&e) != 0) {
			// User quits
			if (e.type == SDL_QUIT) {
				quit = true;
			}
			// User presses a key
			else if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.sym) {
				case SDLK_SPACE: break;
				}
			}
			else if (e.type == SDL_MOUSEBUTTONDOWN) {
				int x, y;
				SDL_GetMouseState(&x, &y);
				for (Button& b : buttons) {
					b.checkClicked(x*SCALE_FACTOR_HORIZONTAL,
						y*SCALE_FACTOR_VERTICAL);
				}
			}
			else if (e.type == SDL_MOUSEBUTTONUP) {
				int x, y;
				SDL_GetMouseState(&x, &y);
				for (Button& b : buttons) {
					if (b.isClicked() && (b.checkClicked(
						x*SCALE_FACTOR_HORIZONTAL, y*SCALE_FACTOR_VERTICAL))) {
						switch (b.getButtonType()) {
						case ButtonStart: return GAMESCREEN; break;
						case ButtonBack: return STARTSCREEN; break;
						}
					}
				}
			}
		}

		// Clear Screen
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(renderer);

		// Render objects
		for (int i = 0; i < VIEW_WIDTH / rm.textures[TextureSky].getWidth(); i++) {
			rm.textures[TextureSky].render(16 * i, 0);
			rm.textures[TextureWater].render(16 * i,
				VIEW_HEIGHT - rm.textures[TextureWater].getHeight());
		}
		ecm.renderSurfaceWater();

		// Draw UI Elements
		for (Button& b : buttons)
			b.render();

		// Update Screen
		SDL_RenderPresent(renderer);
	}
	return -1;
}


int gameScreen(SDL_Renderer* renderer, SDL_Window* window, ResourceManager& rm) {
	// Initialize Grid of tiles
	Grid grid = Grid(*renderer, rm);

	// Create player-controlled silhouette
	Silhouette silhouette = Silhouette(*renderer, rm, grid);

	// Initialize Cat for player
	Cat cat = Cat(*renderer, rm, grid);
	
	// Begin wave generation
	WaveMaker waveMaker = WaveMaker(*renderer, rm);
	waveMaker.startWaveCounter();

	// Eye candy handler
	EyeCandyManager ecm = EyeCandyManager(*renderer, rm);

	int score = 0;
	int cameraOffset = 0;
	Uint32 timerStartTime = -1;
	Uint32 timerFinishTime = -1;

	// Main Loop Flag
	bool quit = false;
	bool lost = false;
	// Event handler
	SDL_Event e;

	// While app is running
	while (!quit && !lost) {

		// Handle events
		while (SDL_PollEvent(&e) != 0) {
			// User quits
			if (e.type == SDL_QUIT) {
				quit = true;
			}
			// User presses a key
			else if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.sym) {
				case SDLK_UP: silhouette.rotateLeft(); break;
				case SDLK_DOWN: silhouette.moveDown(); break;
					// might instead want to just move vertically upward until finds next open space
					//silhouette.moveToLowestPosition(); break;
				case SDLK_LEFT: silhouette.moveLeft(); break;
				case SDLK_RIGHT: silhouette.moveRight(); break;
				case SDLK_z: silhouette.rotateRight(); break;
				case SDLK_x: silhouette.rotateLeft(); break;
				case SDLK_a: cat.toggleDirection(); break;
				case SDLK_SPACE: silhouette.placeTile(); break;
				}
			}
		}

		// Move screen left if cat too far right
		if (cat.getTileX() + 1 > VIEW_WIDTH / 16 / 2) {
			grid.shiftTilesLeft();
			cat.shiftLeft();
			silhouette.moveLeft();
			timerStartTime = SDL_GetTicks();
			timerFinishTime = SDL_GetTicks() + 1000;
		}

		// Check wave status
		if (waveMaker.waveHasArrived()) {
			grid.removeBottomRow();
			grid.dropUnstableTiles();
			grid.updateAllTiles();
			cat.fall();
			silhouette.moveToLowestPosition();
			waveMaker.updateWaveDelta(score);
			waveMaker.startWaveCounter();
		}

		// Game Over if Cat falls into water
		if (cat.isWet()) lost = true;

		// Try to move the cat if not already moving
		if (cat.getMovementState() == MovementState::STOPPED &&
			cat.isPath(cat.getDirection()) && cat.getAutoMovement()) {
			if (cat.getDirection() == RIGHT)
				cat.moveToPosition(cat.getTileX() + 1);
			else if (cat.getDirection() == LEFT)
				cat.moveToPosition(cat.getTileX() - 1);
			silhouette.updateValidity();
		} // If moving, see if has reached new location
		else if (cat.getMovementState() != MovementState::STOPPED &&
			cat.isTileTransitionComplete()) {
			if (cat.getNewTileX() > cat.getTileX())
				score++; /// TODO store largest TileX achieved and incr if beyond
			cat.updateToNewPosition();
			if (!grid.isTileAt(cat.getTileX(), cat.getTileY() + 1))
				cat.fall();
			silhouette.updateValidity();
		}

		if (silhouette.isNeedToCheckValidity())
			silhouette.updateValidity();

		// Clear Screen
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(renderer);


		// Update camera position relative to tiles
		// PIXELS * (FRAME_DELTA) / (FRAMES_PER_TRANSIT)
		cameraOffset = -16 * (getFrameDelta(timerStartTime) / (FRAMES_PER_SECOND * 1.0));
		if (cameraOffset < -16) cameraOffset = -16;

		// Render objects
		for (int i = 0; i < VIEW_WIDTH / rm.textures[TextureSky].getWidth(); i++) {
			rm.textures[TextureSky].render(16 * i, 0);
			rm.textures[TextureWater].render(16 * i,
				VIEW_HEIGHT - rm.textures[TextureWater].getHeight());
		}
		ecm.renderSurfaceWater();

		grid.renderTiles(cameraOffset);
		cat.render(cameraOffset);
		// Foreground
		for (int i = 0; i < VIEW_WIDTH / rm.textures[TextureSky].getWidth(); i++) {
			rm.textures[TextureWaterTransparent].render(16 * i,
				VIEW_HEIGHT - rm.textures[TextureWaterTransparent].getHeight());
		}
		ecm.renderSurfaceWater(true);

		silhouette.render(cameraOffset);

		// Draw UI Elements
		//int fontWidth = FC_GetWidth(rm.fonts[FontDef12], "Score");
		FC_DrawAlign(rm.fonts[FontDef12], renderer, VIEW_WIDTH / 2, 0, FC_ALIGN_CENTER, "Score");
		FC_DrawAlign(rm.fonts[FontDef24], renderer, VIEW_WIDTH / 2, 12, FC_ALIGN_CENTER, "%d", score);
		//FC_Draw(rm.fonts[FontDefault], renderer, VIEW_WIDTH/2 - fontWidth, 0,
		//	"Score\n%d", score);
		waveMaker.render();

		// Update Screen
		SDL_RenderPresent(renderer);
	}

	// Check if quitting or just lost
	if (quit) return -1;
	// If lost init post-lost UI
	// Create buttons
	std::vector<Button> buttons;
	Button retryButton(*renderer, rm, ButtonRetry);
	retryButton.setPosition(
		VIEW_WIDTH / 2 - rm.textures[TextureButtonRetry].getWidth() / 2,
		VIEW_HEIGHT / 2 - rm.textures[TextureButtonRetry].getHeight() / 2);
	buttons.push_back(retryButton);
	Button shopButton(*renderer, rm, ButtonShop);
	shopButton.setPosition(
		VIEW_WIDTH - rm.textures[TextureButtonShop].getWidth() - 2,
		VIEW_HEIGHT - rm.textures[TextureButtonShop].getHeight() - 2
	);
	buttons.push_back(shopButton);
	Button backButton(*renderer, rm, ButtonBack);
	backButton.setPosition(
		2,
		VIEW_HEIGHT - rm.textures[TextureButtonBack].getHeight() - 2
	);
	buttons.push_back(backButton);


	while (!quit) {
		// Handle events
		while (SDL_PollEvent(&e) != 0) {
			// User quits
			if (e.type == SDL_QUIT) {
				quit = true;
			}
			// User presses a key
			else if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.sym) {
				case SDLK_SPACE: break;
				}
			}
			else if (e.type == SDL_MOUSEBUTTONDOWN) {
				int x, y;
				SDL_GetMouseState(&x, &y);
				for (Button& b : buttons) {
					b.checkClicked(x*SCALE_FACTOR_HORIZONTAL,
						y*SCALE_FACTOR_VERTICAL);
				}
			}
			else if (e.type == SDL_MOUSEBUTTONUP) {
				int x, y;
				SDL_GetMouseState(&x, &y);
				for (Button& b : buttons) {
					if (b.isClicked() && (b.checkClicked(
						x*SCALE_FACTOR_HORIZONTAL, y*SCALE_FACTOR_VERTICAL))) {
						switch (b.getButtonType()) {
						case ButtonRetry: return GAMESCREEN; break;
						case ButtonShop: return SHOPSCREEN; break;
						case ButtonBack: return STARTSCREEN; break;
						}
					}
				}
			}
		}
		// Clear Screen
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(renderer);

		// Update camera position relative to tiles
		// PIXELS * (FRAME_DELTA) / (FRAMES_PER_TRANSIT)
		//cameraOffset = -16 * (getFrameDelta(timerStartTime) / (FRAMES_PER_SECOND * 1.0));
		//if (cameraOffset < -16) cameraOffset = -16;

		// Render environment 
		for (int i = 0; i < VIEW_WIDTH / rm.textures[TextureSky].getWidth(); i++) {
			rm.textures[TextureSky].render(16 * i, 0);
			rm.textures[TextureWater].render(16 * i,
				VIEW_HEIGHT - rm.textures[TextureWater].getHeight());
		}
		ecm.renderSurfaceWater();

		//rm->textures[TextureBlock].render(0, 0);
		grid.renderTiles(cameraOffset);
		//cat.render(cameraOffset);
		//silhouette.render(cameraOffset);

		// Foreground
		for (int i = 0; i < VIEW_WIDTH / rm.textures[TextureSky].getWidth(); i++) {
			rm.textures[TextureWaterTransparent].render(16 * i,
				VIEW_HEIGHT - rm.textures[TextureWaterTransparent].getHeight());
		}
		ecm.renderSurfaceWater(true);

		// Draw UI Elements
		//int fontWidth = FC_GetWidth(rm.fonts[FontDef12], "Score");
		FC_DrawAlign(rm.fonts[FontDef12], renderer, VIEW_WIDTH / 2, 0, FC_ALIGN_CENTER, "Score");
		FC_DrawAlign(rm.fonts[FontDef24], renderer, VIEW_WIDTH / 2, 12, FC_ALIGN_CENTER, "%d", score);
		//FC_Draw(rm.fonts[FontDefault], renderer, VIEW_WIDTH/2 - fontWidth, 0,
		//	"Score\n%d", score);
		//waveMaker.render();
		for (Button& b : buttons) {
			b.render();
		}

		// Update Screen
		SDL_RenderPresent(renderer);

	}
	return -1;
}


int main(int argc, char* args[]) {

	// Initialize SDL, window, and renderer
	if (init() == false) {
		printf("Failed to initialize SDL\n");
		return -1;
	}
	SDL_Window* window = setupWindow();
	SDL_Renderer* renderer = setupRenderer(*window);
	if (window == NULL) { printf("WINDOW IS NULL\n"); return -1; }
	if (renderer == NULL) {	printf("RENDERER IS NULL!"); return -1; }

	SDL_RenderSetLogicalSize(renderer, VIEW_WIDTH, VIEW_HEIGHT); // scales renderer

	// Initialize a Resource Manager
	ResourceManager rm = ResourceManager(*renderer);

	// Load media
	if (loadMedia(rm, *renderer) == false) {
		printf("Failed to load media\n");
		return -2;
	}

	// Main Loop Flag
	bool quit = false;
	// Event handler
	SDL_Event e;

	// Screen return
	int nextScreen = STARTSCREEN;

	// While app is running
	while (!quit) {

		// Get screen return
		switch (nextScreen) {
		case STARTSCREEN: nextScreen = startScreen(renderer, window, rm);
			break;
		case GAMESCREEN: nextScreen = gameScreen(renderer, window, rm);
			break;
		case RETRYSCREEN: nextScreen = retryScreen(renderer, window, rm);
			break;
		case SHOPSCREEN: nextScreen = shopScreen(renderer, window, rm);
			break;
		case -1: quit = true;
		}
	}

	// Free resources and close SDL
	close(renderer, window, rm);

	return 0;
}