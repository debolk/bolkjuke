#ifndef LIBBOLKJUKE_H
#define	LIBBOLKJUKE_H

#ifdef	__cplusplus
extern "C" {
#endif

typedef enum {
    BOLKJUKE_STOPPED,
    BOLKJUKE_PLAYING,
    BOLKJUKE_PAUSED
} bolkjuke_state_t;

typedef enum {
    BOLKJUKE_FILE,
    BOLKJUKE_PLAYLIST,
    BOLKJUKE_DIRECTORY
} bolkjuke_type_t;

typedef struct bolkjuke_song bolkjuke_song_t;

typedef struct bolkjuke_song_list bolkjuke_song_list_t;

typedef enum {
    BOLKJUKE_EVENT_STOPPED,
    BOLKJUKE_EVENT_PLAYING,
    BOLKJUKE_EVENT_PAUSED,
} bolkjuke_event_type_t;

typedef struct {
    bolkjuke_event_type_t type;

    struct {
    } stopped;

    struct {
        int index;
    } playing;

    struct {
    } paused;
} bolkjuke_event_t;

typedef void (*bolkjuke_callback_t)(bolkjuke_event_t *event);


void
bolkjuke_new();

void
bolkjuke_destroy();

void
bolkjuke_play();

void
bolkjuke_pause();

void
bolkjuke_play_pause();

void
bolkjuke_prev();

void
bolkjuke_next();

void
bolkjuke_fast_rewind();

void
bolkjuke_fast_forward();

void
bolkjuke_stop();

void
bolkjuke_append(bolkjuke_song_t *song);

void
bolkjuke_insert(bolkjuke_song_t *song, int i);

void
bolkjuke_move(int from, int to);

void
bolkjuke_remove(int i);

void
bolkjuke_clear();

void
bolkjuke_shuffle();

bolkjuke_song_list_t *
bolkjuke_get_filelist();

bolkjuke_song_list_t *
bolkjuke_get_playlist();

bolkjuke_song_list_t *
bolkjuke_read(char *path);

void
bolkjuke_enter(bolkjuke_song_t *song);

void
bolkjuke_leave();

bolkjuke_song_list_t *
bolkjuke_search(char *query);

void
bolkjuke_save(char *name);

void
bolkjuke_attach(bolkjuke_callback_t callback);

void
bolkjuke_log(const char *format, ...);


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


#ifdef	__cplusplus
}
#endif

#endif	/* LIBBOLKJUKE_H */

