#include <stdlib.h>
#include <curses.h>

#include "info.h"

Info *
info_new(Window *window, int yoffset)
{
    Info *info = calloc(1, sizeof (Info));

    info->window = window;
    info->yoffset = yoffset;
    info->height = 3;
    info->width = info->window->width - 4;
    info->y = 1 + info->yoffset;
    info->x = 2;
    info->win = derwin(info->window->win,
            info->height, info->width, info->y, info->x);

    return (info);
}

void
info_destroy(Info *info)
{
    if (info->song)
        bolkjuke_song_release(info->song);

    delwin(info->win);

    free(info);
}

void
info_set(Info *info, bolkjuke_song_t *song)
{
    if (info->song)
        bolkjuke_song_release(info->song);

    info->song = song;
}

void
info_draw(Info *info)
{
    char *title = "";
    char *artist = "";
    char *album = "";

    if (info->song) {
        title = bolkjuke_song_get_title(info->song);
        artist = bolkjuke_song_get_artist(info->song);
        album = bolkjuke_song_get_album(info->song);
    }

    wclear(info->win);
    wmove(info->win, 0, 0);

    waddstr(info->win, "Title : ");
    waddnstr(info->win, title, info->width - 8);
    waddch(info->win, '\n');
    waddstr(info->win, "Artist: ");
    waddnstr(info->win, artist, info->width - 8);
    waddch(info->win, '\n');
    waddstr(info->win, "Album : ");
    waddnstr(info->win, album, info->width - 8);

    wsyncup(info->win);

    window_update(info->window);
}
