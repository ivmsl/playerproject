#include <stdio.h>
#include "dirutils.h"
#include <stdlib.h>
#include <limits.h>

struct dir_content *dir_c = NULL;

void free_dir_content(struct dir_content *dir_con) {
        free(dir_con->path);
        dir_ent *dir_entry;
        dir_ent *next_dir_entry;

        if (dir_con->first) {
            dir_entry = dir_con->first;
            while (dir_entry != NULL) {
                next_dir_entry = dir_entry->next;
                free(dir_entry);
                dir_entry = next_dir_entry;
            }
        }
        //free(dir_c->first);
        free(dir_con);
}

    
char* get_curr_dir(void) {
        if (dir_c) {
            return dir_c->path;
        }
        else {
            return NULL;
        }
}


#ifdef _WIN32
    #include <windows.h>
#else
    #include <dirent.h>
    #include <string.h>
#endif

#ifdef _WIN32
    void 
#else



    struct dir_content* open_directory(const char *path) {
        DIR *dir;
        struct dirent *entry;
        //personal directory cont
        //
        dir_ent *mp3_ent;

        dir = opendir(path);
        if (dir == NULL) {
            perror("Unable to open dir \n");
            return -1;
        }

        dir_c = (struct dir_content*) calloc(1, sizeof(struct dir_content));
        if (!dir_c) {
            perror("Unable to allocate for dir_c");
            return -1;
        }

        dir_c->path = realpath(path, NULL);
        if (!dir_c->path) {
            perror("Unable to allocate for dir_c->path");
            return -1;
        }

        printf("Path: %s \n", dir_c->path);
        printf("Dir_name: %s \n", path);


        while ((entry = readdir(dir)) != NULL) {
            if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) continue;
            
            if (strstr(entry->d_name, ".mp3") != NULL) {
                //printf("Found MP3: %s\n", entry->d_name);
                if (!dir_c->first) {
                    mp3_ent = (dir_ent*) calloc(1, sizeof(struct dir_ent));
                    mp3_ent->name = (char*) calloc(entry->d_namlen + 1, sizeof(char));
                    memcpy(mp3_ent->name, entry->d_name, entry->d_namlen);
                    mp3_ent->next = NULL;
                    dir_c->first = mp3_ent;
                    if (!mp3_ent) break;
                } else {
                    mp3_ent->next = (dir_ent*) calloc(1, sizeof(struct dir_ent));
                    if (mp3_ent->next) {
                        mp3_ent = mp3_ent->next;
                    }
                    mp3_ent->name = (char*) calloc(entry->d_namlen + 1, sizeof(char));
                    memcpy(mp3_ent->name, entry->d_name, entry->d_namlen);
                    mp3_ent->next = NULL;
                }
                dir_c->len++;
            
            }
            
        }

        printf("Len: %i\n", dir_c->len);

        mp3_ent = dir_c->first;
        while (mp3_ent != NULL) {
            if (mp3_ent->name) printf("Name: %s \n", mp3_ent->name);
            mp3_ent = mp3_ent->next;
        }

        closedir(dir);
        return dir_c;

    }


#endif
