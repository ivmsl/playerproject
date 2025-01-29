#include <SDL.h>

#ifndef main_h_
#define main_h_

#define FONTPATH "res/fonts/Roboto-Medium.ttf"

typedef struct wr_couple {
    SDL_Window *window;
    SDL_Renderer *renderer;
    Uint32 windowID;
} wr_couple;

struct winManager {
    wr_couple mainWindow;
    wr_couple *playlistWindow;
    wr_couple *filesWindow;
    wr_couple *errWindow;
};

enum windowEvents {
    WIN_IDLE,
    CLICK_PLAYLIST_OPEN,
    PLAYLIST_OPENED,
    CLICK_PLAYLIST_CLOSE,
    PLAYLIST_CLOSED,
    CLICK_FILEDIR_OPEN,
    FILEDIR_OPENED,
    CLICK_FILEDIR_CLOSE,
    FILEDIR_CLOSED
};


#endif
