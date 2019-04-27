/* TODO
 * [DONE] Limit Framerate 
 * [DONE] Load and Render an image
 *
 * Load and Play audio
 */

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

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
SDLGlobals sdl     = {0};

#define TEMPORARY_STRING_SIZE 1024
char temporary_string[1024];

#include "render.c"
#include "game.c"

void cleanup_sdl()
{
    if (sdl.renderer) SDL_DestroyRenderer(sdl.renderer);
    if (sdl.window)   SDL_DestroyWindow(sdl.window);

    SDL_Quit();
}

int log_error_and_cleanup_sdl(char *message)
{
    SDL_Log("%s: %s", message, SDL_GetError());
    cleanup_sdl();
    return 1;
}

u64 microtime()
{
    return (SDL_GetPerformanceCounter() * 1000000) / sdl.performance_frequency;
}

int main(int argc, char **argv)
{
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0) {
        return log_error_and_cleanup_sdl("Unable to initialize SDL");
    }

    sdl.performance_frequency = SDL_GetPerformanceFrequency();

    sdl.window = SDL_CreateWindow("Test", 
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600,
            0);
    if (!sdl.window) {
        return log_error_and_cleanup_sdl("Unable to create window");
    }

    sdl.renderer = SDL_CreateRenderer(sdl.window, -1,
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!sdl.renderer) {
        return log_error_and_cleanup_sdl("Unable to create renderer");
    }

    setup_game();

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

        SDL_SetRenderDrawColor(sdl.renderer, 64, 128, 255, SDL_ALPHA_OPAQUE);
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

    cleanup_sdl();

    return 0;
}
