#define SDL_MAIN_USE_CALLBACKS
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "buffer.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 960
 
SDL_Window *window;
SDL_Renderer *renderer;
TTF_Font *font;
TTF_TextEngine *textEngine;
SDL_Surface *texty;
SDL_Surface *windowSurface;
SDL_Texture *hello;
SDL_FRect rect = {0.0f, 0.0f, 120.0f, 26.0f};
Uint32 maroon = 0x7A0000;
TTF_Text *textino;
const int fontSize = 20;
bool textInputComplete = false;



//Gets input from keyboard
/*
void getInput() {

    SDL_Event e;

    textInputComplete = false;

    SDL_StartTextInput(window);


    while (!textInputComplete) {
        while (SDL_PollEvent(&e)) {

            switch (e.type) {
                case SDL_EVENT_TEXT_INPUT:
                    SDL_Log("Key Pressed");
                    renderKey(e.text.text);
                    textInputComplete = true;
                    break;
                default:
                    SDL_Log("Unhandled Event");
                    break;
            }
        }
    }
    

    SDL_StopTextInput(window);
}
*/

//initial render
void render() {

    SDL_RenderClear(renderer);

    //SDL_SetRenderDrawColor(renderer, 122, 0, 0, 255); //rgba

    SDL_FillSurfaceRect(windowSurface, NULL, maroon);

    SDL_BlitSurface(texty, NULL, windowSurface, NULL);

    hello = SDL_CreateTextureFromSurface(renderer, windowSurface);

    SDL_RenderTexture(renderer, hello, NULL, NULL);

    TTF_DrawRendererText(textino, 100, 100);

    SDL_RenderPresent(renderer);

}

void renderKey(const char* inputKey) {

    SDL_RenderClear(renderer);

    SDL_FillSurfaceRect(windowSurface, NULL, maroon);
    SDL_BlitSurface(texty, NULL, windowSurface, NULL);

    hello = SDL_CreateTextureFromSurface(renderer, windowSurface);
    SDL_RenderTexture(renderer, hello, NULL, NULL);

    textino = TTF_CreateText(textEngine, font, inputKey, strlen(inputKey));
    TTF_SetTextColor(textino, 0, 0, 255, 255);
    TTF_DrawRendererText(textino, 200, 200);

    SDL_RenderPresent(renderer);

}


// sets the window icon on the top left
SDL_AppResult icon() {

    SDL_Surface *icon = IMG_Load("assets/server-icon.png");

    if(!icon) {

        SDL_Log("Error loading Surface: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if(!SDL_SetWindowIcon(window, icon)) {

        SDL_Log("Error setting Window Icon: %s", SDL_GetError());
        SDL_DestroySurface(icon);
        return SDL_APP_FAILURE;
    }

    SDL_DestroySurface(icon);

    return SDL_APP_CONTINUE;
}

// loads font
SDL_AppResult text() {
    font = TTF_OpenFont("assets/FreeSansBold.ttf", fontSize);

    if(!font) {

        SDL_Log("Error loading Font: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    textEngine = TTF_CreateRendererTextEngine(renderer);

    if(!textEngine) {

        SDL_Log("Error creating Text Engine: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_Color green = {0, 255, 0, 255};

    windowSurface = SDL_GetWindowSurface(window);

    textino = TTF_CreateText(textEngine, font, "haiiii", 6); 

    TTF_SetTextColor(textino, 0, 0, 255, 255);

    texty = TTF_RenderText_Blended(font, "Hello, World", 12, green);


    hello = SDL_CreateTextureFromSurface(renderer, texty);

    SDL_RenderTexture(renderer, hello, NULL, &rect);

    return SDL_APP_CONTINUE;
}

//Initializes SDL Window
SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv) {

    if (!SDL_Init(SDL_INIT_VIDEO)) {

        SDL_Log("Error initializing SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!TTF_Init()) {

        SDL_Log("Error initializing SDL3_ttf: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    window = SDL_CreateWindow("Vulp", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);

    if (!window) {

        SDL_Log("Error initializing window: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    renderer = SDL_CreateRenderer(window, NULL);

    if (!renderer) {

        SDL_Log("Error initializing renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    icon();
    text();
    render();

    SDL_StartTextInput(window);
    
    return SDL_APP_CONTINUE;
}

//Waits for events to be called
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {


    switch (event->type) {
        case SDL_EVENT_QUIT:
            return SDL_APP_SUCCESS;
        /*case SDL_EVENT_KEY_DOWN:
            SDL_Keycode keycode = SDL_GetKeyFromScancode(event->key.scancode, event->key.mod, false);
            const char* key = SDL_GetKeyName(keycode);
            
            SDL_Log(key);
            renderKey(key);
            break;
            */
        case SDL_EVENT_TEXT_INPUT:
            SDL_Log(event->text.text);
            renderKey(event->text.text);
            textInputComplete = true;
            break;
    }


    return SDL_APP_CONTINUE;
}

//Iterates logic every frame
SDL_AppResult SDL_AppIterate(void *appstate) {
    
    //render();
    

    return SDL_APP_CONTINUE;
}

//Frees memory upon app quit
void SDL_AppQuit(void *appstate, SDL_AppResult result) {

    SDL_StopTextInput(window);

    SDL_DestroyWindow(window);
    window = NULL;

    SDL_DestroyRenderer(renderer);
    renderer = NULL;

    TTF_CloseFont(font);
    font = NULL;

    TTF_Quit();

    SDL_QuitSubSystem(SDL_INIT_VIDEO);
    SDL_Quit();
}