#include "globals.h"
#include "satellite.h"

using namespace std;

Uint64 NOW = SDL_GetPerformanceCounter();
Uint64 LAST = 0;
//deltatime in the globals so other can use it

Satellite sat;

//Game init stuff
void init() {
    SDL_HideCursor();

    sat.dp.init();
}

//main loop
int loop() {
    //cout << deltaTime;

    //Input handler
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        //Quit events
        if (e.type == SDL_EVENT_QUIT)   {return 1;}   if (e.type == SDL_EVENT_KEY_UP && e.key.key == SDLK_ESCAPE){return 1;}



        //.......................... other key events here`
    }

    //Draw stuff
    SDL_SetRenderDrawColor(sdl_renderer,100,150,100,255);
    SDL_RenderClear(sdl_renderer);


    SDL_RenderPresent(sdl_renderer);
    return 0;
}

int main() {
    //Setup window
    gFrameBuffer = new int[WINDOW_WIDTH * WINDOW_HEIGHT];
    sdl_window = SDL_CreateWindow("Demo Game", WINDOW_WIDTH, WINDOW_HEIGHT,SDL_WINDOW_RESIZABLE);
    //Setup font
    if (!TTF_Init()) {cout << "TTF Error!\n"; return 1;}
    TTF_Font* font = TTF_OpenFont("Assets/InclusiveSans-Medium.ttf", 24);
    //Setup renderer
    sdl_renderer = SDL_CreateRenderer(sdl_window, NULL);
    SDL_SetRenderVSync(sdl_renderer, 0); //i had to do this to fix stuttying
    sdl_texture = SDL_CreateTexture(sdl_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, WINDOW_WIDTH, WINDOW_HEIGHT);
    SDL_SetTextureScaleMode(sdl_texture,SDL_SCALEMODE_NEAREST);

    //Check errors
    if (!gFrameBuffer || !sdl_window || !sdl_renderer || !sdl_texture) {
        cout << "SDL3 init error!\n";
        return 1;
    }
    if (font == nullptr) { cout << "Font error!\n"; return 1;}
    //END PRE INIT

    init(); //Place any init things here

    //Run game loop and get dT
    //while (true) {
    //    LAST = NOW;NOW = SDL_GetPerformanceCounter(); deltaTime = (double)((NOW - LAST)*1000 / (double)SDL_GetPerformanceFrequency() );
    //    if (loop()) break;
    //}

    //Termination
    SDL_DestroyRenderer(sdl_renderer);
    SDL_DestroyWindow(sdl_window);
    TTF_Quit(); SDL_Quit();
    //free(textures);

    return 0;
}