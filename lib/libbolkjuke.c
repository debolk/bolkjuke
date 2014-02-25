#include "libbolkjuke.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <dirent.h>
#include <sys/stat.h>
#include <assert.h>
#include <xine.h>

static bolkjuke_state_t state;
static bolkjuke_song_list_t *filelist;
static bolkjuke_song_list_t *playlist;
static bolkjuke_callback_t callback;

static xine_t *xine;
static xine_stream_t *stream;
static xine_video_port_t *vo_port;
static xine_audio_port_t *ao_port;
static xine_event_queue_t *event_queue;
static xine_stream_t *meta_stream;
static xine_video_port_t *meta_vo_port;
static xine_audio_port_t *meta_ao_port;

struct bolkjuke_song {
    int refcount;
    bolkjuke_type_t type;
    char *path;
    char *name;
    char *title;
    char *artist;
    char *album;
};

struct bolkjuke_song_list {
    int refcount;
    char *path;
    int playing;
    int size;
    int capacity;
    bolkjuke_song_t **songs;
};

static void
event_listener(void *user_data, const xine_event_t *event)
{
    bolkjuke_log("xine event: %d", event->type);
    switch (event->type) {
    case XINE_EVENT_UI_PLAYBACK_FINISHED:
        bolkjuke_next();
        break;
    }
}

void
bolkjuke_new()
{
    freopen("bolkjuke.log", "a", stderr);

    bolkjuke_log("started");

    xine = xine_new();
    xine_init(xine);

    ao_port = xine_open_audio_driver(xine, "auto", NULL);
    vo_port = xine_open_video_driver(xine, NULL, XINE_VISUAL_TYPE_NONE, NULL);
    stream = xine_stream_new(xine, ao_port, vo_port);

    event_queue = xine_event_new_queue(stream);
    xine_event_create_listener_thread(event_queue, event_listener, NULL);

    meta_ao_port = xine_open_audio_driver(xine, NULL, NULL);
    meta_vo_port = xine_open_video_driver(xine, NULL, XINE_VISUAL_TYPE_NONE, NULL);
    meta_stream = xine_stream_new(xine, meta_ao_port, meta_vo_port);

    state = BOLKJUKE_STOPPED;

    filelist = bolkjuke_read(strdup("."));
    playlist = bolkjuke_song_list_new();
}

void
bolkjuke_destroy()
{
    xine_close(stream);
    xine_event_dispose_queue(event_queue);
    xine_dispose(stream);
    xine_close_audio_driver(xine, ao_port);
    xine_close_video_driver(xine, vo_port);

    xine_dispose(meta_stream);
    xine_close_audio_driver(xine, meta_ao_port);
    xine_close_video_driver(xine, meta_vo_port);

    xine_exit(xine);

    bolkjuke_song_list_release(playlist);
    bolkjuke_song_list_release(filelist);
}

void
bolkjuke_play()
{
    bolkjuke_event_t event;

    state = BOLKJUKE_PLAYING;
    xine_play(stream, 0, 0);

    if (callback) {
        event.type = BOLKJUKE_EVENT_PLAYING;
        event.playing.index = playlist->playing;
        callback(&event);
    }
}

void
bolkjuke_pause()
{
    bolkjuke_event_t event;

    state = BOLKJUKE_PAUSED;
    xine_set_param(stream, XINE_PARAM_SPEED, XINE_SPEED_PAUSE);

    if (callback) {
        event.type = BOLKJUKE_EVENT_PAUSED;
        callback(&event);
    }
}

void
bolkjuke_resume()
{
    bolkjuke_event_t event;

    state = BOLKJUKE_PLAYING;
    xine_set_param(stream, XINE_PARAM_SPEED, XINE_SPEED_NORMAL);

    if (callback) {
        event.type = BOLKJUKE_EVENT_PLAYING;
        event.playing.index = playlist->playing;
        callback(&event);
    }
}

