#include "gui.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
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
    if (!app->renderer){
        fprintf(stderr, "Error Creating renderer: %s\n", SDL_GetError());
        return true;
    }

    if (TTF_Init()){
        fprintf(stderr, "Error initializing Text: %s\n", TTF_GetError());
        return true;
    }
    
    app->text_font = TTF_OpenFont("fonts/freesansbold.ttf", 20);
    if(!app->text_font){
        fprintf("Error with initialization fon:%s ",TTF_GetError());
    }
    
    return false;

}

void gui_cleanup(struct GUIState* app, int exit_status){
    
    free(app->chats_rect);
    
    for (int i = 0; i < app->chats_count; i++){
        SDL_DestroyTexture(app->text_image[i]);
    }
    free(app->text_image); 
    
    SDL_DestroyRenderer(app->renderer);
    SDL_DestroyWindow(app->window);

        SDL_Quit();
    exit(exit_status);
}

bool update_gui(struct GUIState* app, struct showData* data){
    
    int num = data->count;

        
    if (app->chats_rect){
        free(app->chats_rect);
        app->chats_rect = NULL;
    }

    if (app->text_image) {
        for (int i = 0; i < app->chats_count; i++){
            SDL_DestroyTexture(app->text_image[i]);
        }
        free(app->text_image);
        app->text_image = NULL;
    }
    
    
    app->chats_rect = (SDL_Rect* )malloc(num * sizeof(SDL_Rect));
    app->text_image = (SDL_Texture** )malloc(num * sizeof(SDL_Texture*));

    SDL_Color color = {255, 255, 255, 255};

    app->chats_count = num;

    int x = 0, y = 30;
    
    for(int i = 0; i < num; i++){
        
        const char *name = data->users[i];
        
        SDL_Surface *textSurface = TTF_RenderText_Blended(app->text_font, name, color);
        if (!textSurface){
            return true;
        }

        SDL_Texture *textTexture = SDL_CreateTextureFromSurface(app->renderer, textSurface);
        SDL_FreeSurface(textSurface);
        if (!textTexture) return true;

        app->text_image[i] = textTexture;
        
        app->chats_rect[i] = (SDL_Rect){
            .x = x,
            .y = y,
            .w = 400,
            .h = 70
        };

        y+=100;
    }

    return false;
}

void render_chats(struct GUIState *app) {
    
    for (int i = 0; i < app->chats_count; i++) {
        if (!app->text_image[i]) continue;
        
        SDL_SetRenderDrawColor(app->renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(app->renderer, &app->chats_rect[i]);

        int textW, textH;
        SDL_QueryTexture(app->text_image[i], NULL, NULL, &textW, &textH);

        SDL_Rect dstRect = {
            .x = app->chats_rect[i].x + (app->chats_rect[i].w - textW)/2,
            .y = app->chats_rect[i].y + (app->chats_rect[i].h - textH)/2,
            .w = textW,
            .h = textH
        };

        SDL_RenderCopy(app->renderer, app->text_image[i], NULL, &dstRect);
    }
}