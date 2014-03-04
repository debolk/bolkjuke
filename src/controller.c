#include "controller.h"
#include "gui.h"
#include "lib/bolkjuke.h"

#include <stdlib.h>

static int running;
static void callback(bolkjuke_event_t *event);

void
controller_init()
{
    bolkjuke_attach(callback);

    running = 1;
}

static void
callback(bolkjuke_event_t *event)
{
    switch (event->type) {
    case BOLKJUKE_EVENT_PLAYING:
        info_set(playback, bolkjuke_song_list_get(
                playlist->songlist, event->playing.index));
        info_draw(playback);

        playlist->playing = event->playing.index;
        list_draw(playlist);

        doupdate();
        break;
    }
}

void
controller_destroy()
{
}

int
controller_is_running()
{
    return (running);
}

void
controller_exit()
{
    if (bar_ask_yes_or_no(bar, "Exit"))
        running = 0;
}

void
controller_play_pause_playback()
{
    bolkjuke_play_pause();
}

void
controller_stop_playback()
{
    bolkjuke_stop();
}

void
controller_play_next()
{
    bolkjuke_next();
}

void
controller_play_previous()
{
    bolkjuke_prev();
}

void
controller_fast_forward_playback()
{
    bolkjuke_fast_forward();
}

void
controller_fast_rewind_playback()
{
    bolkjuke_fast_rewind();
}

void
controller_open_record()
{
    bolkjuke_song_t *song = list_get_selected_song(filelist);

    bolkjuke_enter(song);
    bolkjuke_song_release(song);

    list_set(filelist, bolkjuke_get_filelist());
    list_draw(filelist);
}

void
controller_close_record()
{
    bolkjuke_leave();

    list_set(filelist, bolkjuke_get_filelist());
    list_draw(filelist);
}

void
controller_search_records()
{
    char *answer;

    if ((answer = bar_ask(bar, "Search for")))
        bolkjuke_search(answer);
    free(answer);
}

void
controller_add_song()
{
    bolkjuke_song_t *song = list_get_selected_song(filelist);

    if (filelist->selected == filelist->prev_selected &&
            filelist->selected == filelist->prev_added) return;

    filelist->prev_added = filelist->selected;

    bolkjuke_append(song);
    bolkjuke_song_release(song);

    list_driver(filelist, LIST_DOWN);
    list_draw(filelist);
    list_draw(playlist);
}

void
controller_insert_song()
{
    bolkjuke_song_t *song = list_get_selected_song(filelist);

    if (filelist->selected < 0 ||
            filelist->selected == filelist->prev_selected &&
            filelist->selected == filelist->prev_added) return;

    filelist->prev_added = filelist->selected;

    bolkjuke_insert(song, playlist->selected + 1);
    bolkjuke_song_release(song);

    list_driver(filelist, LIST_DOWN);
    list_draw(filelist);
    list_draw(playlist);
}

void
controller_add_song_recursive()
{
    // TODO
}

void
controller_insert_song_recursive()
{
    // TODO
}

void
controller_delete_song()
{
    bolkjuke_remove(playlist->selected);
    list_draw(playlist);
}

void
controller_move_song_up()
{
    if (playlist->selected == 0) return;

    bolkjuke_move(playlist->selected, playlist->selected - 1);
    list_driver(playlist, LIST_UP);
    list_draw(playlist);
}

void
controller_move_song_down()
{
    if (playlist->selected == playlist->size - 1) return;

    bolkjuke_move(playlist->selected, playlist->selected + 1);
    list_driver(playlist, LIST_DOWN);
    list_draw(playlist);
}

void
controller_save_playlist()
{
    char *answer;

    if ((answer = bar_ask(bar, "Save playlist as")))
        bolkjuke_save(answer);
    free(answer);
}

void
controller_clear_playlist()
{
    if (bar_ask_yes_or_no(bar, "Clear playlist")) {
        bolkjuke_clear();
        list_draw(playlist);
    }
}

void
controller_shuffle_playlist()
{
    if (bar_ask_yes_or_no(bar, "Shuffle playlist")) {
        bolkjuke_shuffle();
        list_draw(playlist);
    }
}
