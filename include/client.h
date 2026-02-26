#include "gui.h"

//extern struct showData data;
void client_events(struct GUIState *app, SDL_Event *event, bool *isRunning);
void handle_keydown(struct GUIState *app, const SDL_Event *event, bool *isRunning);
void run_client(struct GUIState *app);