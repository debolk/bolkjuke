#include "bolkjuke.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <xine.h>

static bolkjuke_state_t state;
static bolkjuke_song_list_t *filelist;
static bolkjuke_song_list_t *playlist;

void
bolkjuke_new()
{
    char *cwd;

    bolkjuke_log_init();

    bolkjuke_log("started");

    state = BOLKJUKE_STOPPED;

    bolkjuke_events_init();
    bolkjuke_engine_init();
    bolkjuke_records_init();

    cwd = getcwd(NULL, 0);
    filelist = bolkjuke_records_read(cwd);
    playlist = bolkjuke_song_list_new();
}

void
bolkjuke_destroy()
{
    bolkjuke_records_destroy();
    bolkjuke_engine_destroy();
    bolkjuke_events_destroy();

    bolkjuke_song_list_release(playlist);
    bolkjuke_song_list_release(filelist);

    bolkjuke_log_destroy();
}

void
bolkjuke_play()
{
    bolkjuke_event_t event;

    state = BOLKJUKE_PLAYING;
    bolkjuke_engine_play();

    event.type = BOLKJUKE_EVENT_PLAYING;
    event.playing.index = playlist->playing;
    bolkjuke_events_callback(&event);
}

void
bolkjuke_pause()
{
    bolkjuke_event_t event;

    state = BOLKJUKE_PAUSED;
    bolkjuke_engine_pause();

    event.type = BOLKJUKE_EVENT_PAUSED;
    bolkjuke_events_callback(&event);
}

void
bolkjuke_resume()
{
    bolkjuke_event_t event;

    state = BOLKJUKE_PLAYING;
    bolkjuke_engine_resume();

	event.type = BOLKJUKE_EVENT_PLAYING;
	event.playing.index = playlist->playing;
	bolkjuke_events_callback(&event);
}

void
bolkjuke_play_pause()
{
    switch (state) {
    case BOLKJUKE_STOPPED:
        if (playlist->playing < playlist->size) {
            bolkjuke_engine_open(playlist->songs[playlist->playing]->path);
            bolkjuke_play();
        }
        break;
    case BOLKJUKE_PLAYING:
        bolkjuke_pause();
        break;
    case BOLKJUKE_PAUSED:
        bolkjuke_resume();
        break;
    }
}

void
bolkjuke_prev()
{
    bolkjuke_event_t event;

    if (playlist->playing == 0) {
        return;
    }

    playlist->playing--;

    bolkjuke_engine_open(playlist->songs[playlist->playing]->path);
    bolkjuke_play();
}

void
bolkjuke_next()
{
    bolkjuke_event_t event;

    if (playlist->playing >= playlist->size - 1) {
        return;
    }

    playlist->playing++;

    bolkjuke_engine_open(playlist->songs[playlist->playing]->path);
    bolkjuke_play();
}

void
bolkjuke_fast_rewind()
{
    // TODO
}

void
bolkjuke_fast_forward()
{
    // TODO
}

void
bolkjuke_stop()
{
    bolkjuke_event_t event;

    state = BOLKJUKE_STOPPED;
    bolkjuke_engine_stop();

    event.type = BOLKJUKE_EVENT_STOPPED;
    bolkjuke_events_callback(&event);
}

void
bolkjuke_append(bolkjuke_song_t *song)
{
    bolkjuke_song_list_append(playlist, song);
    // TODO
}

void
bolkjuke_insert(bolkjuke_song_t *song, int i)
{
    bolkjuke_song_list_insert(playlist, song, i);
    // TODO
}

void
bolkjuke_move(int from, int to)
{
    bolkjuke_song_list_move(playlist, from, to);
    // TODO
}

void
bolkjuke_remove(int i)
{
    bolkjuke_song_list_remove(playlist, i);
    // TODO
}

void
bolkjuke_clear()
{
    bolkjuke_stop();
    bolkjuke_song_list_clear(playlist);
}

void
bolkjuke_shuffle()
{
    bolkjuke_song_list_shuffle(playlist);
    // TODO
}

bolkjuke_song_list_t *
bolkjuke_get_filelist()
{
    bolkjuke_song_list_retain(filelist);

    return (filelist);
}

bolkjuke_song_list_t *
bolkjuke_get_playlist()
{
    bolkjuke_song_list_retain(playlist);

    return (playlist);
}

void
bolkjuke_enter(bolkjuke_song_t *song)
{
    if (filelist)
        bolkjuke_song_list_release(filelist);

    filelist = bolkjuke_records_read(song->path);
}

void
bolkjuke_leave()
{
    char *path = NULL;

    asprintf(&path, "%s/..", filelist->path); // TODO

    if (filelist)
        bolkjuke_song_list_release(filelist);

    filelist = bolkjuke_records_read(path);
}

bolkjuke_song_list_t *
bolkjuke_search(char *query)
{
    bolkjuke_song_list_t *results = bolkjuke_records_search(query);

    return (results);
}

void
bolkjuke_save(char *name)
{
    char *cwd;
    char *path;

    cwd = getcwd(NULL, 0);
    asprintf(&path, "%s/%s", cwd, name);
    free(cwd);
    bolkjuke_records_save(playlist, path);
}

void
bolkjuke_attach(bolkjuke_callback_t callback)
{
    bolkjuke_events_attach(callback);
}
