#ifndef BOLKJUKE_SONG_LIST_H_
#define BOLKJUKE_SONG_LIST_H_

#include "song.h"

typedef struct bolkjuke_song_list bolkjuke_song_list_t;

struct bolkjuke_song_list {
    int refcount;
    char *path;
    int playing;
    int size;
    int capacity;
    bolkjuke_song_t **songs;
};

bolkjuke_song_list_t *
bolkjuke_song_list_new();

void
bolkjuke_song_list_retain(bolkjuke_song_list_t *list);

void
bolkjuke_song_list_release(bolkjuke_song_list_t *list);

void
bolkjuke_song_list_grow(bolkjuke_song_list_t *list, int capacity);

int
bolkjuke_song_list_get_size(bolkjuke_song_list_t *list);

bolkjuke_song_t *
bolkjuke_song_list_get(bolkjuke_song_list_t *list, int i);

void
bolkjuke_song_list_append(bolkjuke_song_list_t *list, bolkjuke_song_t *song);

void
bolkjuke_song_list_insert(bolkjuke_song_list_t *list, bolkjuke_song_t *song, int i);

void
bolkjuke_song_list_move(bolkjuke_song_list_t *list, int from, int to);

void
bolkjuke_song_list_remove(bolkjuke_song_list_t *list, int i);

void
bolkjuke_song_list_clear(bolkjuke_song_list_t *list);

void
bolkjuke_song_list_shuffle(bolkjuke_song_list_t *list);

#endif /* BOLKJUKE_SONG_LIST_H_ */
