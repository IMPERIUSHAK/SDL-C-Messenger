#include "gui.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdio.h>
#include <stdbool.h>

static const char *str[] = {"AKO", "AMIN", "BCHFV", "N3SHOW", "HAMZA4IK"};
static const int str_count = sizeof(str)/sizeof(str[0]);

bool initialize_gui(struct GUIState* app){

    if(SDL_Init(SDL_INIT_EVERYTHING)) {
        fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
        return true;
    }

    app->window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED,
                                    SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH,
                                    SCREEN_HEIGHT, 0);
    if (!app->window){
        fprintf(stderr, "Error Creating window: %s\n", SDL_GetError());
        return true;
    }

    app->renderer = SDL_CreateRenderer(app->window, -1, 0);
    if(!app->renderer){
        fprintf(stderr, "Error Creating renderer: %s\n", SDL_GetError());
        return true;
    }

    return false;
}

void gui_cleanup(struct GUIState* app, int exit_status){
    SDL_DestroyWindow(app->window);
    SDL_DestroyRenderer(app->renderer);

    SDL_Quit();
    exit(exit_status);
}