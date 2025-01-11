#include <SDL.h>
#include <SDL_mixer.h>
#include <string.h>
#include "playlist.h"
#include "dirutils.h"
#include "controls.h"
//#include <SDL_filesystem.h>

plist *playlist_str;

plist* get_playlist_handler(void) {
    if (playlist_str) {
        return playlist_str;
    }
    else return NULL;

}

int playlist_from_dir(struct dir_content* dir_c) {

    if (!dir_c) {
        return -1;
    }

    if (!dir_c->first || !dir_c->len) {
        perror("No dir_c first or no dir_c len");
        return -1; //if no first or we don't know how many
        
    }

    playlist_entry *playlist = (playlist_entry*) calloc(dir_c->len, sizeof(playlist_entry));
    if (!playlist) {
        perror("Unable to allocate playlist_entry array");
        return -1;
    }

    if (!playlist_str) {
        playlist_str = (plist*) calloc(1, sizeof(plist));
        if (!playlist_str) {
            return -1;
            perror("Unable to allocate playlist_str");
        }
    }
    
    playlist_str->playlist = playlist;
    playlist_str->len = 0;

    dir_ent *mp3_ent = dir_c->first;

    while (mp3_ent != NULL) {
        playlist->folder = (char *) calloc(strlen(dir_c->path) + 1, sizeof(char));
        
        if (playlist->folder) {
            memcpy(playlist->folder, dir_c->path, strlen(dir_c->path) + 1);
        } else {
            continue;
        }
        playlist->name = (char *) calloc(strlen(mp3_ent->name) + 1, sizeof(char));
        if (playlist->name) memcpy(playlist->name, mp3_ent->name, strlen(mp3_ent->name) + 1);
        mp3_ent = mp3_ent->next;
        printf("Playlist entry_name: %s \n", playlist->name);
        playlist++;
        playlist_str->len++;
    }
    playlist_str->current = 0;

    if (!playlist) {
        return -1;
    }
    
    return 0;

}

int playlist_next(void) {
    Mix_Music *music;
    if (getMusicHandler() != NULL) {
        music = getMusicHandler();
        Mix_HaltMusic();
        Mix_FreeMusic(music);
        int pos = (playlist_str->current + 1) % (playlist_str->len); 
        playlist_entry *cursw = &(playlist_str->playlist[pos]);
        
        printf("TrackName from second: %s Len: %i Pos: %i\n", cursw->name, playlist_str->len, pos);
        char *fullpath = calloc(strlen(cursw->name) + strlen(cursw->folder) + 2, sizeof(char));
        if (fullpath) {
            memcpy(fullpath, cursw->folder, strlen(cursw->folder));
            strcat(fullpath, "/\0");
            strcat(fullpath, cursw->name);

        }
        else return -1;

        if ((music = Mix_LoadMUS(fullpath))) {
            playlist_str->current = pos;
            updateCurrentMusic(music);
        } else return -1;
        //Mix_FadeInMusic()
        free(fullpath);
        return 0;
    }
    else {
        playlist_entry *cursw = &(playlist_str->playlist[playlist_str->current]);
        //printf("TrackName: %s\n", cursw->name);
        char *fullpath = calloc(strlen(cursw->name) + strlen(cursw->folder) + 2, sizeof(char));
        if (fullpath) {
            memcpy(fullpath, cursw->folder, strlen(cursw->folder));
            strcat(fullpath, "/\0");
            strcat(fullpath, cursw->name);
            
            //printf("TrackName: %s\n", fullpath);
        }
        else return -1;

        if (music = Mix_LoadMUS(fullpath)) {
            updateCurrentMusic(music);
        } else return -1;
        //Mix_FadeInMusic()
        free(fullpath);
        return 0;
    }
    

}
