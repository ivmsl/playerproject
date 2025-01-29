#include <SDL.h>
#include <stdio.h>
#include <SDL_ttf.h>
#include "main.h"
#include "playlist.h"
#include "tt_window.h"
#include "plutils.h"
#include "controls.h"


/*******************************
* Plik odpowiedzialny do tworzenia i destrukcje okna playlisty.
* Wyświetla listę utworów playlisty i obsługuje naciśnięcia myszy.
*/

//https://github.com/libsdl-org/SDL/blob/SDL2/include/SDL_events.h#L315
//https://lazyfoo.net/tutorials/SDL/35_window_events/index.php 



wr_couple *playlistWindow = NULL; //handler dla okna i renderera
playlist_block playl_brick[MAX_PLAYLIST_ENTRIES]; //bloki playlistu, MAX_PL_EN defined in playlist.h
int playlist_len = 0;
int START_Y_COORD = 0; //dla scrollingu 
playlist_gui pGUI;

playlist_gui *getpGUIHandler(void) {
    return &pGUI;
}


void render_playlist_if_present(void) {
    if (!playlistWindow) return;
    
    plist *playlist = get_playlist_handler();
    for (int counter = 0; counter < playlist->len; counter++)
    {
        playl_brick[counter].renderPos = (struct SDL_Rect) {0, START_Y_COORD + counter*50, 300, 50};
        SDL_SetRenderDrawColor(playlistWindow->renderer, 183, 45, 45, 100);
        if (playlist->current == counter) SDL_SetRenderDrawColor(playlistWindow->renderer, 65, 105, 255, 100);
        SDL_RenderDrawRect(playlistWindow->renderer, &(playl_brick[counter].renderPos)); 
        if (!playl_brick[counter].title) continue;
        playl_brick[counter].titlePos.y = START_Y_COORD + 5 + counter*50;  
        SDL_RenderCopy(playlistWindow->renderer, playl_brick[counter].title, NULL, &(playl_brick[counter].titlePos));
    }
   

   SDL_SetRenderDrawColor(playlistWindow->renderer, 183, 45, 45, 100);
   SDL_RenderDrawRect(playlistWindow->renderer, &(pGUI.guiPos));

   if (pGUI.atlas) {
        SDL_RenderCopy(playlistWindow->renderer, pGUI.atlas, &(pGUI.clearButton.texPos), &(pGUI.clearButton.renderPos));
        if (!pGUI.isRepeat) { 
            SDL_RenderCopy(playlistWindow->renderer, pGUI.atlas, &(pGUI.repeatButton.texPos), &(pGUI.repeatButton.renderPos)); 
            } else { 
            SDL_RenderCopy(playlistWindow->renderer, pGUI.atlas, &(pGUI.repeatButtonOn.texPos), &(pGUI.repeatButtonOn.renderPos));  
        }     
   }
   SDL_RenderDrawRect(playlistWindow->renderer, &(pGUI.clearButton.renderPos));
   SDL_RenderDrawRect(playlistWindow->renderer, &(pGUI.repeatButton.renderPos));  
}

void changeXCoordScrolling(Sint32 scroll) {
    if (START_Y_COORD + scroll > 0) START_Y_COORD = 0;
    else {
        START_Y_COORD += scroll * 3;
    }
}

wr_couple *GetPlaylistRenderer(void) {
    if (playlistWindow) return playlistWindow;
    else return NULL;
}

void populate_playlist(void) {
    plist *playlist = get_playlist_handler();

    TTF_Font *font = TTF_OpenFont(FONTPATH, 18);
    if (!font) {
        printf("Cannot open the font %s", TTF_GetError());
    }
 
    for (int counter = 0; counter < playlist->len; counter++)
    {
        printf("Playlist: %s\n", playlist->playlist[counter].name);
        playl_brick[counter].title = getTextureFromWords(playlistWindow->renderer, font, playlist->playlist[counter].name);
        if (!playl_brick[counter].title) continue;
        int tempW, tempH;
        SDL_QueryTexture(playl_brick[counter].title, NULL, NULL, &tempW, &tempH);
        playl_brick[counter].titlePos = (struct SDL_Rect) {5, 5 + counter*50, tempW, tempH};

    }
    playlist_len = playlist->len;
    TTF_CloseFont(font);
}

