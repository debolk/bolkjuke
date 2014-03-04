#ifndef BOLKJUKE_SONG_H_
#define BOLKJUKE_SONG_H_

typedef enum {
    BOLKJUKE_FILE,
    BOLKJUKE_PLAYLIST,
    BOLKJUKE_DIRECTORY
} bolkjuke_type_t;

typedef struct bolkjuke_song bolkjuke_song_t;

struct bolkjuke_song {
    int refcount;
    bolkjuke_type_t type;
    char *path;
    char *name;
    char *title;
    char *artist;
    char *album;
};

bolkjuke_song_t *
bolkjuke_song_new();

void
bolkjuke_song_retain(bolkjuke_song_t *song);

void
bolkjuke_song_release(bolkjuke_song_t *song);

int
bolkjuke_song_get_type(bolkjuke_song_t *song);

char *
bolkjuke_song_get_path(bolkjuke_song_t *song);

char *
bolkjuke_song_get_name(bolkjuke_song_t *song);

void
bolkjuke_song_parse(bolkjuke_song_t *song);

char *
bolkjuke_song_get_title(bolkjuke_song_t *song);

char *
bolkjuke_song_get_artist(bolkjuke_song_t *song);

char *
bolkjuke_song_get_album(bolkjuke_song_t *song);

#endif /* BOLKJUKE_SONG_H_ */
