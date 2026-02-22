#ifndef GUI_H
#define GUI_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdio.h>
#include <stdbool.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define WINDOW_TITLE "HaNaKo"

struct GUIState{
    SDL_Window *window;
    SDL_Renderer *renderer;
};

/*struct GUIState{
    SDL_Window *window;
    SDL_Renderer *renderer;
    TTF_Font *font;
    SDL_Texture *background;
    //SDL_Rect input_box;
    //SDL_Rect send_button;
    //SDL_Rect chat_area;
    //SDL_Rect message_area;
};*/


//test-case
struct showData{
    char *users[5];
    int count;
};
/*
static struct showData data = {
    .users = {"AKO", "AMIN", "BCHFV", "N3SHOW", "HAMZA4IK"},
    .count = 5,
};
*/


bool initialize_gui(struct GUIState* app);
void update_gui(struct showData *data);
void gui_cleanup(struct GUIState* app, int exit_status);

#endif