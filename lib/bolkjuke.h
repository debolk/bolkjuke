#ifndef LIBBOLKJUKE_H
#define	LIBBOLKJUKE_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "log.h"
#include "song.h"
#include "song_list.h"
#include "events.h"
#include "engine.h"
#include "records.h"

typedef enum {
    BOLKJUKE_STOPPED,
    BOLKJUKE_PLAYING,
    BOLKJUKE_PAUSED
} bolkjuke_state_t;

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

#ifdef	__cplusplus
}
#endif

#endif	/* LIBBOLKJUKE_H */
