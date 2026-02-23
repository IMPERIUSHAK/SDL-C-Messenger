#include "client.h"

static void handle_keydown(struct GUIState *app, const SDL_Event *event, bool *isRunning){
    if (event->key.keysym.scancode == SDL_SCANCODE_ESCAPE){
        *isRunning = false;
        gui_cleanup(app, EXIT_SUCCESS);
    }
}

void client_events(struct GUIState *app, SDL_Event *event, bool *isRunning) {
    
    while (SDL_PollEvent(event)) {
        switch (event->type)
        {
            case SDL_QUIT:
                *isRunning = false;
                gui_cleanup(app, EXIT_SUCCESS);
                return;
            case SDL_KEYDOWN:
                handle_keydown(app, event, isRunning);
                break;
            default:
                break;
        }
            //update_gui(window);
    }
}

void run_client(struct GUIState *app) {
    bool isRunning = true;
    SDL_Event event;

    while (isRunning) { 
        client_events(app, &event, &isRunning);
    }
}