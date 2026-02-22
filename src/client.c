#include "client.h"
#include "gui.h"


void run_client(SDL_Window *window) {
    int running = 1;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
            
            handle_input(event);
            
            update_gui(window);
        }
    }
}