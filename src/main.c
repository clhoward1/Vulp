#define SDL_MAIN_USE_CALLBACKS
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdio.h>
#include "buffer.h"
#include "cursor.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 960

#define FONT_SIZE 20
#define BLINK_INTERVAL_MS 667

const Uint32 BLACK = 0x262626; // hex code for a lighter black #262626
const Uint32 WHITE = 0xDCDCDC; // a light grey to stand in for white #DCDCDC
 
SDL_Window *window;
SDL_Renderer *renderer;

SDL_Surface *textSurface;
SDL_Surface *windowSurface;
SDL_Texture *windowTexture;

TTF_Font *font;
TTF_TextEngine *textEngine;

TTF_Text *displayText;
char *renderText;

SDL_FRect cursorRect = {12, 4, 12, 20};
bool cursorOn = false;

gapBuffer *buffer;
cursor *cur;

Uint64 currentTime, previousTime = 0;

FILE *textFile;

//initial render
void initialRender() {

    SDL_RenderClear(renderer);

    SDL_FillSurfaceRect(windowSurface, NULL, BLACK);

    windowTexture = SDL_CreateTextureFromSurface(renderer, windowSurface);

    SDL_RenderTexture(renderer, windowTexture, NULL, NULL);

    SDL_RenderPresent(renderer);
}


void renderCursor() {

    if (cursorOn) {

        //int textWidth = 0, textHeight = 0;

        //TTF_GetTextSize(displayText, &textWidth, &textHeight);

        /*
        if (buffer->data[buffer->gapLeft - 1] == '\n') {
            cursorRect.x = (1 * 12) + 12;
        }
        */
        
        cursorRect.x = (buffer->lineOffset * 12) + 12;
        cursorRect.y = (buffer->lineCount * 22) + 4;

        SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255);
        SDL_RenderFillRect(renderer, &cursorRect);
    }
}

void cursorMoveRender() {

    SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer, windowTexture, NULL, NULL);
    cursorOn = true;
    renderCursor();
    SDL_RenderPresent(renderer);
}


bool checkTimeInterval() {

    currentTime = SDL_GetTicks();

    if (currentTime > previousTime + BLINK_INTERVAL_MS) {
        cursorOn = !cursorOn;
        previousTime = currentTime;  
        return true;
    } else {
        return false;
    }
}


void initializeBuffer() {

    buffer = initBuffer();
    cur = initCursor();

}


void renderKey() {

    windowSurface = SDL_GetWindowSurface(window);

    size_t preGap = buffer->gapLeft;
    size_t postGap = buffer->totalSize - buffer->gapRight - 1;

    renderText = malloc(preGap + postGap + 1);

    memcpy(renderText, buffer->data, preGap);
    memcpy(renderText + preGap, buffer->data + buffer->gapRight + 1, postGap);
    renderText[preGap + postGap] = '\0';

    SDL_RenderClear(renderer);

    SDL_FillSurfaceRect(windowSurface, NULL, BLACK);

    displayText = TTF_CreateText(textEngine, font, renderText, buffer->totalSize);

    TTF_SetTextColor(displayText, 220, 220, 220, 255);
    TTF_DrawSurfaceText(displayText, 12, 4, windowSurface);

    windowTexture = SDL_CreateTextureFromSurface(renderer, windowSurface);
    SDL_RenderTexture(renderer, windowTexture, NULL, NULL);

    /*
    printf("\n");
    for (int i = 0; i < strlen(buffer->data); i++) {
        printf("%c", buffer->data[i]);
    }
    */
    
    renderCursor();

    SDL_RenderPresent(renderer);
}


void addToBuffer(const char *input) {

    insert((char*)input, buffer->gapLeft, buffer);

    renderKey();
}

void removeFromBuffer() {

    backspace(buffer);
    renderKey();
}

bool openTextFile() {

    textFile = fopen("assets/text.txt", "r");

    if (textFile == NULL) {
        printf("Error while opening text file");
        return false;
    }

    return true;
}

void closeTextFile() {

    fclose(textFile);
}

void displayTextFile() {
    char c;

    while ((c = fgetc(textFile)) != EOF) {
        
        addToBuffer((const char*)&c);
    }

    renderKey();
}

void writeTextFile() {

    textFile = fopen("assets/text.txt", "w");

    if (textFile == NULL) {
        printf("Error while opening text file");
        return;
    }

    fputs(renderText, textFile);

    fclose(textFile);
}


// sets the application icon
SDL_AppResult icon() {

    SDL_Surface *icon = IMG_Load("assets/icon.jpg");

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
    icon = NULL;

    return SDL_APP_CONTINUE;
}

// loads font
SDL_AppResult text() {
    font = TTF_OpenFont("assets/FreeMonoBold.ttf", FONT_SIZE);

    if(!font) {

        SDL_Log("Error loading Font: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    textEngine = TTF_CreateSurfaceTextEngine();

    if(!textEngine) {

        SDL_Log("Error creating Text Engine: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    windowSurface = SDL_GetWindowSurface(window);

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

    initializeBuffer();

    icon();
    text();
    initialRender();

    SDL_StartTextInput(window);

    openTextFile();
    displayTextFile();

    //printf("\nBuffer totalSize: %d", buffer->totalSize);
    
    return SDL_APP_CONTINUE;
}

//Waits for events to be called
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {


    switch (event->type) {
        case SDL_EVENT_QUIT:
            return SDL_APP_SUCCESS;
        case SDL_EVENT_KEY_DOWN:
            switch (event->key.key) {
                case SDLK_BACKSPACE: 
                    removeFromBuffer();
                    break;
                case SDLK_TAB:
                    for (int i = 0; i < 4; i++) {
                        addToBuffer(" ");
                    }
                    break;
                case SDLK_RETURN:
                    addToBuffer("\n");
                    break;
                case SDLK_LEFT:
                    moveCursor((buffer->gapLeft - 1), buffer);
                    cursorMoveRender();
                    renderKey();
                    break;
                case SDLK_RIGHT:
                    moveCursor((buffer->gapLeft + 1), buffer);
                    cursorMoveRender();
                    renderKey();
                    break;
            }
            break;
        case SDL_EVENT_TEXT_INPUT:
            addToBuffer(event->text.text);
            break;
        case SDL_EVENT_WINDOW_RESIZED:
            renderKey();
            break;
    }


    return SDL_APP_CONTINUE;
}


SDL_AppResult SDL_AppIterate(void *appstate) {

    if(checkTimeInterval()) {
        SDL_RenderClear(renderer);
        SDL_RenderTexture(renderer, windowTexture, NULL, NULL);
        renderCursor();
        SDL_RenderPresent(renderer);
    }
    
    return SDL_APP_CONTINUE;
}

//Frees memory upon app quit
void SDL_AppQuit(void *appstate, SDL_AppResult result) {

    closeTextFile();
    writeTextFile();
    SDL_StopTextInput(window);

    free(buffer);
    buffer = NULL;
    TTF_DestroyText(displayText);
    displayText = NULL;
    TTF_DestroyRendererTextEngine(textEngine);
    textEngine = NULL;
    TTF_CloseFont(font);
    font = NULL;
    SDL_DestroyTexture(windowTexture);
    windowTexture = NULL;
    SDL_DestroySurface(windowSurface);
    windowSurface = NULL;
    SDL_DestroySurface(textSurface);
    textSurface = NULL;
    SDL_DestroyRenderer(renderer);
    renderer = NULL;
    SDL_DestroyWindow(window);
    window = NULL;
    
    TTF_Quit();

    SDL_QuitSubSystem(SDL_INIT_VIDEO);
    SDL_Quit();
}