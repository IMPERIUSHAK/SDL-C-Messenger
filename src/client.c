#include "client.h"

struct showData data = {
    .users = {"AKO", "AMIN", "BCHFV", "N3SHOW", "HAMZA4IK"},
    .count = 5,
};

void handle_keydown(struct GUIState *app, const SDL_Event *event, bool *isRunning){
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
        
    }
}

void run_client(struct GUIState *app) {
    bool isRunning = true;
    SDL_Event event;

    if (update_gui(app, &data)){
        gui_cleanup(app, EXIT_FAILURE);
        return;
    }

    while (isRunning) { 

        client_events(app, &event, &isRunning);
        SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 255);
        SDL_RenderClear(app->renderer);
        
        render_chats(app);

        SDL_SetRenderDrawColor(app->renderer, 124, 125, 80, 55);
        
        SDL_RenderPresent(app->renderer);

    }
}