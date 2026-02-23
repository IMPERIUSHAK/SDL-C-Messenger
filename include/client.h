#include "gui.h"

static void handle_keydown(struct GUIState *app, const SDL_Event *event, bool *isRunning);
void client_events(struct GUIState *app, SDL_Event *event, bool *isRunning);
void run_client(struct GUIState *app);