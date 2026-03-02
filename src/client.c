#include "client.h"

void handle_keydown(struct GUIState *app, const SDL_Event *event, bool *isRunning){
    
    if (event->key.keysym.scancode == SDL_SCANCODE_ESCAPE){
        *isRunning = false;
        gui_cleanup(app, EXIT_SUCCESS);
    }

}

void handle_wheele_up(struct GUIState* app){

    int key = app->chats.scroll_offset;

    if (key >= app->chats.count) return;

    app->chats.scroll_offset++;
    update_gui(app);
}

void handle_wheele_down(struct GUIState* app){

    int key = app->chats.scroll_offset;

    if (key == 0) return;

    app->chats.scroll_offset--;
    update_gui(app);

}

void handle_mousewheel(struct GUIState *app, SDL_Event *event){

    if(event->wheel.y > 0){
        handle_wheele_up(app);
    }else{
        handle_wheele_down(app);
    }

}

void handle_mousebutton(struct GUIState *app, SDL_Event *event){

    if (event->button.button == SDL_BUTTON_LEFT){
        if (isInputActive(app)){
            printf("Left mouse button clicked\n");
            SDL_StartTextInput();
        }else{
            SDL_StopTextInput();
        }
    }
   
}

void handle_text_input(struct GUIState *app, SDL_Event *event){
    
    if(!app->userinput.isactive) return;
    strcat(app->userinput.userinput, event->text.text);
    
    if (update_text_input(app)){
        gui_cleanup(app, EXIT_FAILURE);
    }

}

void client_events(struct GUIState *app, SDL_Event *event, bool *isRunning) {
    
    //SDL_StartTextInput();

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
            case SDL_MOUSEWHEEL:
                handle_mousewheel(app, event);
                break;
            case SDL_TEXTINPUT:
                //strcat(app->userinput.userinput, event->text.text);
                //update_text_input(app);
                handle_text_input(app, event);
                break;
            case SDL_MOUSEBUTTONDOWN:
                // if (event->button.button == SDL_BUTTO)N_LEFT)
                //     printf("Mouse left clicked");
                handle_mousebutton(app, event);
                break;
            default:
                break;
        }
        
    }
}

void run_client(struct GUIState *app) {
    bool isRunning = true;
    SDL_Event event;

    if (update_gui(app)){
        gui_cleanup(app, EXIT_FAILURE);
        fprintf(stderr, "Error initalizeing `update_gui`\n");
        return;
    }


    while (isRunning) { 

        client_events(app, &event, &isRunning);
        SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 255);
        SDL_RenderClear(app->renderer);
        
        render_chats(app);

        mouse_hover(app);
        
        render_text_input(app);
        SDL_RenderPresent(app->renderer);

    }
}