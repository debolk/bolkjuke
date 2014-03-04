#ifndef INFO_H
#define	INFO_H

#include <curses.h>

#include "lib/bolkjuke.h"

#include "window.h"

typedef struct {
    Window *window;
    int width;
    int height;
    int x;
    int y;
    int yoffset;
    WINDOW *win;
    bolkjuke_song_t *song;
} Info;

Info *info_new(Window *window, int yoffset);
void info_destroy(Info *info);
void info_set(Info *info, bolkjuke_song_t *song);
void info_draw(Info *info);

#endif	/* INFO_H */
