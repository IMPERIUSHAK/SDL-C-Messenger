#include "gui.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <stdio.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

//function for first initialization to check and set everything  before start-up  
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
char* who_sent(enum MessageType bywho, char* str) {
    char *prefix;

    
    if (bywho == MESSAGE_INCOMING) {
        prefix = "Me: ";

    } else if (bywho == MESSAGE_OUTGOING) {
        prefix = "Friend: ";

    } else {
        return strdup(str);
    }
    
    char *new_text = malloc(strlen(prefix) + strlen(str) + 1);
    
    if (!new_text) return NULL;
   
    strcpy(new_text, prefix);
    strcat(new_text, str);

    return new_text;
}

//update all
bool update_gui(struct GUIState* app){
   
    int num = app->messages.count;

    if (app->chats.items){
       for (int i = 0; i < app->messages.count; i++){
            SDL_DestroyTexture(app->chats.items[i].texture);
       }
       free(app->chats.items);
       app->chats.items = NULL;
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

        //who_sent(bywho, name);
        
        char* nm = who_sent(bywho, name);

        //Font Init
        SDL_Surface *textSurface = TTF_RenderText_Blended(app->text_font, nm, color);
        free(nm);
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

// this function will change a color of message rectangle if mouse cover it
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
//This method updates `userinput sctruct` if everytime our programm modifies it
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

//As you can understand from name of this function, it will render text input for every frame
void render_text_input(struct GUIState *app){

    SDL_RenderFillRect(app->renderer, &app->userinput.rect);

    int textW, textH;
    SDL_QueryTexture(app->userinput.texture, NULL, NULL, &textW, &textH);
    SDL_Rect dstRect = center_text_rect(app->userinput.rect, textW, textH);

    SDL_RenderCopy(app->renderer, app->userinput.texture, NULL, &dstRect);    

}


//Checks if your in InputField(clicked with mouse)
bool isInputActive(struct GUIState* app){

    int x = 0, y = 0;

    SDL_GetMouseState(&x, &y);
    app->userinput.isactive = false;
    if (point_in_rect(x, y, &app->userinput.rect)){
        app->userinput.isactive = true;
        return true;
    }

    return false;
}


void send_message(struct GUIState *app){
    
    char* str = app->userinput.userinput;
    int len = strlen(str);
    
    if (len == 0) return;
 
    /*printf("%s\n", str);
    
    memset(app->userinput.userinput, 0, sizeof(app->userinput.userinput));
    app->userinput.texture = NULL;
    */

    struct Message *msg = (struct Message*)malloc(sizeof(struct Message));
    if (!msg){
        fprintf(stderr, "Error to create struct for new  message\n");
        return;
    }

    msg->text = (char *)malloc((len * sizeof(char))+1);
    if (!msg->text){
        fprintf(stderr, "Error to malloc message size");
        free(msg);
        return;
    }

    strcpy(msg->text, str);
    msg->text[len] = '\0';
    

    msg->type = MESSAGE_OUTGOING;
    if (!msg->type){
        fprintf(stderr, "Error while creating timestamp for new `message`");
        free(msg);
        return;
    }

    msg->timestamp = time(NULL);
    if (!msg->timestamp){
        fprintf(stderr, "Error while adding `timestamp` to new `message`");
        free(msg);
    }

    pthread_mutex_lock(&app->json_queue.mutex);
    queue_push(&app->json_queue, msg);
    pthread_cond_wait(&app->json_queue.cond, &app->json_queue.mutex);
    pthread_mutex_unlock(&app->json_queue.mutex);

    free(msg->text);
    free(msg);
    
    memset(app->userinput.userinput, 0, sizeof(app->userinput.userinput));
    app->userinput.texture = NULL;
}
