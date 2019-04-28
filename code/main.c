#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include <SDL2/SDL.h>

typedef Sint8  s8;
typedef Sint16 s16;
typedef Sint32 s32;
typedef Sint64 s64;
typedef Uint8  u8;
typedef Uint16 u16;
typedef Uint32 u32;
typedef Uint64 u64;

typedef struct SDLGlobals 
{
    SDL_Window   *window;
    SDL_Renderer *renderer;
    u64           performance_frequency;
}
SDLGlobals;

// Global Variables
SDLGlobals  sdl = {0};

#define TEMPORARY_STRING_SIZE 5012
char *temporary_string;

int window_width = 640;
int window_height = 480;
int render_width = 320;
int render_height = 240;

#include "globals.c"
#include "input.c"
#include "render.c"
#include "font.c"
#include "sprite.c"
#include "events.c"
#include "sky.c"
#include "game.c"

void cleanup_sdl()
{
    if (sdl.renderer) SDL_DestroyRenderer(sdl.renderer);
    if (sdl.window)   SDL_DestroyWindow(sdl.window);

    SDL_Quit();
}

int log_error_and_cleanup_sdl(char *message)
{
    SDL_Log("%s", message);
    cleanup_sdl();
    return 1;
}

int log_sdl_error_and_cleanup_sdl(char *message)
{
    SDL_Log("%s: %s", message, SDL_GetError());
    cleanup_sdl();
    return 1;
}

u64 microtime()
{
    return (SDL_GetPerformanceCounter() * 1000000) / sdl.performance_frequency;
}

void toggle_fullscreen()
{
    if (SDL_GetWindowFlags(sdl.window) & SDL_WINDOW_FULLSCREEN_DESKTOP) 
        SDL_SetWindowFullscreen(sdl.window, 0);
    else
        SDL_SetWindowFullscreen(sdl.window, SDL_WINDOW_FULLSCREEN_DESKTOP);
}

int main(int argc, char **argv)
{
    srand(time(0));

    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0) {
        return log_sdl_error_and_cleanup_sdl("Unable to initialize SDL");
    }

    sdl.performance_frequency = SDL_GetPerformanceFrequency();

#ifdef NDEBUG
    int window_x = SDL_WINDOWPOS_CENTERED;
#else
    SDL_Rect display_bounds;
    SDL_GetDisplayBounds(0, &display_bounds);
    int window_x = display_bounds.w / 2;
#endif

    sdl.window = SDL_CreateWindow("Test", 
            window_x, SDL_WINDOWPOS_CENTERED, 
            window_width, window_height, 0);
    if (!sdl.window) {
        return log_sdl_error_and_cleanup_sdl("Unable to create window");
    }

    sdl.renderer = SDL_CreateRenderer(sdl.window, -1,
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!sdl.renderer) {
        return log_sdl_error_and_cleanup_sdl("Unable to create renderer");
    }

    SDL_RenderSetLogicalSize(sdl.renderer, render_width, render_height);

    temporary_string = malloc(TEMPORARY_STRING_SIZE);

    if (!setup_input()) {
        return log_error_and_cleanup_sdl("Error setting up input");
    }

    if (!setup_game()) {
        return log_error_and_cleanup_sdl("Error setting up game");
    }

    u64 start_time    = microtime();
    u64 previous_time = 0;
    u64 elapsed_time  = 0;

    u64 fps_timer = 0;
    u32 target_fps = 60;
    u64 time_per_frame = 1000000 / target_fps;
    u32 fps = 0;

    SDL_Event event;
    bool running = true;

    while (running) {
        previous_time = start_time;
        start_time = microtime();

        elapsed_seconds = ((double)start_time - (double)previous_time) / 1000000.0;
        
        fps_timer += start_time - previous_time;

        while (fps_timer >= 1000000) {
            fps_timer -= 1000000;

            snprintf(temporary_string, TEMPORARY_STRING_SIZE, "FPS: %u", fps);
            SDL_SetWindowTitle(sdl.window, temporary_string);

            fps = 0;
        }

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        if (!running) break;

        update_input();

        if (key_just_down(fullscreen_key)) {
            toggle_fullscreen();
        }

        set_hex_color(0xff4d4222);
        SDL_RenderClear(sdl.renderer);

        if (!update_game()) {
            running = false;
            break;
        }

        SDL_RenderPresent(sdl.renderer);

        while (( elapsed_time = microtime() - start_time ) < time_per_frame) {
            SDL_Delay((u32) ((time_per_frame - elapsed_time) / 1000));
        }

        ++fps;
    }

    destroy_game();

    cleanup_sdl();

    return 0;
}
