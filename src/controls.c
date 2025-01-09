#include <SDL.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include "controls.h"
#include "plutils.h"

void renderButtons(SDL_Window *window, SDL_Renderer *renderer, texControls_t* ctrs) {
        int windowWidth, windowHeight;
        SDL_GetWindowSize(window, &windowWidth, &windowHeight);
        

        int big_button = 128;
        int small_button = 32;
        
        int y_big = (windowHeight / 2) - (big_button / 2);
        int y_small = (windowHeight / 2) + (big_button / 2) - small_button;

        int x_small = 25 + big_button + 15; //+(15+small_button)*m


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
        
        ctrs->stop.renderPos = (struct SDL_Rect) {x_small + (15 + small_button) * 1, y_small, small_button, small_button};
        SDL_RenderCopy(renderer, ctrs->atlas, &ctrs->stop.texPos, &ctrs->stop.renderPos);

        ctrs->next.renderPos = (struct SDL_Rect) {x_small + ((15 + small_button) * 2), y_small, small_button, small_button};
        SDL_RenderCopy(renderer, ctrs->atlas, &ctrs->next.texPos, &ctrs->next.renderPos);
}

void renderTitle(SDL_Window *window, SDL_Renderer *renderer, texControls_t* ctrs) {
    int windowWidth, windowHeight;
        SDL_GetWindowSize(window, &windowWidth, &windowHeight);

        SDL_Rect rect = {
            windowWidth / 4,
            windowHeight / 4,
            windowWidth / 2,
            windowHeight / 2
        };

        rect.x = rect.x - (rect.h / 2);
        rect.h = 0;
        rect.w = 0;

        if (!ctrs->title.title) {
            const char* title = Mix_GetMusicTitle(NULL);
            if (strcmp(title, "") > 0) {
                ctrs->title.title = getTextureFromWords(renderer, ctrs->font, title);
              
                int tempW, tempH;
                SDL_QueryTexture(ctrs->title.title, NULL, NULL, &tempW, &tempH);

                printf("Text W: %i text H: %i \n", tempW, tempH);
                rect.w = tempW;  // 10 pixels padding on each side
                rect.h = tempH; // 5 pixels padding top and bottom
                rect.x = rect.x - (rect.h / 4);
                ctrs->title.renderPos = rect; 
            }

        }
        else {
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
            SDL_RenderCopy(renderer, ctrs->title.title, NULL, &ctrs->title.renderPos);
        }
}