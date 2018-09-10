#pragma once
#include <SDL.h>

#include "SDL_FontCache.h"
#include "Texture.h"

enum Resources {
	TextureSilhouetteSheet = 0,
	TextureSilhouetteInvalidSheet,
	TextureBlockShadow,
	TextureBlock,
	TextureBlockFalling,
	TextureWater,
	TextureWaterTransparent,
	TextureWaterSurfaceSheet,
	TextureWaterSurfaceTransparentSheet,
	TextureSky,
	TextureCat,
	TextureWaveMeterSheet,
	TextureButtonStart,
	TextureButtonBack,
	TextureButtonRetry,
	TextureButtonShop,
	TextureTotal
};

enum ButtonType {
	ButtonStart = 0,
	ButtonRetry,
	ButtonBack,
	ButtonShop
};

enum Fonts {
	FontDef24 = 0,
	FontDef12 = 1,
	FontTotal = 2
};

enum BlockTiles {
	BlockEmpty = 0,
	BlockTopLeft = 1,
	BlockTopCenter = 2,
	BlockTopRight = 3,
	BlockLeft = 4,
	BlockCenter = 5,
	BlockRight = 6,
	BlockSingleTop = 7,
	BlockSingleBot = 8,
	BlockBottomLeftCorner = 9,
	BlockBottomRightCorner = 10,
	BlockTilesTotal = 11
};
enum SilhouetteTiles {
	TileEmpty = 0,
	TileTopLeft, TileTopCenter, TileTopRight, TilePipeTopLeft, TilePipeTopRight, 
		TileTopHorizontalPipeLeft, TileTopHorizontalPipeRight, TileSingle, 
		TileInnerFour, TileInnerDiagDown, TileInnerTwoTop,
	TilePipeTop, TileLeft, TileCenter, TileRight, TilePipeBotLeft, 
		TilePipeBotRight, TileBotHorizontalPipeLeft, TileBotHorizontalPipeRight, 
		TilePipeTLeft, TilePipeTRight, TileInnerDiagUp, TileInnerTwoRight,
	TilePipeVertical, TileBotLeft, TileBot, TileBotRight, TileInnerCornerTopLeft,
		TileInnerCornerTopRight, TileLeftVerticalPipeTop, 
		TileRightVerticalPipeTop, TilePipeTTop, TileInnerThreeTL, 
		TileInnerThreeTR, TileInnerTwoBot,
	TilePipeBot, TilePipeLeft, TilePipeHorizontal, TilePipeRight, 
		TileInnerCornerBotLeft, TileInnerCornerBotRight, TileLeftVerticalPipeBot,
		TileRightVerticalPipeBot, TilePipeTBot, TileInnerThreeBL, 
		TileInnerThreeBR, TileInnerTwoLeft,
	SilhouetteTilesTotal

};


class ResourceManager {
public:
	Texture textures[TextureTotal];
	FC_Font* fonts[FontTotal];
	SDL_Rect silhouetteClips[SilhouetteTilesTotal];
	SDL_Rect silhouetteInvalidClips[SilhouetteTilesTotal];
	SDL_Rect blockClips[BlockTilesTotal];
	SDL_Rect waterSurfaceClips[12];
	SDL_Rect waveMeterClips[45];

	ResourceManager(SDL_Renderer& renderer);
	void setTexture(Texture& t, int num);
	void clearTexture(int num);
	void clearAllTextures();
	void clearFonts();

	void initializeBlockTileClips();
	void initializeSilhouetteTileClips();
	void initializeSilhouetteInvalidTileClips();
	void initializeWaveMeterClips();
	void initializeWaterSurfaceClips();
	void initializeFonts();

	SDL_Renderer& renderer;
};
