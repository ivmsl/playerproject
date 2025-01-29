#include "dirutils.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include "stdio.h"
#include "main.h"

// Pointer to currently opened render-window couple
wr_couple *dirWindow = NULL;

/**
    \fn wr_couple createAndReturnDirWindow()
    \brief Creates and initializes a playlist window with renderer

    Allocates memory for a wr_couple structure, creates an SDL window
    for playlist display, and initializes an accelerated renderer. Also populates 
    the playlist content.

    \return Pointer to initialized window-renderer couple structure (wr_couple*)
    \returns NULL if window or renderer creation fails

    \warning Caller is responsible for freeing returned wr_couple structure
    \warning Window is created as resizable with initial size 725x300

    \sa populate_playlist()
    \sa SDL_CreateWindow()
    \sa SDL_CreateRenderer()
*/
wr_couple* createAndReturnDirWindow(void) {
    dirWindow = calloc(1, sizeof(wr_couple));
    dirWindow->window = SDL_CreateWindow("Select files",
                              SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              725, 300,
                              SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (dirWindow->window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return NULL;
    }

    // Create renderer (RENDERER POINTER!!)
    dirWindow->renderer = SDL_CreateRenderer(dirWindow->window, -1, SDL_RENDERER_ACCELERATED);
    if (dirWindow->renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return NULL;
    }    
    return dirWindow;

}

/**
 * \fn int destroyDirWindow()
 * \brief Destroy Directory List window if it was previously inicialised. 
 *  
 * Funciton grabs dirWindow declared as global for dirwindow's file scope.
 * Afther it checks if there's renderer and window declared (if there is corresponding pointers).
 * 
 * \return 0 after function is done whether or not it was destroyed
 * \sa SDL_DestroyRenderer()
 * \sa SDL_DestroyWindow()
*/
int destroyDirWindow(void) {
    if (dirWindow) {
        //printf("DESTROOOOY \n");
        if (dirWindow->renderer) SDL_DestroyRenderer(dirWindow->renderer);
        if (dirWindow->window) SDL_DestroyWindow(dirWindow->window);
        dirWindow = NULL; //nullPtr
    }
    return 0;
}