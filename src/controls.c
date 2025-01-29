#include <SDL.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include "controls.h"
#include "plutils.h"
#include "playlist.h"

#define BIG_BUTTN_DEF 128
#define SMALL_BUTTN_DEF 32
#define SMALL_GAP 15

//!TODO — store WindowSize somewhere to not call the API function each render function 

struct RAW_SOURCES *raw_res = NULL;
struct GUI *gui = NULL;
enum playerEvent events;

struct GUI* getGUIHandler(void) {
    if (gui) {
        return gui;
    }
    else return NULL;
}

Mix_Music* getMusicHandler(void) {
    if (gui) {
        if (!gui->currentlyPlaying) {
            return NULL;
        }
        else return gui->currentlyPlaying;
    }
    else return NULL;
}

void updateCurrentMusic(Mix_Music *music) {
    if (gui) {
      //  printf("Update music \n");
        if (gui->title.title) SDL_DestroyTexture(gui->title.title);
        gui->title.title = NULL;
        gui->currentlyPlaying = music;
        Mix_PlayMusic(gui->currentlyPlaying, 1);
        gui->slider.duration = Mix_MusicDuration(NULL);
        gui->buttons.ctrlAct = PAUSE_DISPL;
        events = NOOP;
        
    }
}

int initGUI(SDL_Renderer *renderer, const char *font_path, const char *atl_path) {

    //printf("Create Alloc");
    gui = (struct GUI*) calloc(1, sizeof(struct GUI));
    if (!gui) {
        perror("Cannot allocate for gui. Program halted \n");
        return -1;
    }
    gui->currentlyPlaying = NULL;

    //printf("CREATED ALLOC");
    
    gui->buttons.play.texPos = (struct SDL_Rect) {0, 0, 150, 128};  // x, y, width, height in the texture atlas
    gui->buttons.pause.texPos = (struct SDL_Rect) {150, 0, 150, 128};
    gui->buttons.prev.texPos = (struct SDL_Rect) {300, 0, 150, 128};
    gui->buttons.stop.texPos = (struct SDL_Rect) {450, 0, 150, 128};
    gui->buttons.next.texPos = (struct SDL_Rect) {600, 0, 150, 128};
    gui->buttons.plst.texPos = (struct SDL_Rect) {300, 128, 150, 128};
    gui->buttons.ctrlAct = PLAY_DISPL;
    gui->slider = (slider_info_t) {0, 0, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
    gui->volume = (slider_info_t) {128, MIX_MAX_VOLUME, {0, 0, 15, SMALL_BUTTN_DEF}, {0, 0, 128, SMALL_BUTTN_DEF}, {0, 0, 0, 0}};
    gui->volume.texturePos = (struct SDL_Rect) {0, 128, 300, 128};
    gui->title = (title_t) {{0, 0, 0, 0}, NULL};

    raw_res = (struct RAW_SOURCES*) calloc(1, sizeof(struct RAW_SOURCES));
    if (!raw_res) {
        perror("Cannot allocate for raw resources (textures, fonts). Program halted \n");
        return -1;
    }

    raw_res->atlas = (SDL_Texture*) loadTexture(renderer, atl_path);
    if (!raw_res->atlas) {
        perror("Cannot load texture atlas.\n");
    }

    raw_res->font = TTF_OpenFont(font_path, 48);
    if (!raw_res->font) {
        printf("Cannot open the font %s", TTF_GetError());
    }

    return 0;
}

int deinitGUI(void) {
    if (raw_res->font) TTF_CloseFont(raw_res->font);
    if (raw_res->atlas) SDL_DestroyTexture(raw_res->atlas);
    if (raw_res) {
        free(raw_res); 
        raw_res = NULL;
    }
    if (gui->title.title) SDL_DestroyTexture(gui->title.title);
    if (gui->currentlyPlaying)  Mix_FreeMusic(gui->currentlyPlaying);
    if (gui) {
        free(gui);
        gui = NULL;
    }

    return 0;   
}


void playPauseAndSwitchButton(void) {
    if (events == PLAYER_STOPPED) {
        gui->buttons.ctrlAct = PLAY_DISPL;
        return;
    }
    if (!gui->currentlyPlaying) {
        //printf("Next_track \n");
        events = NEED_NEXT_TRACK;
        return;
    }

    texControls_t *controls = &(gui->buttons);
    if (controls->ctrlAct == PLAY_DISPL) {
        controls->ctrlAct = PAUSE_DISPL;
        if (Mix_PausedMusic()) {
            Mix_ResumeMusic();
        } else {
            Mix_PlayMusic(gui->currentlyPlaying, 1);
        }
                            
    } else if (controls->ctrlAct == PAUSE_DISPL || controls->ctrlAct == IDLE) {
        controls->ctrlAct = PLAY_DISPL;
        if (Mix_PlayingMusic()) {
            Mix_PauseMusic();
        }
        else {
            Mix_PlayMusic(gui->currentlyPlaying, 1);
        }
        
    }

}

void renderButtons(SDL_Window *window, SDL_Renderer *renderer) {
        int windowWidth, windowHeight;
        SDL_GetWindowSize(window, &windowWidth, &windowHeight);
        

        texControls_t *ctrs = &(gui->buttons);

        int big_button = BIG_BUTTN_DEF;
        int small_button = SMALL_BUTTN_DEF;
        int small_gap = SMALL_GAP;
        
        int y_big = (windowHeight / 2) - (big_button / 2);
        int y_small = (windowHeight / 2) + (big_button / 2) - small_button;

        int x_small = 25 + big_button + small_gap; //+(15+small_button)*m


        //int buttonWidth = 150 / 3;
        //int buttonHeight = 128 / 3;
        //int y = windowHeight - buttonHeight - 9;  // 10 pixels from the bottom

        //SDL_Rect dstRect = {0, y, buttonWidth, buttonHeight};

        ctrs->play.renderPos = (struct SDL_Rect) {25, y_big, big_button, big_button}; 
        ctrs->pause.renderPos = (struct SDL_Rect) {25, y_big, big_button, big_button}; 
        
        switch (ctrs->ctrlAct)
        {
        case PAUSE_DISPL:
            SDL_RenderCopy(renderer, raw_res->atlas, &ctrs->pause.texPos, &ctrs->pause.renderPos);
            break;
        default:
            SDL_RenderCopy(renderer, raw_res->atlas, &ctrs->play.texPos, &ctrs->play.renderPos);
            break;
        }
        
        //SDL_Rect pauseButton = {150, 0, 150, 128};
        //SDL_RenderCopy(renderer, atlas, &pauseButton, &dstRect);

        ctrs->prev.renderPos = (struct SDL_Rect) {x_small, y_small, small_button, small_button};
        SDL_RenderCopy(renderer, raw_res->atlas, &ctrs->prev.texPos, &ctrs->prev.renderPos);
        
        ctrs->stop.renderPos = (struct SDL_Rect) {x_small + (small_gap + small_button) * 1, y_small, small_button, small_button};
        SDL_RenderCopy(renderer, raw_res->atlas, &ctrs->stop.texPos, &ctrs->stop.renderPos);

        ctrs->next.renderPos = (struct SDL_Rect) {x_small + ((small_gap + small_button) * 2), y_small, small_button, small_button};
        SDL_RenderCopy(renderer, raw_res->atlas, &ctrs->next.texPos, &ctrs->next.renderPos);

        ctrs->plst.renderPos = (struct SDL_Rect) {gui->slider.slider.x + gui->slider.slider.w - small_button, y_small, small_button, small_button};
        SDL_RenderCopy(renderer, raw_res->atlas, &ctrs->plst.texPos, &ctrs->plst.renderPos);

        SDL_SetRenderDrawColor(renderer, 183, 45, 45, 100);
        SDL_RenderDrawRect(renderer, &(ctrs->prev.renderPos));
        SDL_RenderDrawRect(renderer, &(ctrs->stop.renderPos));
        SDL_RenderDrawRect(renderer, &(ctrs->next.renderPos));
        SDL_RenderDrawRect(renderer, &(ctrs->play.renderPos)); 
        SDL_RenderDrawRect(renderer, &(ctrs->plst.renderPos)); 
}

void renderTitle(SDL_Window *window, SDL_Renderer *renderer) {
        int windowWidth, windowHeight;
        SDL_GetWindowSize(window, &windowWidth, &windowHeight);

        int x_cord = 25 + BIG_BUTTN_DEF + SMALL_GAP;
        int y_cord = gui->buttons.play.renderPos.y;
        SDL_Rect rect = {x_cord, y_cord, 0, 0};


        if (!gui->title.title) {
            const char* title = GetCurrTrackName(); 
            if (!title) return;
            if (strcmp(title, "") > 0) {
                gui->title.title = getTextureFromWords(renderer, raw_res->font, title);
              
                int tempW, tempH;
                SDL_QueryTexture(gui->title.title, NULL, NULL, &tempW, &tempH);

                //printf("Text W: %i text H: %i \n", tempW, tempH);
                rect.w = tempW;  // 10 pixels padding on each side
                rect.h = tempH; // 5 pixels padding top and botton
                rect.y = (((windowHeight / 2) + rect.y) / 2) - (tempH / 2) - SMALL_GAP / 2;
                ///
                gui->title.renderPos = rect; 
                //printf("Text W: %i text H: %i X: %i Y: %i %i med %i med med %i\n", tempW, tempH, rect.x, rect.y, ctrs->play.renderPos.y, (windowHeight / 2), (((windowHeight / 2) + rect.y)));
            }

        }
        else {
            //SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
            gui->title.renderPos.y = (((windowHeight / 2) + rect.y) / 2) - (gui->title.renderPos.h / 2) - SMALL_GAP / 2;
            SDL_RenderCopy(renderer, gui->title.title, NULL, &gui->title.renderPos);

            
//            printf("Test: render pos %i %i %i %i \n", ctrs->title.renderPos.h, ctrs->title.renderPos.w, ctrs->title.renderPos.x, ctrs->title.renderPos.y);
        }
}

void renderSlider(SDL_Window *window, SDL_Renderer *renderer) {
        int windowWidth, windowHeight;
        SDL_GetWindowSize(window, &windowWidth, &windowHeight);
        slider_info_t* slider = &(gui->slider);
        
        int x_cord = 25 + BIG_BUTTN_DEF + SMALL_GAP;
        int y_cord = (windowHeight / 2) - 12;

        slider->slider = (struct SDL_Rect) {x_cord, y_cord, windowWidth - x_cord - 25, 24};

        //somehow should be placed in init_play or smth
        if (slider->duration <= 0) {
            slider->duration = Mix_MusicDuration(NULL); //NULL — currently playing music;
        }


        if (Mix_GetMusicPosition(NULL) > -1) { 
            slider->position = Mix_GetMusicPosition(NULL);
        }
        
        //if music is not paused —> recalculate position
        if (!Mix_PausedMusic()) {
            int xs_cord = (int) (((slider->position * 100) / slider->duration / 100) * (slider->slider.w - 6) + slider->slider.x);
            slider->slider_bar = (struct SDL_Rect) {xs_cord, y_cord + 5, 5, 10};
           // printf("Position: %lf Duration: %lf Xs_cord: %i Procent: %lf In dic: %lf \n", slider->position, slider->duration, xs_cord, ((slider->position * 100) / slider->duration), (100 / (slider->position * 100) / slider->duration));
        }

    
        SDL_SetRenderDrawColor(renderer, 183, 45, 45, 100);
        SDL_RenderDrawRect(renderer, &(slider->slider)); 
        SDL_RenderDrawRect(renderer, &(slider->slider_bar)); 
}

void renderVolumeSlider(SDL_Window *window, SDL_Renderer *renderer) {
    int windowWidth, windowHeight;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);
    slider_info_t *slider = &(gui->volume);

    slider->slider.y = gui->buttons.prev.renderPos.y;
    slider->slider.x = gui->buttons.plst.renderPos.x - SMALL_GAP - slider->slider.w;
    gui->volume.slider_bar.y = gui->volume.slider.y;
    int x = (int) gui->volume.position;
    gui->volume.slider_bar.x = gui->volume.slider.x + x - 16;
    if (gui->volume.slider_bar.x < gui->volume.slider.x) gui->volume.slider_bar.x = gui->volume.slider.x;
   // slider->slider.w = 80;
   // slider->slider.h = SMALL_BUTTN_DEF;

    //int xs_cord = (int) (((slider->position * 100) / slider->duration / 100) * (slider->slider.w - 16) + slider->slider.x);
    SDL_RenderCopy(renderer, raw_res->atlas, &(slider->texturePos), &(slider->slider));
    SDL_SetRenderDrawColor(renderer, 183, 45, 45, 100);
    //SDL_RenderDrawRect(renderer, &(slider->slider));
    SDL_RenderDrawRect(renderer, &(slider->slider_bar));  

}

void setVolumeSliderPos(int x) {
    
    //gui->volume.slider_bar.x = x - 16;
    //printf("New volume: %i \n", (x - gui->volume.slider.x));
    if (x - gui->volume.slider.x - 15 <= 0) gui->volume.position = 0;
    else gui->volume.position = (Uint8) (x - gui->volume.slider.x);
    Mix_VolumeMusic((int) (gui->volume.position));
    
}

void setSliderPos(int x) {
        if (gui->currentlyPlaying) {
            double proc =  ((1.0 * (x - gui->slider.slider.x)) / gui->slider.slider.w); 
            //(100 * (x - gui->slider.slider.x)) / (double) ((gui->slider.slider.w) + gui->slider.slider.x) / 100; 
            //printf("Procent: %lf X: %i Length: %i Xs: %i \n", proc, x, (gui->slider.slider.w), x - gui->slider.slider.x);
            Mix_SetMusicPosition(gui->slider.duration * proc);
        }
    
    //(slider->slider.w - 6) + slider->slider.x
}

