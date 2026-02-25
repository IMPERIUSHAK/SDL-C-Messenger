#include <SDL2/SDL.h>
#include "client.h"
//#include "gui.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

int main(void)
{
    struct GUIState app = {
        .window = NULL,
        .renderer = NULL,

        .chats = {
            .items = NULL,
            .count = 0,
            .scroll_offset = 0,
        },

        .color_state = NULL,
        .text_font = NULL,
    };

    if (initialize_gui(&app)) {
        gui_cleanup(&app, EXIT_FAILURE);
        return EXIT_FAILURE;
    }

    run_client(&app);
    gui_cleanup(&app, EXIT_SUCCESS);
    return EXIT_SUCCESS;
}

