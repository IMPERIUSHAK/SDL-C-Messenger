#include <SDL2/SDL.h>
#include "client.h"
//#include "gui.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
int main() {

    struct GUIState app = {

        .window = NULL,
        .renderer = NULL,
        .chats_rect = NULL,
        .text_image = NULL,
        .text_font = NULL,
        .chats_count = 0

    };
    
    if (initialize_gui(&app)){
        gui_cleanup(&app, EXIT_FAILURE);
    }
    
    app.text_font = TTF_OpenFont("fonts/freesansbold.ttf", 20);

    run_client(&app);
    gui_cleanup(&app, EXIT_SUCCESS);
    
    return 0;
}