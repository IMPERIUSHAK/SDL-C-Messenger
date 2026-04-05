#include <SDL2/SDL.h>
#include "client.h"


int main(void)
{
    struct GUIState app = {0};
    pthread_cond_init(&app.json_queue.cond, NULL);

    if (init_messages(&app.messages)){
        gui_cleanup(&app, EXIT_FAILURE);
        messages_free(&app.messages);
        return EXIT_FAILURE;
    }

    if (initialize_gui(&app)) {
        gui_cleanup(&app, EXIT_FAILURE);
        return EXIT_FAILURE;
    }

    run_client(&app);
    gui_cleanup(&app, EXIT_SUCCESS);
    return EXIT_SUCCESS;
}

