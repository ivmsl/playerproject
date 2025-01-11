#include <SDL.h>
#include <stdio.h>
//#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "src/controls.h"
#include "src/playlist.h"
#include "src/plutils.h"
#include "src/dirutils.h"

#define WINDOW_WIDTH 725
#define WINDOW_HEIGHT 200
#define BTTNPATH "res/textures/buttons01.png"
#define MP3PATH "res/test/1.mp3"
#define FONTPATH "res/fonts/Roboto-Medium.ttf"

void eventLoop(SDL_Window *window, int* quit) {
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
                        playPauseAndSwitchButton();       
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                SDL_GetMouseState(&mouseX, &mouseY);
                printf("MouseX: %d, MouseY: %d \n", mouseX, mouseY);
                struct GUI *gui = getGUIHandler();
                if (checkAreaClick(mouseX, mouseY, &(gui->buttons.play.renderPos))) {
                        playPauseAndSwitchButton();
                }
                if (checkAreaClick(mouseX, mouseY, &(gui->buttons.next.renderPos))) {
                        events = NEED_NEXT_TRACK;
                }     
                break;     
            default:
                break;
            }
    }

}

void doRender(SDL_Window *window, SDL_Renderer *renderer) {
        SDL_SetRenderDrawColor(renderer, 217, 217, 217, 255);
        SDL_RenderClear(renderer);

        //SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        //SDL_RenderFillRect(renderer, &rect);
        

        //from controls.c!!!
        
        renderButtons(window, renderer);
        if (events != NEED_NEXT_TRACK && events != CHANGING_TRACK) renderTitle(window, renderer);
        renderSlider(window, renderer);
        // Update screen
        SDL_RenderPresent(renderer);

}

int main(void) {

    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    // Main loop flag  
    int quit = 0;
    

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

    if (TTF_Init() < 0) {
        printf("TTF init error: %s\n", TTF_GetError());
    }

    // Create window (WINDOW POINTER!!)
    window = SDL_CreateWindow("Simple music player",
                              SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              WINDOW_WIDTH, WINDOW_HEIGHT,
                              SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // Create renderer (RENDERER POINTER!!)
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    printf("\nINIT GUI\n");
    if (initGUI(renderer, FONTPATH, BTTNPATH) != 0) {
        perror("Gui can't be loaded");
        quit = 1;
    }

    //Mix_Music *music = Mix_LoadMUS(MP3PATH);

    struct dir_content *dir_c = open_directory("./res/test");
    printf("Current dir: %s, Len: %i, Path: %s\n", get_curr_dir(), dir_c->len, dir_c->path);
    printf("Exit code: %i\n", playlist_from_dir(dir_c));
    plist *playlist = get_playlist_handler();
    if (playlist) printf("Name [2]: %s \n", playlist->playlist[2].name);


    // Main loop
    while (!quit) {
        
        //Check events
        eventLoop(window, &quit);

        // Clear screen
        doRender(window, renderer);
            //switch_states
        switch (events)
        {
        case NEED_NEXT_TRACK:
            events = CHANGING_TRACK;
            //printf("Exit code next track: %i \n", playlist_next());
            playlist_next();

        break;
    
        default:
            break;
    }    

        SDL_Delay(10);
    }

    // Destroy window
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    
    //custom free-dir
    free_dir_content(dir_c);
    deinitGUI();

    // Quit SDL subsystems
    TTF_Quit(); 
    SDL_Quit();

    return 0;
}
