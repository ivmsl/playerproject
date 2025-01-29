#include <SDL.h>
#include <stdio.h>
#include "main.h"
#include "playlist.h"
#include "controls.h"

#ifndef tt_window_h_
#define tt_window_h_

typedef struct playlist_block {
    playlist_entry *entry;
    struct SDL_Rect renderPos;
    SDL_Texture *title;
    struct SDL_Rect titlePos;
} playlist_block;

typedef struct playlist_info {
    unitControlRender_t clearButton;
    unitControlRender_t repeatButton;
    struct SDL_Rect guiPos; 
    uint playlist_len;
    struct SDL_Texture *atlas;
} playlist_gui;


extern void *render_playlist_if_present(void);
extern Uint32 get_playlist_window_id(void);
extern int destroyPlaylistWindow(void);
extern wr_couple* createAndReturnPlaylistWindow(void);
extern wr_couple *GetPlaylistRenderer(void);
void changeXCoordScrolling(Sint32 scroll);

#endif