void clearPlaylistBlocks(void) {

    int count = playlist_len;
    for (int i = 0; i < count; i++)
    {
        if (playl_brick[i].title) SDL_DestroyTexture(playl_brick[i].title);
        playl_brick[i].title = NULL;
        playl_brick[i].entry = NULL;
    }
    playlist_len = 0;
}


void updatePlaylistBlocks(void) {
    if (!playlistWindow) return;
    plist *playlist = get_playlist_handler();
    if (!playlist) return;
    //printf("New songs added to playlist %i %i \n", playlist->len, playlist_len);
    if (playlist->len > playlist_len) {
        for (int i = playlist_len; i < playlist->len; i++) {
            TTF_Font *font = TTF_OpenFont(FONTPATH, 18);
            //printf("Playlist: %s\n", playlist->playlist[i].name);
            playl_brick[i].title = getTextureFromWords(playlistWindow->renderer, font, playlist->playlist[i].name);
        if (!playl_brick[i].title) return;
        int tempW, tempH;
        SDL_QueryTexture(playl_brick[i].title, NULL, NULL, &tempW, &tempH);
        playl_brick[i].titlePos = (struct SDL_Rect) {5, 5 + i*50, tempW, tempH};
        }
    }
    playlist_len = playlist->len;
}

Uint32 get_playlist_window_id(void) {
    if (!playlistWindow) return 0;
    if (playlistWindow->window) return SDL_GetWindowID(playlistWindow->window);
    else return 0;
}

int initPlaylistInfoBlock(void) {
    if (!playlistWindow) return -1;
    int tempW, tempH;
    SDL_GetWindowSize(playlistWindow->window, &tempW, &tempH);

    pGUI.atlas = (SDL_Texture*) loadTexture(playlistWindow->renderer, BUTTONS_PLAYLIST_TEX);
    if (!pGUI.atlas) {
        perror("Cannot load texture atlas.\n");
    }
    pGUI.isRepeat = 1;
    
    pGUI.playlist_len = playlist_len;
    //printf("Window size: %d, %d\n", tempW, tempH);
    pGUI.guiPos = (struct SDL_Rect) {0, tempH - 50, 300, 50};


    pGUI.repeatButton.renderPos = (struct SDL_Rect) {
                                    pGUI.guiPos.x + 20, 
                                    pGUI.guiPos.y + 5, 
                                    40, 
                                    40};

    pGUI.repeatButton.texPos = (struct SDL_Rect) {0, 0, 150, 128};                                

    pGUI.repeatButtonOn = pGUI.repeatButton;
    pGUI.repeatButtonOn.texPos = (struct SDL_Rect) {0, 128, 150, 128};                                
    
    pGUI.clearButton.renderPos = (struct SDL_Rect) {
                                    pGUI.guiPos.w - 60, 
                                    pGUI.guiPos.y + 5, 
                                    40, 
                                    40};

    pGUI.clearButton.texPos = (struct SDL_Rect) {150, 0, 150, 128};   
    
    return 0;
}

wr_couple* createAndReturnPlaylistWindow(void) {
    playlistWindow = calloc(1, sizeof(wr_couple));
    playlistWindow->window = SDL_CreateWindow("Playlist",
                              SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              300, 725,
                              SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (playlistWindow->window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return NULL;
    }

    // Create renderer (RENDERER POINTER!!)
    playlistWindow->renderer = SDL_CreateRenderer(playlistWindow->window, -1, SDL_RENDERER_ACCELERATED);
    if (playlistWindow->renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return NULL;
    }    
    populate_playlist();
    initPlaylistInfoBlock();
    return playlistWindow;

}

int destroyPlaylistWindow(void) {
    clearPlaylistBlocks();
    if (playlistWindow) {
        //printf("DESTROOOOY \n");
        if (playlistWindow->renderer) SDL_DestroyRenderer(playlistWindow->renderer);
        if (playlistWindow->window) SDL_DestroyWindow(playlistWindow->window);
        playlistWindow = NULL; //nullPtr
    }
    if (pGUI.atlas) SDL_DestroyTexture(pGUI.atlas);
    pGUI.atlas = NULL;
    pGUI.playlist_len = 0;
    return 0;
}
