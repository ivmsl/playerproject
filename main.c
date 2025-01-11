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
                        playPauseAndSwitchButton(controls, music);       
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

                        playPauseAndSwitchButton(controls, music);
                    }     
                break;     
            default:
                break;
            }
            

    }

    //switch_states
    

}

void doRender(SDL_Window *window, SDL_Renderer *renderer, texControls_t* ctrs) {
        SDL_SetRenderDrawColor(renderer, 217, 217, 217, 255);
        SDL_RenderClear(renderer);

        //SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        //SDL_RenderFillRect(renderer, &rect);
        

        //from controls.c!!!
        
        renderButtons(window, renderer, ctrs);
        renderTitle(window, renderer, ctrs);
        renderSlider(window, renderer, ctrs);
        // Update screen
        SDL_RenderPresent(renderer);

}

int main(int argc, char* args[]) {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    TTF_Font *font;

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

    font = TTF_OpenFont(FONTPATH, 48);
    if (!font) {
        printf("Cannot open the font %s", TTF_GetError());
    }

  
    Mix_Music *music = Mix_LoadMUS(MP3PATH);


    struct dir_content *dir_c = open_directory("./res/test");
    printf("Current dir: %s, Len: %i, Path: %s\n", get_curr_dir(), dir_c->len, dir_c->path);
    printf("Exit code: %i\n", playlist_from_dir(dir_c));
    plist *playlist = get_playlist_handler();
    if (playlist) printf("Name [2]: %s \n", playlist->playlist[2].name);

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

    //Button texture load
    texControls_t textureControls;
        textureControls.atlas = (SDL_Texture*) loadTexture(renderer, BTTNPATH);
        textureControls.play.texPos = (struct SDL_Rect) {0, 0, 150, 128};  // x, y, width, height in the texture atlas
        textureControls.pause.texPos = (struct SDL_Rect) {150, 0, 150, 128};
        textureControls.prev.texPos = (struct SDL_Rect) {300, 0, 150, 128};
        textureControls.stop.texPos = (struct SDL_Rect) {450, 0, 150, 128};
        textureControls.next.texPos = (struct SDL_Rect) {600, 0, 150, 128};
        textureControls.ctrlAct = PLAY_DISPL;
        textureControls.title = (title_t) {NULL, NULL};
        textureControls.font = font;
        textureControls.slider = (slider_info_t) {0, 0, NULL, NULL};
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
    SDL_DestroyTexture(textureControls.title.title);

    Mix_FreeMusic(music);
    TTF_CloseFont(font);
    
    //custom free-dir
    free_dir_content(dir_c);
    

    // Quit SDL subsystems
    TTF_Quit(); 
    SDL_Quit();

    return 0;
}
