#include <SDL.h>
#include <SDL_image.h>

#ifndef controls_h_
#define controls_h_

enum switchTex {
    PLAY_DISPL,
    PAUSE_DISPL,
    IDLE
};

typedef struct unitControlRender {
    struct SDL_Rect texPos;
    struct SDL_Rect renderPos;
} unitControlRender_t;


typedef struct texControls {
    struct SDL_Texture *atlas; 
    unitControlRender_t play;
    unitControlRender_t pause;
    unitControlRender_t prev;
    unitControlRender_t stop;
    unitControlRender_t next;
    enum switchTex ctrlAct;
} texControls_t;


extern void renderButtons(SDL_Window *window, SDL_Renderer *renderer, texControls_t* ctrs);

#endif
