#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <curses.h>

#include "main.h"

void
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
    }
}

void
list_input(List *list, int c)
{
    switch (c) {
    case KEY_HOME:
        list_driver(list, LIST_HOME);
        break;
    case KEY_END:
        list_driver(list, LIST_END);
        break;
    case KEY_UP:
        list_driver(list, LIST_UP);
        break;
    case KEY_DOWN:
        list_driver(list, LIST_DOWN);
        break;
    case KEY_PPAGE:
        list_driver(list, LIST_PAGE_UP);
        break;
    case KEY_NPAGE:
        list_driver(list, LIST_PAGE_DOWN);
        break;
    case '0'...'9':
    case 'a'...'z':
    case 'A'...'Z':
    case ' ':
    case '-':
        list_driver(list, c);
        break;
    case KEY_DC: // backspace
        list_driver(list, LIST_PATTERN_DELETE);
        break;
    case 27: // esc
        list_driver(list, LIST_PATTERN_CLEAR);
        break;
    default:
        return;
    }
    list_draw(list);

    info_set(info, list_get_selected_song(list));
    info_draw(info);
}

void
filelist_enter()
{
    bolkjuke_song_t *song = list_get_selected_song(filelist);

    bolkjuke_enter(song);
    bolkjuke_song_release(song);

    list_set(filelist, bolkjuke_get_filelist());
    list_draw(filelist);
}

void
filelist_leave()
{
    bolkjuke_leave();

    list_set(filelist, bolkjuke_get_filelist());
    list_draw(filelist);
}

void
filelist_add()
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
filelist_insert()
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
filelist_input(int c)
{
    int alt = 0;
    bolkjuke_song_t *song = list_get_selected_song(filelist);

    if (song == NULL) {
        return;
    }

    if (c == 27) {
        alt = 1;
        c = getch();
    }

    switch (c) {
    case '\n':
    case '\r':
    case KEY_ENTER:
        if (filelist->selected == -1) break;
        if (bolkjuke_song_get_type(song) != BOLKJUKE_FILE) {
            if (alt) {
                filelist_add();
            } else {
                filelist_enter();
            }
        } else {
            filelist_add();
        }
        break;

    case KEY_IC:
        if (filelist->selected == -1) break;
        if (bolkjuke_song_get_type(song) != BOLKJUKE_FILE) {
            if (alt) {
                filelist_insert(song);
            }
        } else {
            filelist_add();
        }
        break;
    case KEY_RIGHT:
        if (filelist->selected == -1) break;
        if (bolkjuke_song_get_type(song) != BOLKJUKE_FILE) {
            filelist_enter();
        }
        break;
    case KEY_LEFT:
        if (filelist->selected == -1) break;
        if (bolkjuke_song_get_type(song) != BOLKJUKE_FILE) {
            filelist_leave();
        }
        break;
    default:
        list_input(filelist, c);
        break;
    }

    bolkjuke_song_release(song);

    filelist->prev_selected = filelist->selected;
}

void
playlist_delete()
{
    bolkjuke_remove(playlist->selected);
    list_draw(playlist);
}

void
playlist_move_up()
{
    if (playlist->selected == 0) return;

    bolkjuke_move(playlist->selected, playlist->selected - 1);
    list_driver(playlist, LIST_UP);
    list_draw(playlist);
}

void
playlist_move_down()
{
    if (playlist->selected == playlist->size - 1) return;

    bolkjuke_move(playlist->selected, playlist->selected + 1);
    list_driver(playlist, LIST_DOWN);
    list_draw(playlist);
}

void
playlist_input(int c)
{
    switch (c) {
    case KEY_DC:
        if (playlist->selected == -1) break;
        playlist_delete();
        break;
    case '-':
        if (playlist->selected == -1) break;
        playlist_move_up();
        break;
    case '+':
    case '=':
        if (playlist->selected == -1) break;
        playlist_move_down();
        break;
    default:
        list_input(playlist, c);
        break;
    }
}

