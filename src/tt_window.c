#include <SDL.h>
#include "main.h"
#include <stdio.h>
#include "playlist.h"
#include "tt_window.h"
#include "plutils.h"
#include <SDL_ttf.h>

/*******************************
* Plik odpowiedzialny do tworzenia i destrukcje okna playlisty.
* Wyświetla listę utworów playlisty i obsługuje naciśnięcia myszy.
*/

//https://github.com/libsdl-org/SDL/blob/SDL2/include/SDL_events.h#L315
//https://lazyfoo.net/tutorials/SDL/35_window_events/index.php 



wr_couple *playlistWindow = NULL; //handler dla okna i renderera
playlist_block playl_brick[MAX_PLAYLIST_ENTRIES]; //bloki playlistu, MAX_PL_EN defined in playlist.h

void *render_playlist_if_present(void) {
    if (!playlistWindow) return NULL;
    
    plist *playlist = get_playlist_handler();
    for (int counter = 0; counter < playlist->len; counter++)
    {
        playl_brick[counter].renderPos = (struct SDL_Rect) {0, 0 + counter*50, 300, 50};
        SDL_SetRenderDrawColor(playlistWindow->renderer, 183, 45, 45, 100);
        if (playlist->current == counter) SDL_SetRenderDrawColor(playlistWindow->renderer, 65, 105, 255, 100);
        SDL_RenderDrawRect(playlistWindow->renderer, &(playl_brick[counter].renderPos)); 
        if (!playl_brick[counter].title) continue;
        SDL_RenderCopy(playlistWindow->renderer, playl_brick[counter].title, NULL, &(playl_brick[counter].titlePos));
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
    TTF_CloseFont(font);
}

void clear_playlist_blocks(void) {
    plist *playlist = get_playlist_handler();
    if (!playlist) return;

    int count = playlist->len;
    for (int i = 0; i < count; i++)
    {
        if (playl_brick[i].title) SDL_DestroyTexture(playl_brick[i].title);
        playl_brick[i].title = NULL;
    }
    
}


void append_playlist_block(int *index, playlist_entry *entry) {
    if (index) {
        TTF_Font *font = TTF_OpenFont(FONTPATH, 18);
        playl_brick[*index].title = getTextureFromWords(playlistWindow->renderer, font, entry->name);
        if (!playl_brick[*index].title) return;
        int tempW, tempH;
        SDL_QueryTexture(playl_brick[*index].title, NULL, NULL, &tempW, &tempH);
        playl_brick[*index].titlePos = (struct SDL_Rect) {5, 5 + (*index)*50, tempW, tempH};
    }

}

Uint32 get_playlist_window_id(void) {
    if (!playlistWindow) return 0;
    if (playlistWindow->window) return SDL_GetWindowID(playlistWindow->window);
}

wr_couple* createAndReturnPlaylistWindow(void) {
    playlistWindow = calloc(1, sizeof(wr_couple));
    playlistWindow->window = SDL_CreateWindow("Playlist",
                              SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              300, 725,
                              SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (playlistWindow->window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // Create renderer (RENDERER POINTER!!)
    playlistWindow->renderer = SDL_CreateRenderer(playlistWindow->window, -1, SDL_RENDERER_ACCELERATED);
    if (playlistWindow->renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }    
    populate_playlist();
    return playlistWindow;

}

int destroyPlaylistWindow(void) {
    clear_playlist_blocks();
    if (playlistWindow) {
        //printf("DESTROOOOY \n");
        if (playlistWindow->renderer) SDL_DestroyRenderer(playlistWindow->renderer);
        if (playlistWindow->window) SDL_DestroyWindow(playlistWindow->window);
        playlistWindow = NULL; //nullPtr
    }
    return 0;
}