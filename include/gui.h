#ifndef GUI_H
#define GUI_H
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include "messages.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define TEXT_FONT 23
#define WINDOW_TITLE "HaNaKo"
#define BASE_COLOR {255, 255, 255, 255}
#define HOVER_COLOR {0, 125, 125, 124}

struct ChatItem{
    struct Message* text;
    SDL_Texture *texture;
    SDL_Rect rect;
};

struct ChatList{
    struct ChatItem* items;
    size_t count;
    int scroll_offset;
};

struct GUIState{
    SDL_Window *window;
    SDL_Renderer *renderer;
    struct ChatList chats;
    struct MessageList messages;
    SDL_Rect input_rect;
    char userinput[2048];
    SDL_Color *color_state;
    TTF_Font *text_font;
};

struct TextField{
    SDL_Rect rect;
    SDL_Texture *texture;
};
/*
struct GUIState{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Rect *chats_rect;
    SDL_Texture **text_image;
    SDL_Color *color_state;
    TTF_Font *text_font;
    uint32_t chats_count;
    const SDL_Rect chats_tab={
        .x = 0,
        .y = 0,
        .w = 100,
        .h = 300
   .}
*/

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
/*struct showData{
    char *info[1000];
    int count;
};*/

bool initialize_gui(struct GUIState* app);
bool update_gui(struct GUIState* app);

void render_chats(struct GUIState* app);
void gui_cleanup(struct GUIState* app, int exit_status);
void mouse_hover(struct GUIState* app);

#endif