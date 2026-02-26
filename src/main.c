#include <SDL2/SDL.h>
#include "client.h"
//#include "gui.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

int main(void)
{
    struct GUIState app = {0};

    if (init_messages(&app.messages)){
        gui_cleanup(&app, EXIT_FAILURE);
        messages_free(&app.messages);
        return EXIT_FAILURE;
    }

    for(int i = 0; i < app.messages.capacity; i++) {
        printf("%s/n", app.messages.items[i].text);
    }
    
    if (initialize_gui(&app)) {
        gui_cleanup(&app, EXIT_FAILURE);
        return EXIT_FAILURE;
    }

    run_client(&app);
    gui_cleanup(&app, EXIT_SUCCESS);
    return EXIT_SUCCESS;
}

