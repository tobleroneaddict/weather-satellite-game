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
//double physics_deltaTime = 0;
double universal_time = 862790195;

void clear_ram(uint8_t* ram) {
    for (unsigned long y = 0; y < sizeof(ram); y++) {ram[y] = y % 128;} //garbage data
    return;
}