#include <SDL.h>
#include <stdio.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
//#include "plutils.h"
/*!
    \fn SDL_Texture* loadTexture(SDL_Renderer* renderer, const char* path)
    
    \brief Loads an image file and creates an SDL texture from it.
    
    This function loads an image from the specified path using SDL_image (IMG_Load),
    converts it to an SDL_Texture, and properly manages the memory by freeing the
    intermediate SDL_Surface.
    
    \param renderer Pointer to the SDL_Renderer that will create the texture
    \param path Path to the image file to be loaded
    
    \return Pointer to the created SDL_Texture, or NULL if loading fails
    
    \warning The returned texture must be freed with SDL_DestroyTexture when no longer needed
    \warning If the function returns NULL, check SDL error messages for details
    
    \sa SDL_CreateTextureFromSurface
    \sa IMG_Load
    \sa SDL_FreeSurface
*/
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


SDL_Texture* getTextureFromWords(SDL_Renderer* renderer, TTF_Font* font, char* text) {
    SDL_Color black = {0, 0, 0, 255};
    
    SDL_Surface* tmp = TTF_RenderText_Blended(font, text, black);
    if (!tmp) {
        printf("Error getting text surface: %s\n", TTF_GetError());
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, tmp);
    if (!texture) {
        printf("Error creating texture: %s\n", SDL_GetError());
    }

    SDL_FreeSurface(tmp);
    return texture;

}

int checkAreaClick(int mouseX, int mouseY, struct SDL_Rect* area) {
    
    if (mouseX >= area->x && 
        mouseX < area->x + area->w &&
        mouseY >= area->y && 
        mouseY < area->y + area->h)
    {
        return 1;
    }
    else return 0;        
    
}

