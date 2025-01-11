#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#ifndef controls_h_
#define controls_h_

struct GUI {
    
};

typedef struct title {
    struct SDL_Rect renderPos;
    SDL_Texture *title;
    //ou bien sa doit etre struct_text
} title_t;


typedef struct slider_info {
    double duration; //Mix_MusicDuration(Mix_Music* music)
    double position; //Mix_GetMusicPosition(Mix_Music* music)
    struct SDL_Rect slider_bar;
    struct SDL_Rect slider;
} slider_info_t;

///
//j'ai besoin d'avoir quelque text primitives? 

enum playerEvent {
    NOOP,
    NEED_NEXT_TRACK,
    PLAYER_PLAYING,
    NEED_PAUSING,
    PLAYER_PAUSED,
    NEED_RESUME,
    UPDATING_PLAYLIST,
    PLAYER_STOPPED
};

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
    title_t title;
    TTF_Font* font;
    slider_info_t slider;
} texControls_t;


extern void renderButtons(SDL_Window *window, SDL_Renderer *renderer, texControls_t* ctrs);
extern void renderTitle(SDL_Window *window, SDL_Renderer *renderer, texControls_t* ctrs);
extern void renderSlider(SDL_Window *window, SDL_Renderer *renderer, texControls_t* ctrs);
extern void playPauseAndSwitchButton(texControls_t* controls, Mix_Music* music);

#endif
