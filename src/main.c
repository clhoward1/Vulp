#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

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


void update() {


}

void render() {

    SDL_RenderClear(renderer);

    //SDL_SetRenderDrawColor(renderer, 122, 0, 0, 255); //rgba

    SDL_FillSurfaceRect(windowSurface, NULL, maroon);

    SDL_BlitSurface(texty, NULL, windowSurface, NULL);

    hello = SDL_CreateTextureFromSurface(renderer, windowSurface);

    SDL_RenderTexture(renderer, hello, NULL, NULL);

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
    font = TTF_OpenFont("assets/FreeSansBold.ttf", 20);

    if(!font) {

        SDL_Log("Error loading Font: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    textEngine = TTF_CreateRendererTextEngine(renderer);

    if(!textEngine) {

        SDL_Log("Error creating Text Engine: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_Color color = {0, 255, 0, 255};

    windowSurface = SDL_GetWindowSurface(window);

    texty = TTF_RenderText_Blended(font, "Hello, World", 12, color);


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
    
    return SDL_APP_CONTINUE;
}

//Waits for events to be called
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {

    if (event->type == SDL_EVENT_QUIT) {

        return SDL_APP_SUCCESS;
    }

    return SDL_APP_CONTINUE;
}

//Iterates logic every frame
SDL_AppResult SDL_AppIterate(void *appstate) {
    
    render();
    

    return SDL_APP_CONTINUE;
}

//Frees memory upon app quit
void SDL_AppQuit(void *appstate, SDL_AppResult result) {

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