//This is a skeleton for my future games :3

#include "globals.h"
#include "satellite.h"

using namespace std;

Uint64 NOW = SDL_GetPerformanceCounter();
Uint64 LAST = 0;

//deltatime in the globals so other can use it

Satellite sat;

//Game init stuff
void init() {
    sat.init();
    sat.step_simulation();
    sat.physics.attitude = quat(vec3(45,45,45));

    SDL_HideCursor();

    //Already inserted in orbit
    sat.physics.STAR_37_XFB_FUEL = 0;
    sat.physics.HYDRAZINE_FUEL = 4.98f;

}


//main loop
int loop() {

    
    
    sat.step_simulation();
    //sat.tantenna.uplink.data_verb = GET_TIME_BASE;

    //if (sat.tantenna.downlink.result != 0)
    //{cout << "Result reads: " << sat.tantenna.downlink.result << endl;}


    //cout << deltaTime;

    //Input handler
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        //Quit events
        if (e.type == SDL_EVENT_QUIT)   {return 1;}   if (e.type == SDL_EVENT_KEY_UP && e.key.key == SDLK_ESCAPE){return 1;}
        if (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_E) {
            sat.physics.kick_motor(2537.49*1000);
        }


        //.......................... other key events here`
    }

    //Draw stuff
    SDL_SetRenderDrawColor(sdl_renderer,0,0,0,255);
    //SDL_RenderClear(sdl_renderer);

    

    float x = sat.physics.POS.x / 50000;
    float y = sat.physics.POS.y / 50000;
    float z = sat.physics.POS.z / 50000;
    x += 400;
    y += 400;
    z += 400;

    SDL_SetRenderDrawColor(sdl_renderer,250,250,250,255);
    SDL_RenderLine(sdl_renderer,x+800,z,x+800,z);
    SDL_RenderLine(sdl_renderer,x+800,y,x+800,y);
    SDL_RenderLine(sdl_renderer,400+800,400,400+800,400);
    SDL_RenderLine(sdl_renderer,400+800,400 - (6378137/50000),400+800,400 + (6378137/50000));
    SDL_RenderLine(sdl_renderer,400+800 - (6378137/50000),400,400+800 + (6378137/50000),400);
    //SDL_RenderLine(sdl_renderer,600,600,y,z);
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

    //sat.step_simulation();
    //sat.tantenna.uplink.data_verb = GET_TIME_BASE;
    

    //Example: Setting the time base
    sat.tantenna.uplink.data_verb = SET_TIME_BASE;
    sat.tantenna.uplink.x = universal_time; //Set X register to universal time
    sat.tantenna.uplink.TTG = 0; // Instant execute
    sat.step_simulation(); //Flush to stack
    cout << "Onboard RTC has been set to: " << sat.tantenna.downlink.result << endl; //Check RTC (SET_TIME_BASE txs this)

    //Run game loop and get dT
    while (true) {
        LAST = NOW;NOW = SDL_GetPerformanceCounter(); deltaTime = (double)((NOW - LAST) / (double)SDL_GetPerformanceFrequency() );
        deltaTime = deltaTime * 1000;
        universal_time += deltaTime; //Ground station
        if (loop()) break;
        //SDL_Delay(100);
    }

    //Termination
    SDL_DestroyRenderer(sdl_renderer);
    SDL_DestroyWindow(sdl_window);
    TTF_Quit(); SDL_Quit();
    //free(textures);

    return 0;
}