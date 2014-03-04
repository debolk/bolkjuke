#include "song.h"

#include <assert.h>
#include <stdlib.h>

bolkjuke_song_t *
bolkjuke_song_new()
{
    bolkjuke_song_t *song = calloc(1, sizeof (bolkjuke_song_t));

    song->refcount = 1;

    return (song);
}

void
bolkjuke_song_retain(bolkjuke_song_t *song)
{
    assert(song->refcount > 0);

    song->refcount++;
}

void
bolkjuke_song_release(bolkjuke_song_t *song)
{
    assert(song->refcount > 0);

    song->refcount--;

    if (song->refcount == 0) {
        if (song->name)
            free(song->name);
        if (song->path)
            free(song->path);
        if (song->title)
            free(song->title);
        if (song->artist)
            free(song->artist);
        if (song->album)
            free(song->album);
        free(song);
    }
}

int
bolkjuke_song_get_type(bolkjuke_song_t *song)
{
    return (song->type);
}

char *
bolkjuke_song_get_path(bolkjuke_song_t *song)
{
    return (song->path);
}

char *
bolkjuke_song_get_name(bolkjuke_song_t *song)
{
    return (song->name);
}

char *
bolkjuke_song_get_title(bolkjuke_song_t *song)
{
    return (song->title);
}

char *
bolkjuke_song_get_artist(bolkjuke_song_t *song)
{
    return (song->artist);
}

char *
bolkjuke_song_get_album(bolkjuke_song_t *song)
{
    return (song->album);
}
