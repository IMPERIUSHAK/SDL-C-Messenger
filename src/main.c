#include <SDL2/SDL.h>
#include "client.h"
//#include "gui.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
int main() {

    struct GUIState app;

    if (initialize_gui(&app)){
        gui_cleanup(&app, EXIT_FAILURE);
    }

    run_client(app.window);

    SDL_RenderClear(app.renderer);
    SDL_RenderPresent(app.renderer);

    gui_cleanup(&app, EXIT_SUCCESS);
    
    return 0;
}