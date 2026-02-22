#include "client.h"

void run_client(SDL_Window *window) {
    int running = 1;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            switch (event.type)
            {
                case SDL_QUIT:
                    running = 0;
                    break;
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_ESCAPE){    
                        running = 0;
                        break;
                    }
                default:
                    break;
            }
            //update_gui(window);
        }
    }
}