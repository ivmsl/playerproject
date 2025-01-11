#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#ifndef controls_h_
#define controls_h_



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
    CHANGING_TRACK,
    PLAYER_PLAYING,
    NEED_PAUSING,
    PLAYER_PAUSED,
    NEED_RESUME,
    UPDATING_PLAYLIST,
    PLAYER_STOPPED,
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
    unitControlRender_t play;
    unitControlRender_t pause;
    unitControlRender_t prev;
    unitControlRender_t stop;
    unitControlRender_t next;
    enum switchTex ctrlAct;
    
} texControls_t;

struct RAW_SOURCES {
    struct SDL_Texture *atlas; 
    TTF_Font* font;
};

struct GUI {
    slider_info_t slider;
    title_t title;
    texControls_t buttons;
    Mix_Music *currentlyPlaying;
};


extern int initGUI(SDL_Renderer *renderer, const char *font_path, const char *atl_path);
extern void renderButtons(SDL_Window *window, SDL_Renderer *renderer);
extern void renderTitle(SDL_Window *window, SDL_Renderer *renderer);
extern void renderSlider(SDL_Window *window, SDL_Renderer *renderer);
extern void playPauseAndSwitchButton();
extern int deinitGUI(void);
extern struct GUI* getGUIHandler(void);
extern struct GUI *gui;
extern Mix_Music* getMusicHandler(void);
enum playerEvent events;
void updateCurrentMusic(Mix_Music *music);
void setSliderPos(int x);

#endif
