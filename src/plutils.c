#include <SDL.h>
#include <stdio.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <SDL_image.h>

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
