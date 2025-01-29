#include <SDL.h>
#include <SDL_mixer.h>
#include <string.h>
#include "playlist.h"
#include "dirutils.h"
#include "controls.h"
//#include <SDL_filesystem.h>

plist *playlist_str;
playlist_entry playlist_array[MAX_PLAYLIST_ENTRIES]; //defined in playlist.h

plist* get_playlist_handler(void) {
    if (playlist_str) {
        return playlist_str;
    }
    else return NULL;

}

int playlist_append(char* dirpath) {//, char* filepath) {
    printf("Playlist_append: %s\n", dirpath);

    if (playlist_str) {
        playlist_entry *playlist_added = &(playlist_array[playlist_str->len]);

        char *file_name = strrchr(dirpath, '/');

        playlist_added->name = calloc(strlen(file_name) + 1, sizeof(char));
        if (playlist_added->name) memcpy(playlist_added->name, ++file_name, strlen(file_name) + 1);
        playlist_added->folder = calloc(strlen(dirpath) - strlen(file_name) + 1, sizeof(char));
        if (playlist_added->folder) {
            memcpy(playlist_added->folder, dirpath, strlen(dirpath) - strlen(file_name));
            playlist_added->folder[strlen(dirpath) - strlen(file_name)] = '\0';
        }

        playlist_str->len++;
        
        return 0;   
    }
    else {
        printf("Playlist nie został utworzony");
        return -1;
    }

    return 0;
}

int initEmptyPlaylist(void) {
    if (!playlist_str) {
        playlist_str = (plist*) calloc(1, sizeof(plist));
        if (!playlist_str) {
            return -1;
            perror("Unable to allocate playlist_str");
        }
    }

    playlist_str->playlist = playlist_array;
    playlist_str->len = 0;
    playlist_array[0].name = NULL;
    return 0;
}

int playlist_from_dir(struct dir_content* dir_c) {

    if (!dir_c) {
        return -1;
    }

    if (!dir_c->first || !dir_c->len) {
        perror("No dir_c first or no dir_c len");
        return -1; //if no first or we don't know how many
        
    }

    
    //playlist_entry *playlist = (playlist_entry*) calloc(dir_c->len, sizeof(playlist_entry));
    if (!playlist_array) {
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
    
    playlist_str->playlist = playlist_array;
    playlist_str->len = 0;
    playlist_str->current = 0;

    dir_ent *mp3_ent = dir_c->first;
    playlist_entry *entry = playlist_array; //if we use playlist on the heap — use that

    while (mp3_ent != NULL) {
        entry->folder = (char *) calloc(strlen(dir_c->path) + 1, sizeof(char));
        
        if (entry->folder) {
            memcpy(entry->folder, dir_c->path, strlen(dir_c->path) + 1);
        } else {
            continue;
        }
        entry->name = (char *) calloc(strlen(mp3_ent->name) + 1, sizeof(char));
        if (entry->name) memcpy(entry->name, mp3_ent->name, strlen(mp3_ent->name) + 1);
        mp3_ent = mp3_ent->next;
        printf("Playlist entry_name: %s \n", entry->name);
        entry++;
        playlist_str->len++;
    }
    playlist_str->current = 0;

    if (!playlist_array) {
        return -1;
    }
    
    return 0;

}

int select_track_from_playlist(int pos) {
    
    Mix_Music *music;
    if (getMusicHandler() != NULL) {
        music = getMusicHandler();
        Mix_HaltMusic();
        Mix_FreeMusic(music);
        
        playlist_entry *cursw = &(playlist_str->playlist[pos]);
        
        printf("TrackName from second: %s Len: %i Pos: %i\n", cursw->name, playlist_str->len, pos);
        char *fullpath = calloc(strlen(cursw->name) + strlen(cursw->folder) + 2, sizeof(char));
        if (fullpath) {
            memcpy(fullpath, cursw->folder, strlen(cursw->folder));
            strcat(fullpath, "/\0");
            strcat(fullpath, cursw->name);

        }
        else {
            return -1;
        }

        if ((music = Mix_LoadMUS(fullpath))) {
            playlist_str->current = pos;
            updateCurrentMusic(music);
        } else {
            free(fullpath);   
            return -1;
        }
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

        if ((music = Mix_LoadMUS(fullpath))) {
            updateCurrentMusic(music);
        } else {
            free(fullpath);
            return -1;
        }
        //Mix_FadeInMusic()
        free(fullpath);
        return 0;
    }
    

}

int playlist_prev(void) {
    if (playlist_str->len == 0) return -1;
    int pos = (playlist_str->current - 1) % (playlist_str->len); 
    if (pos < 0) pos = playlist_str->len - 1;
    return select_track_from_playlist(pos);    
}


int playlist_next(void) {
    if (playlist_str->len == 0) return -1;
    int pos = (playlist_str->current + 1) % (playlist_str->len); 
    return select_track_from_playlist(pos);    
}

void musicFinishedCallback(void) {
    events = NEED_NEXT_TRACK;
}

void registerHaltCallback(void) {
    Mix_HookMusicFinished(musicFinishedCallback);
}


char *GetCurrTrackName(void) {
    char *temp = Mix_GetMusicTitle(NULL);
    if (strcmp(temp, "")) return temp;
    else return playlist_array[playlist_str->current].name;
}

