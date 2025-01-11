#include <SDL.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include "controls.h"
#include "plutils.h"

#define BIG_BUTTN_DEF 128
#define SMALL_BUTTN_DEF 32
#define SMALL_GAP 15

//!TODO — store WindowSize somewhere to not call the API function each render function 


void playPauseAndSwitchButton(texControls_t* controls, Mix_Music* music) {
    if (controls->ctrlAct == PLAY_DISPL) {
        controls->ctrlAct = PAUSE_DISPL;
        if (Mix_PausedMusic()) {
            Mix_ResumeMusic();
        } else {
            Mix_PlayMusic(music, 1);
        }
                            
    } else if (controls->ctrlAct == PAUSE_DISPL || controls->ctrlAct == IDLE) {
        controls->ctrlAct = PLAY_DISPL;
        Mix_PauseMusic();
    }

}

void renderButtons(SDL_Window *window, SDL_Renderer *renderer, texControls_t* ctrs) {
        int windowWidth, windowHeight;
        SDL_GetWindowSize(window, &windowWidth, &windowHeight);
        

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
            SDL_RenderCopy(renderer, ctrs->atlas, &ctrs->pause.texPos, &ctrs->pause.renderPos);
            break;
        default:
            SDL_RenderCopy(renderer, ctrs->atlas, &ctrs->play.texPos, &ctrs->play.renderPos);
            break;
        }
        
        //SDL_Rect pauseButton = {150, 0, 150, 128};
        //SDL_RenderCopy(renderer, atlas, &pauseButton, &dstRect);

        ctrs->prev.renderPos = (struct SDL_Rect) {x_small, y_small, small_button, small_button};
        SDL_RenderCopy(renderer, ctrs->atlas, &ctrs->prev.texPos, &ctrs->prev.renderPos);
        
        ctrs->stop.renderPos = (struct SDL_Rect) {x_small + (small_gap + small_button) * 1, y_small, small_button, small_button};
        SDL_RenderCopy(renderer, ctrs->atlas, &ctrs->stop.texPos, &ctrs->stop.renderPos);

        ctrs->next.renderPos = (struct SDL_Rect) {x_small + ((small_gap + small_button) * 2), y_small, small_button, small_button};
        SDL_RenderCopy(renderer, ctrs->atlas, &ctrs->next.texPos, &ctrs->next.renderPos);

        SDL_SetRenderDrawColor(renderer, 183, 45, 45, 100);
        SDL_RenderDrawRect(renderer, &(ctrs->play.renderPos)); 
}

void renderTitle(SDL_Window *window, SDL_Renderer *renderer, texControls_t* ctrs) {
        int windowWidth, windowHeight;
        SDL_GetWindowSize(window, &windowWidth, &windowHeight);

        int x_cord = 25 + BIG_BUTTN_DEF + SMALL_GAP;
        int y_cord = ctrs->play.renderPos.y;
        SDL_Rect rect = {x_cord, y_cord, 0, 0};


        if (!ctrs->title.title) {
            const char* title = Mix_GetMusicTitle(NULL);
            if (strcmp(title, "") > 0) {
                ctrs->title.title = getTextureFromWords(renderer, ctrs->font, title);
              
                int tempW, tempH;
                SDL_QueryTexture(ctrs->title.title, NULL, NULL, &tempW, &tempH);

                //printf("Text W: %i text H: %i \n", tempW, tempH);
                rect.w = tempW;  // 10 pixels padding on each side
                rect.h = tempH; // 5 pixels padding top and botton
                rect.y = (((windowHeight / 2) + rect.y) / 2) - (tempH / 2) - SMALL_GAP / 2;
                ///
                ctrs->title.renderPos = rect; 
                //printf("Text W: %i text H: %i X: %i Y: %i %i med %i med med %i\n", tempW, tempH, rect.x, rect.y, ctrs->play.renderPos.y, (windowHeight / 2), (((windowHeight / 2) + rect.y)));
            }

        }
        else {
            //SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
            ctrs->title.renderPos.y = (((windowHeight / 2) + rect.y) / 2) - (ctrs->title.renderPos.h / 2) - SMALL_GAP / 2;
            SDL_RenderCopy(renderer, ctrs->title.title, NULL, &ctrs->title.renderPos);

            
//            printf("Test: render pos %i %i %i %i \n", ctrs->title.renderPos.h, ctrs->title.renderPos.w, ctrs->title.renderPos.x, ctrs->title.renderPos.y);
        }
}

void renderSlider(SDL_Window *window, SDL_Renderer *renderer, texControls_t* ctrs) {
        int windowWidth, windowHeight;
        SDL_GetWindowSize(window, &windowWidth, &windowHeight);
        slider_info_t* slider = &(ctrs->slider);
        
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

