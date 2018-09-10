#pragma once
#include <map>

const int SCREEN_WIDTH = 960; //960		//640x480 for standard
const int SCREEN_HEIGHT = 720; //720
const int VIEW_WIDTH = 320;
const int VIEW_HEIGHT = 240;
const double SCALE_FACTOR_HORIZONTAL = (double)VIEW_WIDTH / SCREEN_WIDTH;
const double SCALE_FACTOR_VERTICAL = (double)VIEW_HEIGHT / SCREEN_HEIGHT;

const int BLOCK_SIZE = 16;

const int MAX_ROWS = VIEW_HEIGHT / 16;
const int MAX_COLUMNS = VIEW_WIDTH / 16;

const int FRAMES_PER_SECOND = 60;
const int MILLISECONDS_PER_FRAME = 17;

const int LEFT = 0;
const int RIGHT = 1;