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
        fprintf(stderr, "Error with initialization fon:%s ",TTF_GetError());
    }
    
    return false;

}


//app clean app function
void gui_cleanup(struct GUIState* app, int exit_status){
    
    for (int i = 0; i < app->chats.count; i++){
        SDL_DestroyTexture(app->chats.items[i].texture);
    }
    free(app->chats.items); 
    
    SDL_DestroyRenderer(app->renderer);
    SDL_DestroyWindow(app->window);

        SDL_Quit();
    exit(exit_status);
}


//update all
bool update_gui(struct GUIState* app, struct showData* data){
    
    int num = data->count;
        
    if (app->chats.items){
        free(app->chats.items);
    }

    //free up space for updated/new chats
    app->chats.items = malloc(num * sizeof(*app->chats.items));
    if(!app->chats.items){
        fprintf(stderr, "Error with Items init");
        return true;
    }

    SDL_Color color = BASE_COLOR;

    app->chats.count = num;
    
    int scroll = app->chats.scroll_offset;

    int x = 0, y = 0, i = scroll;

    
    //update chats
    while(y < SCREEN_WIDTH && i < num){
        
        const char *name = data->users[i];
        
        //Font Init
        SDL_Surface *textSurface = TTF_RenderText_Blended(app->text_font, name, color);
        if (!textSurface){
            return true;
        }
        
        //creating surface and texture for chats
        SDL_Texture *textTexture = SDL_CreateTextureFromSurface(app->renderer, textSurface);
        SDL_FreeSurface(textSurface);
        
        if (!textTexture) return true;
        app->chats.items[i].texture = textTexture;
        
        //setint rect to chats
        app->chats.items[i].rect = (SDL_Rect){
            .x = x,
            .y = y,
            .w = 400,
            .h = 70
        };

        y+=70;
        i++;
    }

    return false;
}

SDL_Rect center_text_rect(SDL_Rect r, int w, int h){
    
    SDL_Rect dst = {
        .x = r.x + (r.w - w)/2,
        .y = r.y + (r.h - h)/2,
        .w = w,
        .h = h,
    };

    return dst;
}

void render_chats(struct GUIState* app) {
    
    for (int i = 0; i < app->chats.count; i++) {
        if (!app->chats.items[i].texture) continue;
        
        SDL_SetRenderDrawColor(app->renderer, 100, 0, 100, 0);
        SDL_RenderFillRect(app->renderer, &app->chats.items[i].rect);

        int textW, textH;
        SDL_QueryTexture(app->chats.items[i].texture, NULL, NULL, &textW, &textH);

        SDL_Rect dstRect = center_text_rect(app->chats.items[i].rect, textW, textH);

        SDL_RenderCopy(app->renderer, app->chats.items[i].texture, NULL, &dstRect);
    }

}


bool point_in_rect(int x, int y, SDL_Rect *r) {
    return (x >= r->x &&
            x <= r->x + r->w &&
            y >= r->y &&
            y <= r->y + r->h);
}



void mouse_hover(struct GUIState* app){

    int n = app->chats.count;
    int x = 0, y = 0;

    for(int i = 0; i < n; i++){
        
        SDL_GetMouseState(&x, &y);
        
        if (!point_in_rect(x, y, &app->chats.items[i].rect)){ continue;}

        SDL_SetRenderDrawColor(app->renderer, 200, 50, 50, 255);
        SDL_RenderFillRect(app->renderer, &app->chats.items[i].rect);

        int textW, textH;
        SDL_QueryTexture(app->chats.items[i].texture, NULL, NULL, &textW, &textH);
        SDL_Rect dstRect = center_text_rect(app->chats.items[i].rect, textW, textH);

        SDL_RenderCopy(app->renderer, app->chats.items[i].texture, NULL, &dstRect);

    }

}