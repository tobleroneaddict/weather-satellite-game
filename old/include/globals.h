#pragma once
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL_time.h>
#include <SDL3/SDL_main.h>
#include <glm/glm.hpp>
#include <SDL3/SDL.h>
#include <iostream>
#include <stdio.h>
#include <cstdint>
#include <vector>
#include <cmath>

extern int* gFrameBuffer;
extern SDL_Window* sdl_window;
extern SDL_Renderer* sdl_renderer;
extern SDL_Texture* sdl_texture;
extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;
extern int MAX_WORLD_X;
extern int MAX_WORLD_Y;
extern double deltaTime;


