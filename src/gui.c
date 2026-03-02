#include "gui.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdio.h>
#include <stdbool.h>

//function for first initialization to check everything  before start-up  
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
    
    app->text_font = TTF_OpenFont("fonts/freesansbold.ttf", 15);
    if(!app->text_font){
        fprintf(stderr, "Error with initialization fon:%s ",TTF_GetError());
        return true;
    }
    // app->input_rect = (SDL_Rect){
    //     .x = 0,
    //     .y = SCREEN_HEIGHT - 70,
    //     .w = SCREEN_WIDTH,
    //     .h = 70
    // };
    
    // SDL_SetTextInputRect (&app->input_rect);

    app->userinput.rect = (SDL_Rect){
        .x = 0,
        .y = SCREEN_HEIGHT - 70,
        .w = SCREEN_WIDTH,
        .h = 70
    };

    return false;

}

//app clean app function
void gui_cleanup(struct GUIState* app, int exit_status){
    
    for (int i = 0; i < app->chats.count; i++){
        SDL_DestroyTexture(app->chats.items[i].texture);
    }
    free(app->chats.items); 
    SDL_DestroyTexture(app->userinput.texture);
    SDL_DestroyRenderer(app->renderer);
    SDL_DestroyWindow(app->window);

        SDL_Quit();
    exit(exit_status);
}


//define sender
void who_sent(enum MessageType bywho, char* str) {
    char *prefix;
    size_t prefix_len;
    
    if (bywho == MESSAGE_INCOMING) {
        prefix = "Me: ";
        prefix_len = 4;
    } else if (bywho == MESSAGE_OUTGOING) {
        prefix = "Friend: ";
        prefix_len = 8;
    } else {
        return;
    }
    
    char *new_text = malloc(prefix_len + strlen(str) + 1);
    
    if (new_text != NULL) {
        
        strcpy(new_text, prefix);
        strcat(new_text, str);
        strcpy(str, new_text);

        free(new_text);
    }
}

//update all
bool update_gui(struct GUIState* app){
    
    int num = app->messages.count;

    if (app->chats.items){
        free(app->chats.items);
    }

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
        
        char *name = app->messages.items[i].text;
        enum MessageType bywho = app->messages.items[i].type;
        
        who_sent(bywho, name);

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
        
        if(i % 2 !=0 ){ x+= (SCREEN_WIDTH - 300);}
        else if(i != 0 && i % 2 == 0){ x-= (SCREEN_WIDTH - 300);}
        //setint rect to chats
        app->chats.items[i].rect = (SDL_Rect){
            .x = x,
            .y = y,
            .w = 300,
            .h = 70
        };

        y+=70;
        i++;
    }

    return false;
}

//function to center the string inside rect
SDL_Rect center_text_rect(SDL_Rect r, int w, int h){
    
    SDL_Rect dst = {
        .x = r.x + (r.w - w)/2,
        .y = r.y + (r.h - h)/2,
        .w = w,
        .h = h,
    };

    return dst;
}

//function to render chat in every frame
void render_chats(struct GUIState* app) {
    
    for (int i = 0; i < app->chats.count; i++) {
        if (!app->chats.items[i].texture) continue;
        
        SDL_SetRenderDrawColor(app->renderer, 100, 0, 100, 0);
        SDL_RenderFillRect(app->renderer, &app->chats.items[i].rect);

        int textW, textH;
        SDL_QueryTexture(app->chats.items[i].texture, NULL, NULL, &textW, &textH);

        SDL_Rect dstRect = center_text_rect(app->chats.items[i].rect, textW, textH);
        
        // system("clear");
        // printf("%s", app->userinput);        
        
        SDL_RenderCopy(app->renderer, app->chats.items[i].texture, NULL, &dstRect);
    }

}

//hover function for mouse cursor
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

bool update_text_input(struct GUIState *app){

    char *txt = app->userinput.userinput;
    SDL_Color color = BASE_COLOR;

    SDL_Surface *textSurface = TTF_RenderText_Blended(app->text_font, txt, color);
    if(!textSurface){
        return true;
    }

    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(app->renderer, textSurface);
    SDL_FreeSurface(textSurface);
    if(!textTexture){
        return true;
    }

    app->userinput.texture = textTexture;

    app->userinput.rect = (SDL_Rect){
        .x = 0,
        .y = SCREEN_HEIGHT - 70,
        .w = SCREEN_WIDTH,
        .h = 70,
    };

    return false;
}

void render_text_input(struct GUIState *app){

    SDL_RenderFillRect(app->renderer, &app->userinput.rect);

    int textW, textH;
    SDL_QueryTexture(app->userinput.texture, NULL, NULL, &textW, &textH);
    SDL_Rect dstRect = center_text_rect(app->userinput.rect, textW, textH);

    SDL_RenderCopy(app->renderer, app->userinput.texture, NULL, &dstRect);    

}

bool isInputActive(struct GUIState* app){

    int x = 0, y = 0;

    SDL_GetMouseState(&x, &y);

    if (point_in_rect(x, y, &app->userinput.rect)){
        app->userinput.isactive = true;
        return true;
    }

    return false;
}