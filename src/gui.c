#include "gui.h"
#include "controller.h"

#include <ncurses.h>

void
gui_init()
{
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
}

void
gui_destroy()
{
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
}

void
gui_loop()
{
    while (controller_is_running()) {
        input(getch());
        doupdate();
    }
}
