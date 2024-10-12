#include <SDL.h>
#include <stdio.h>
//#include <SDL_image.h>
#include <SDL_mixer.h>
#include "src/controls.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define BTTNPATH "res/textures/buttons01.png"
#define MP3PATH "res/test/1.mp3"


SDL_Texture* loadTexture(SDL_Renderer* renderer, const char* path) {
    SDL_Surface* surface = IMG_Load(path);
    if (!surface) {
        printf("Error loading image: %s\n", IMG_GetError());
        return NULL;
    }
    
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        printf("Error creating texture: %s\n", SDL_GetError());
    }
    
    SDL_FreeSurface(surface);
    return texture;
}


void eventLoop(SDL_Window *window, int* quit, texControls_t* controls, Mix_Music* music){
    // Event handler
    SDL_Event e;
    int mouseX, mouseY;
    
    while (SDL_PollEvent(&e) != 0) {            
        // User requests quit
        switch (e.type) {
            case SDL_QUIT:
                *quit = 1;
                break;
            case SDL_KEYDOWN:
                switch (e.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        *quit = 1;
                        break;
                    case SDLK_q:
                        *quit = 1;
                        break;
                    case SDLK_SPACE:
                        if (controls->ctrlAct == PLAY_DISPL) {
                            controls->ctrlAct = PAUSE_DISPL;
                            if (Mix_PausedMusic()) {
                                Mix_ResumeMusic();
                            }
                            else {
                                Mix_PlayMusic(music, 9);
                            }
                            
                        }
                        else if (controls->ctrlAct == PAUSE_DISPL || controls->ctrlAct == IDLE) {
                            controls->ctrlAct = PLAY_DISPL;
                            Mix_PauseMusic();
                        }
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                SDL_GetMouseState(&mouseX, &mouseY);
                printf("MouseX: %d, MouseY: %d \n", mouseX, mouseY);
                printf("block: x: %d, y: %d, x2: %d, y2: %d \n", controls->pause.renderPos.x, 
                        controls->pause.renderPos.y,
                        controls->pause.renderPos.x + controls->pause.renderPos.w, 
                        controls->pause.renderPos.y + controls->pause.renderPos.h);
                if (mouseX >= controls->pause.renderPos.x && 
                    mouseX < controls->pause.renderPos.x + controls->pause.renderPos.w &&
                    mouseY >= controls->pause.renderPos.y && 
                    mouseY < controls->pause.renderPos.y + controls->pause.renderPos.h) {

                        if (controls->ctrlAct == PLAY_DISPL) {
                            controls->ctrlAct = PAUSE_DISPL;
                            if (Mix_PausedMusic()) {
                                Mix_ResumeMusic();
                            }
                            else {
                                Mix_PlayMusic(music, 9);
                            }
                        
                        }
                        else if (controls->ctrlAct == PAUSE_DISPL) {
                            controls->ctrlAct = PLAY_DISPL;
                            Mix_PauseMusic();
                            
                        }
                    }     
                break;     
            default:
                break;
            }
            

        }
}

void doRender(SDL_Window *window, SDL_Renderer *renderer, texControls_t* ctrs) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        int windowWidth, windowHeight;
        SDL_GetWindowSize(window, &windowWidth, &windowHeight);

        SDL_Rect rect = {
            windowWidth / 4,
            windowHeight / 4,
            windowWidth / 2,
            windowHeight / 2
        };

        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderFillRect(renderer, &rect);
        

        //from controls.c!!!
        renderButtons(window, renderer, ctrs);

        // Update screen
        SDL_RenderPresent(renderer);

}

int main(int argc, char* args[]) {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    if (Mix_OpenAudio(48000, AUDIO_S16SYS, 2, 1024) < -1) {
        printf("SDL MIX can't open audio! SDL_Error: %s\n", Mix_GetError());
    }

    if (Mix_Init(MIX_INIT_MP3) < 0) {
        printf("SDL MIX INIT could not initialize! SDL_Error: %s\n", Mix_GetError());
    }

  
    Mix_Music *music = Mix_LoadMUS(MP3PATH);

    // Create window
    window = SDL_CreateWindow("SDL2 Window Example",
                              SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              WINDOW_WIDTH, WINDOW_HEIGHT,
                              SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // Create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    //Button texture load
    texControls_t textureControls;
        textureControls.atlas = (SDL_Texture*) loadTexture(renderer, BTTNPATH);
        textureControls.play.texPos = (struct SDL_Rect) {0, 0, 150, 128};  // x, y, width, height in the texture atlas
        textureControls.pause.texPos = (struct SDL_Rect) {150, 0, 150, 128};
        textureControls.prev.texPos = (struct SDL_Rect) {300, 0, 150, 128};
        textureControls.stop.texPos = (struct SDL_Rect) {450, 0, 150, 128};
        textureControls.next.texPos = (struct SDL_Rect) {600, 0, 150, 128};
        textureControls.ctrlAct = PLAY_DISPL;

    // Main loop flag
    int quit = 0;
    // Main loop
    while (!quit) {
        
        //Check events
        eventLoop(window, &quit, &textureControls, music);

        // Clear screen
        doRender(window, renderer, &textureControls);
        SDL_Delay(10);
    }

    // Destroy window
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_DestroyTexture(textureControls.atlas);
    Mix_FreeMusic(music);

    // Quit SDL subsystems
    SDL_Quit();

    return 0;
}
