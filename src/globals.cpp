#include "../include/globals.h"

int* gFrameBuffer = nullptr;
SDL_Window* sdl_window = nullptr;
SDL_Renderer* sdl_renderer = nullptr;
SDL_Texture* sdl_texture = nullptr;
int WINDOW_WIDTH = 1300;
int WINDOW_HEIGHT = 900;
int MAX_WORLD_X = 3000;
int MAX_WORLD_Y = 3000;
double deltaTime = 0; //milliseconds