void
bolkjuke_play_pause()
{
    switch (state) {
    case BOLKJUKE_STOPPED:
        if (playlist->playing < playlist->size) {
            xine_open(stream, playlist->songs[playlist->playing]->path);
            bolkjuke_play();
        }
        break;
    case BOLKJUKE_PLAYING:
        bolkjuke_pause();
        break;
    case BOLKJUKE_PAUSED:
        bolkjuke_resume();
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
    xine_open(stream, playlist->songs[playlist->playing]->path);
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

    xine_open(stream, playlist->songs[playlist->playing]->path);
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
    xine_stop(stream);

    if (callback) {
        event.type = BOLKJUKE_EVENT_STOPPED;
        callback(&event);
    }
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

static int
scandir_select(const struct dirent *d)
{
    char *extension;

    if (d->d_type == DT_DIR) {
        if (d->d_name[0] != '.' || strcmp(d->d_name, "..") == 0) {
            return (1);
        }
    } else if (d->d_type == DT_REG) {
        extension = strrchr(d->d_name, '.');
        if (extension && strcasestr("wav mp3 acc ogg flac", ++extension)) {
            return (1);
        }
    }

    return (0);
}

static int
scandir_compare(const struct dirent **d1, const struct dirent **d2)
{
    if (strcmp((*d1)->d_name, "..") == 0) {
        return (-1);
    } else if (strcmp((*d2)->d_name, "..") == 0) {
        return (1);
    }

    if ((*d1)->d_type & DT_DIR) {
        if ((*d2)->d_type & DT_DIR) {
            return (strcasecmp((*d1)->d_name, (*d2)->d_name));
        } else {
            return (-1);
        }
    } else {
        if ((*d2)->d_type & DT_DIR) {
            return (1);
        } else {
            return (strcasecmp((*d1)->d_name, (*d2)->d_name));
        }
    }
}

bolkjuke_song_list_t *
bolkjuke_read(char *path)
{
    bolkjuke_song_list_t *songlist;
    bolkjuke_song_t *song;
    char *extension;
    struct dirent **dirents;
    int num_dirents;
    int i;

    bolkjuke_log("read: %s", path);

    songlist = bolkjuke_song_list_new();
    songlist->path = strdup(path);
    num_dirents = scandir(path, &dirents, scandir_select, scandir_compare);
    bolkjuke_song_list_grow(songlist, num_dirents);

    for (i = 0; i < num_dirents; i++) {
        song = bolkjuke_song_new();
        asprintf(&song->path, "%s/%s", path, dirents[i]->d_name);

        if (dirents[i]->d_type & DT_DIR) {
            song->type = BOLKJUKE_DIRECTORY;
            song->name = strdup(dirents[i]->d_name);
        } else /*if (dirents[i]->d_type & DT_REG)*/ {
            song->type = BOLKJUKE_FILE;
            extension = strrchr(dirents[i]->d_name, '.');
            song->name = strndup(dirents[i]->d_name, extension - dirents[i]->d_name);
            bolkjuke_song_parse(song);
        }

        bolkjuke_song_list_append(songlist, song);
        bolkjuke_song_release(song);
    }

    return (songlist);
}

void
bolkjuke_enter(bolkjuke_song_t *song)
{
    if (filelist)
        bolkjuke_song_list_release(filelist);

    filelist = bolkjuke_read(song->path);
}

void
bolkjuke_leave()
{
    char *path = NULL;

    asprintf(&path, "%s/..", filelist->path); // TODO

    if (filelist)
        bolkjuke_song_list_release(filelist);

    filelist = bolkjuke_read(path);
}

bolkjuke_song_list_t *
bolkjuke_search(char *query)
{
    bolkjuke_song_list_t *searchlist = bolkjuke_song_list_new();

    // TODO

    return (searchlist);
}

void
bolkjuke_save(char *name)
{
    // TODO
}

void
bolkjuke_attach(bolkjuke_callback_t cb)
{
    assert(callback == NULL); // TODO

    callback = cb;
}

void
bolkjuke_log(const char *format, ...)
{
    va_list args;

    fprintf(stderr, "libbolkjuke: ");
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fprintf(stderr, "\n");
    fflush(stderr);
}

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

void
bolkjuke_song_parse(bolkjuke_song_t *song)
{
    const char *meta;

    xine_open(meta_stream, song->path);
    meta = xine_get_meta_info(meta_stream, XINE_META_INFO_TITLE);
    if (meta != NULL)
        song->title = strdup(meta);
    meta = xine_get_meta_info(meta_stream, XINE_META_INFO_ARTIST);
    if (meta != NULL)
        song->artist = strdup(meta);
    meta = xine_get_meta_info(meta_stream, XINE_META_INFO_ALBUM);
    if (meta != NULL)
        song->album = strdup(meta);
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