void
input(int c)
{
    char *answer;

    switch (c) {
    case KEY_F(1):
    case KEY_F(4):
        if (bar_ask_yes_or_no(bar, "Exit"))
            running = 0;
        break;
    case KEY_F(2):
        if (bar_ask_yes_or_no(bar, "Clear playlist")) {
            bolkjuke_clear();
            list_draw(playlist);
        }
        break;
    case KEY_F(3):
        if ((answer = bar_ask(bar, "Search for")))
            bolkjuke_search(answer);
        free(answer);
        break;
        //case KEY_F(4):
        // TODO
        //break;
    case KEY_F(5):
        if (bar_ask_yes_or_no(bar, "Shuffle playlist")) {
            bolkjuke_shuffle();
            list_draw(playlist);
        }
        break;
    case KEY_F(6):
        if ((answer = bar_ask(bar, "Save playlist as")))
            bolkjuke_save(answer);
        free(answer);
        break;
    case KEY_F(7):
        bolkjuke_stop();
        break;
    case KEY_F(8):
        bolkjuke_play_pause();
        break;
    case KEY_F(9):
        bolkjuke_prev();
        break;
    case KEY_F(10):
        bolkjuke_fast_rewind();
        break;
    case KEY_F(11):
        bolkjuke_fast_forward();
        break;
    case KEY_F(12):
        bolkjuke_next();
        break;
    case '\t':
        activewin->active = FALSE;
        window_draw(activewin);
        if (activewin == filelistwin)
            activewin = playlistwin;
        else
            activewin = filelistwin;
        activewin->active = TRUE;
        window_draw(activewin);
        break;
    default:
        if (activewin == filelistwin)
            filelist_input(c);
        else if (activewin == playlistwin)
            playlist_input(c);
        break;
    }
}

int
main(int argc, char** argv)
{
    bolkjuke_new();

    initscr();
    curs_set(0);
    refresh();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    leaveok(stdscr, TRUE);
    nonl();
    start_color();

    activewin = filelistwin = window_new(LINES - 1, COLS / 2, 0, 0);
    filelistwin->active = TRUE;
    filelistwin->decorated = TRUE;
    filelistwin->title = "Mp3list";
    filelist = list_new(filelistwin);
    list_set(filelist, bolkjuke_get_filelist());
    window_draw(filelistwin);
    list_draw(filelist);

    playlistwin = window_new(LINES - 12, COLS - COLS / 2, 11, COLS / 2);
    playlistwin->decorated = TRUE;
    playlistwin->title = "Playlist";
    playlist = list_new(playlistwin);
    list_set(playlist, bolkjuke_get_playlist());
    window_draw(playlistwin);
    list_draw(playlist);

    playbackwin = window_new(6, COLS - COLS / 2, 0, COLS / 2);
    playbackwin->decorated = TRUE;
    playbackwin->title = "Playback";
    playback = info_new(playbackwin, 1);
    window_draw(playbackwin);
    info_draw(playback);

    infowin = window_new(5, COLS - COLS / 2, 6, COLS / 2);
    infowin->decorated = TRUE;
    infowin->title = "Info";
    info = info_new(infowin, 0);
    window_draw(infowin);
    info_draw(info);

    barwin = window_new(1, COLS, LINES - 1, 0);
    barwin->decorated = FALSE;
    bar = bar_new(barwin);
    window_draw(barwin);
    bar_draw(bar);

    doupdate();

    bolkjuke_attach(callback);

    running = 1;
    while (running) {
        input(getch());
        doupdate();
    }

    list_destroy(filelist);
    window_destroy(filelistwin);

    list_destroy(playlist);
    window_destroy(playlistwin);

    info_destroy(playback);
    window_destroy(playbackwin);

    info_destroy(info);
    window_destroy(infowin);

    bar_destroy(bar);
    window_destroy(barwin);

    wclear(stdscr);
    refresh();
    endwin();

    bolkjuke_destroy();

    exit(0);
}
