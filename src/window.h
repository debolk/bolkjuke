#ifndef WINDOW_H
#define	WINDOW_H

#include <curses.h>

typedef struct {
    int width;
    int height;
    int x;
    int y;
    bool active;
    char *title;
    bool decorated;
    WINDOW *win;
} Window;

Window *window_new();
void window_init(Window *window);
void window_destroy(Window *window);
void window_draw(Window *window);
void window_update(Window *window);

#endif	/* WINDOW_H */
