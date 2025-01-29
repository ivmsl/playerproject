#include <SDL.h>
#include <stdio.h>
//#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "src/controls.h"
#include "src/playlist.h"
#include "src/plutils.h"
#include "src/dirutils.h"
#include "src/main.h"
#include "src/tt_window.h"
#include "src/dirwindow.h"

#define WINDOW_WIDTH 725
#define WINDOW_HEIGHT 200
#define BTTNPATH "res/textures/buttons01.png"
#define MP3PATH "res/test/1.mp3"

enum windowEvents winEvents;

void eventLoop(SDL_Window *window, int* quit) {
    // Event handler
    SDL_Event e;
    //int mouseX, mouseY;
    
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
                if (e.button.windowID == get_playlist_window_id()) continue;
                Sint32 mouseX = e.button.x;
                Sint32 mouseY = e.button.y;
                //SDL_GetMouseState(&mouseX, &mouseY);
                printf("MouseX: %d, MouseY: %d \n", mouseX, mouseY);
                //struct GUI *gui = getGUIHandler();
                if (checkAreaClick(mouseX, mouseY, &(gui->buttons.play.renderPos))) {
                        playPauseAndSwitchButton();
                }
                if (checkAreaClick(mouseX, mouseY, &(gui->buttons.plst.renderPos))) {
                    winEvents = CLICK_PLAYLIST_OPEN; //CHECK IF OPENED
                     //   create_and_return_playlist_window(); //to events????????
                }
                if (checkAreaClick(mouseX, mouseY, &(gui->buttons.next.renderPos))) {
                        events = NEED_NEXT_TRACK;
                }
                if (checkAreaClick(mouseX, mouseY, &(gui->buttons.prev.renderPos))) {
                        events = NEED_PREV_TRACK;
                }
                if (checkAreaClick(mouseX, mouseY, &(gui->slider.slider))) {
                        setSliderPos(mouseX);
                }          
                break;
            case SDL_MOUSEWHEEL: 
                if (e.wheel.windowID == get_playlist_window_id()) {
                    changeXCoordScrolling(e.wheel.y);
                }
                break;
            case SDL_WINDOWEVENT:
                {   
                    if (e.window.event == SDL_WINDOWEVENT_CLOSE) {
                        //printf("Windowevent_close\n");
                        
                        if (e.window.windowID == get_playlist_window_id()) winEvents = CLICK_PLAYLIST_CLOSE;
                        else {
                            destroyPlaylistWindow();
                            *quit = 1;
                        }
                    }
                    
                }
                break;
            
            case SDL_DROPFILE:
            {
                printf("File dropped \n");
                printf("File: %s\n", e.drop.file);
                playlist_append(e.drop.file); 
                SDL_free(e.drop.file);
            }
            break;

            case SDL_DROPCOMPLETE:
            {
                updatePlaylistBlocks();
            }

            default:
                break;
            }
    }

}

void doRender(wr_couple* mainRenderer) {
        SDL_SetRenderDrawColor(mainRenderer->renderer, 217, 217, 217, 255);
        SDL_RenderClear(mainRenderer->renderer);

        //SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        //SDL_RenderFillRect(renderer, &rect);
        

        //from controls.c!!!
        
        renderButtons(mainRenderer->window, mainRenderer->renderer);
        if (events != NEED_NEXT_TRACK && events != CHANGING_TRACK) renderTitle(mainRenderer->window, mainRenderer->renderer);
        renderSlider(mainRenderer->window, mainRenderer->renderer);
        // Update screen
        SDL_RenderPresent(mainRenderer->renderer);

        wr_couple *playlistWindow = GetPlaylistRenderer();
        if (playlistWindow) if (playlistWindow->renderer) {
            SDL_SetRenderDrawColor(playlistWindow->renderer, 217, 217, 217, 255);
            SDL_RenderClear(playlistWindow->renderer);
            render_playlist_if_present();
            SDL_RenderPresent(playlistWindow->renderer);
        }

}

int main(void) {

    struct winManager windowsManager = (struct winManager) {(wr_couple) {NULL, NULL, -1}, NULL, NULL, NULL};
    
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
    windowsManager.mainWindow.window = SDL_CreateWindow("Simple music player",
                              SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              WINDOW_WIDTH, WINDOW_HEIGHT,
                              SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (windowsManager.mainWindow.window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // Create renderer (RENDERER POINTER!!)
    windowsManager.mainWindow.renderer = SDL_CreateRenderer(windowsManager.mainWindow.window, -1, SDL_RENDERER_ACCELERATED);
    if (windowsManager.mainWindow.renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    windowsManager.mainWindow.windowID = SDL_GetWindowID(windowsManager.mainWindow.window);

    printf("\nINIT GUI\n");
    if (initGUI(windowsManager.mainWindow.renderer, FONTPATH, BTTNPATH) != 0) {
        perror("Gui can't be loaded");
        quit = 1;
    }

    //Mix_Music *music = Mix_LoadMUS(MP3PATH);

    struct dir_content *dir_c = open_directory("./res/test");
    printf("Len: %i, Path: %s\n", dir_c->len, dir_c->path);
    printf("Exit code: %i\n", playlist_from_dir(dir_c));
    plist *playlist = get_playlist_handler();
    if (playlist) printf("Name [2]: %s \n", playlist->playlist[2].name);

    registerHaltCallback();

    // Main loop
    while (!quit) {
        
        //Check events
        eventLoop(windowsManager.mainWindow.window, &quit);

        // Clear screen
        doRender(&(windowsManager.mainWindow));



            //switch_states
        switch (events)
        {
        case NEED_NEXT_TRACK:
            events = CHANGING_TRACK;
            //printf("Exit code next track: %i \n", playlist_next());
            playlist_next();

        break;

        case NEED_PREV_TRACK:
            events = CHANGING_TRACK;
            //printf("Exit code next track: %i \n", playlist_next());
            playlist_prev();

        break;
        default:
            break;
    }    

        switch (winEvents)
        {
        case CLICK_PLAYLIST_OPEN:
            {   
                printf("Playlist opened\n");
                if (!windowsManager.playlistWindow) windowsManager.playlistWindow = createAndReturnPlaylistWindow();
                winEvents = PLAYLIST_OPENED;
            }
            break;
        case PLAYLIST_OPENED:
            if (windowsManager.playlistWindow) SDL_RaiseWindow(windowsManager.playlistWindow->window);
            winEvents = WIN_IDLE;
            break;
        case CLICK_PLAYLIST_CLOSE:
            printf("Playlist_Closed\n");
            if (windowsManager.playlistWindow) {
                destroyPlaylistWindow();
                SDL_RaiseWindow(windowsManager.mainWindow.window);
                windowsManager.playlistWindow = NULL;
                winEvents = WIN_IDLE;
            }
            break;
        
        default:
            break;
        }

            SDL_Delay(10);
    }

    // Destroy window
    SDL_DestroyRenderer(windowsManager.mainWindow.renderer);
    SDL_DestroyWindow(windowsManager.mainWindow.window);
    destroyPlaylistWindow();

    //custom free-dir
    free_dir_content(dir_c);
    deinitGUI();

    // Quit SDL subsystems
    TTF_Quit(); 
    SDL_Quit();

    return 0;
}
