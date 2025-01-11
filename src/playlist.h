#ifndef playlist_h_
#define playlist_h_

#include "dirutils.h"

typedef struct playlist_entry {
    char* folder;
    char* name;
} playlist_entry;

typedef struct plist {
    playlist_entry *playlist;
    int len;
    int current;
} plist;

extern int playlist_from_dir(struct dir_content* dir_c);
extern plist* get_playlist_handler(void);
extern int playlist_next(void);
extern int playlist_prev(void);

#endif
