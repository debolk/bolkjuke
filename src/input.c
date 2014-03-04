#include "input.h"
#include "gui.h"

#include <stdlib.h>
#include <ncurses.h>

void
input(int c)
{
    char *answer;

    switch (c) {
    case KEY_F(1):
        controller_exit();
        break;
    case KEY_F(2):
        controller_clear_playlist();
        break;
    case KEY_F(3):
        controller_search_records();
        break;
     case KEY_F(4):
        // TODO
        break;
    case KEY_F(5):
        controller_shuffle_playlist();
        break;
    case KEY_F(6):
        controller_save_playlist();
        break;
    case KEY_F(7):
        controller_stop_playback();
        break;
    case KEY_F(8):
        controller_play_pause_playback();
        break;
    case KEY_F(9):
        controller_play_previous();
        break;
    case KEY_F(10):
        controller_fast_rewind_playback();
        break;
    case KEY_F(11):
        controller_fast_forward_playback();
        break;
    case KEY_F(12):
        controller_play_next();
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
            input_filelist(c);
        else if (activewin == playlistwin)
            input_playlist(c);
        break;
    }
}

void
input_filelist(int c)
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
                controller_add_song_recursive();
            } else {
                controller_open_record();
            }
        } else {
            controller_add_song();
        }
        break;

    case KEY_IC:
        if (filelist->selected == -1) break;
        if (bolkjuke_song_get_type(song) != BOLKJUKE_FILE) {
            if (alt) {
                controller_insert_song_recursive(song);
            }
        } else {
            controller_add_song();
        }
        break;
    case KEY_RIGHT:
        if (filelist->selected == -1) break;
        if (bolkjuke_song_get_type(song) != BOLKJUKE_FILE) {
            controller_open_record();
        }
        break;
    case KEY_LEFT:
        if (filelist->selected == -1) break;
        if (bolkjuke_song_get_type(song) != BOLKJUKE_FILE) {
            controller_close_record();
        }
        break;
    default:
        input_list(filelist, c);
        break;
    }

    bolkjuke_song_release(song);

    filelist->prev_selected = filelist->selected;
}

void
input_playlist(int c)
{
    switch (c) {
    case KEY_DC:
        if (playlist->selected == -1) break;
        controller_delete_song();
        break;
    case '-':
        if (playlist->selected == -1) break;
        controller_move_song_up();
        break;
    case '+':
    case '=':
        if (playlist->selected == -1) break;
        controller_move_song_down();
        break;
    default:
        input_list(playlist, c);
        break;
    }
}

void
input_list(List *list, int c)
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
