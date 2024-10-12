#include <SDL.h>
#include <stdio.h>
#include "controls.h"

void renderButtons(SDL_Window *window, SDL_Renderer *renderer, texControls_t* ctrs) {
        int windowWidth, windowHeight;
        SDL_GetWindowSize(window, &windowWidth, &windowHeight);
        
        int buttonWidth = 150 / 3;
        int buttonHeight = 128 / 3;
        int y = windowHeight - buttonHeight - 10;  // 10 pixels from the bottom

        //SDL_Rect dstRect = {0, y, buttonWidth, buttonHeight};

        ctrs->play.renderPos = (struct SDL_Rect) {10, y, buttonWidth, buttonHeight}; 
        ctrs->pause.renderPos = (struct SDL_Rect) {10, y, buttonWidth, buttonHeight}; 
        
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

        ctrs->prev.renderPos = (struct SDL_Rect) {windowWidth - (buttonWidth * 3) - 10, y, buttonWidth, buttonHeight};
        SDL_RenderCopy(renderer, ctrs->atlas, &ctrs->prev.texPos, &ctrs->prev.renderPos);
        
        ctrs->stop.renderPos = (struct SDL_Rect) {windowWidth - (buttonWidth * 2) - 10, y, buttonWidth, buttonHeight};
        SDL_RenderCopy(renderer, ctrs->atlas, &ctrs->stop.texPos, &ctrs->stop.renderPos);

        ctrs->next.renderPos = (struct SDL_Rect) {windowWidth - buttonWidth - 10, y, buttonWidth, buttonHeight};
        SDL_RenderCopy(renderer, ctrs->atlas, &ctrs->next.texPos, &ctrs->next.renderPos);
}