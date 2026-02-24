#include "gui.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdio.h>
#include <stdbool.h>

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
    
    free(app->chats_rect);
    SDL_DestroyRenderer(app->renderer);
    SDL_DestroyWindow(app->window);
    SDL_Quit();
    exit(exit_status);
}

void update_gui(struct GUIState* app, struct showData* data){
    
    int num = data->count;
    free(app->chats_rect);
    
    app->chats_rect = (SDL_Rect* )malloc(num * sizeof(SDL_Rect));

    if (app->chats_rect == NULL) gui_cleanup(app, EXIT_FAILURE);

    app->chats_count = num;

    int x = 0, y = 30;
    for(int i = 0; i < num; i++){
        
        app->chats_rect[i] = (SDL_Rect){
            .x = x,
            .y = y,
            .w = 400,
            .h = 70
        };

        y+=100;
    }
}
