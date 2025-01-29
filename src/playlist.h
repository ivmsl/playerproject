#ifndef playlist_h_
#define playlist_h_

#include "dirutils.h"

#define MAX_PLAYLIST_ENTRIES 255

typedef struct playlist_entry {
    char* folder; //**< String containing folder path*/
    char* name; //**< String containing filename*/
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
extern void registerHaltCallback(void);
char *GetCurrTrackName(void); 
void updatePlaylistBlocks(void);
int initEmptyPlaylist(void);

int playlist_append(char* dirpath); 

#endif
