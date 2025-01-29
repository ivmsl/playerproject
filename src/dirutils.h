#ifndef dirutils_h_
#define dirutils_h_

#include <stdio.h>
#include <SDL.h>

#ifdef _WIN32
    
#else
    extern struct dir_content* open_directory(const char *path);
    extern void free_dir_content(struct dir_content *dir_c);
#endif


typedef struct dir_ent {
    char *name;
    struct dir_ent *next;
} dir_ent;


struct dir_content {
    char *path;
    dir_ent *first;
    unsigned len;
};

typedef struct dir_disp {
    SDL_Texture *name_tex;
    char *dir_path;
    char *name;
} dir_disp;


#endif
