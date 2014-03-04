#include "song_list.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

bolkjuke_song_list_t *
bolkjuke_song_list_new()
{
    bolkjuke_song_list_t *list = calloc(1, sizeof (bolkjuke_song_list_t));

    list->refcount++;

    return (list);
}

void
bolkjuke_song_list_retain(bolkjuke_song_list_t *list)
{
    assert(list->refcount > 0);

    list->refcount++;
}

void
bolkjuke_song_list_release(bolkjuke_song_list_t *list)
{
    int i;

    assert(list->refcount > 0);

    list->refcount--;

    if (list->refcount == 0) {
        if (list->path)
            free(list->path);
        for (i = 0; i < list->size; i++)
            bolkjuke_song_release(list->songs[i]);
        free(list->songs);
        free(list);
    }
}

void
bolkjuke_song_list_grow(bolkjuke_song_list_t *list, int capacity)
{
    if (capacity > list->capacity) {
        list->capacity = capacity;
        list->songs = realloc(list->songs, list->capacity * sizeof (bolkjuke_song_t *));
    }
}

int
bolkjuke_song_list_get_size(bolkjuke_song_list_t *list)
{
    return (list->size);
}

bolkjuke_song_t *
bolkjuke_song_list_get(bolkjuke_song_list_t *list, int i)
{
    bolkjuke_song_t *song;

    if (i < 0 || i >= list->size)
        return (NULL);

    song = list->songs[i];
    bolkjuke_song_retain(song);

    return (song);
}

void
bolkjuke_song_list_append(bolkjuke_song_list_t *list, bolkjuke_song_t *song)
{
    bolkjuke_song_list_insert(list, song, list->size);
}

void
bolkjuke_song_list_insert(bolkjuke_song_list_t *list, bolkjuke_song_t *song, int i)
{
    assert(i >= 0);
    assert(i <= list->size);

    bolkjuke_song_retain(song);

    bolkjuke_song_list_grow(list, list->size + 1);

    memcpy(list->songs + i + 1, list->songs + i,
            (list->size - i) * sizeof (bolkjuke_song_t *));

    list->songs[i] = song;
    list->size++;
}

void
bolkjuke_song_list_move(bolkjuke_song_list_t *list, int from, int to)
{
    int i;
    bolkjuke_song_t *tmp;

    assert(from >= 0);
    assert(from < list->size);
    assert(to >= 0);
    assert(to < list->size);

    tmp = list->songs[from];

    if (from < to) {
        memmove(list->songs + from, list->songs + from + 1,
                (to - from) * sizeof (bolkjuke_song_t *));
    } else if (from > to) {
        memmove(list->songs + to + 1, list->songs + to,
                (from - to) * sizeof (bolkjuke_song_t *));
    }

    list->songs[to] = tmp;
}

void
bolkjuke_song_list_remove(bolkjuke_song_list_t *list, int i)
{
    assert(i >= 0);
    assert(i < list->size);

    bolkjuke_song_release(list->songs[i]);

    memmove(list->songs + i, list->songs + i + 1,
            (list->size - i - 1) * sizeof (bolkjuke_song_t *));

    list->size--;
}

void
bolkjuke_song_list_clear(bolkjuke_song_list_t *list)
{
    int i;

    for (i = 0; i < list->size; i++) {
        bolkjuke_song_release(list->songs[i]);
    }

    list->size = 0;
}

void
bolkjuke_song_list_shuffle(bolkjuke_song_list_t *list)
{
    int i;
    int j;
    bolkjuke_song_t *tmp;

    for (i = list->size - 1; i > 0; i--) {
        j = rand() / (RAND_MAX / i + 1);

        tmp = list->songs[i];
        list->songs[i] = list->songs[j];
        list->songs[j] = tmp;
    }
}
