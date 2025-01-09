#include <SDL.h>
#include <stdio.h>
#include "controls.h"

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
