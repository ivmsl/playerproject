#include <SDL.h>
#include <SDL_ttf.h>

#ifndef plutils_h_
#define plutils_h_

extern int checkAreaClick(int mouseX, int mouseY, struct SDL_Rect* area);
extern SDL_Texture* loadTexture(SDL_Renderer* renderer, const char* path);
extern SDL_Texture* getTextureFromWords(SDL_Renderer* renderer, TTF_Font* font, char* text);


#endif
