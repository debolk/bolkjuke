#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <curses.h>

#include "window.h"

Window *
window_new(int height, int width, int y, int x)
{
    Window *window = calloc(1, sizeof (Window));
    window->height = height;
    window->width = width;
    window->y = y;
    window->x = x;
    window->win = newwin(window->height, window->width, window->y, window->x);

    return (window);
}

void
window_destroy(Window *window)
{
    delwin(window->win);
    free(window);
}

void
window_draw(Window *window)
{
    int len;
    int offset;

    assert(window);
    assert(window->win);

    if (window->decorated) {
        if (window->active) {
            wattron(window->win, A_BOLD);
        } else {
            wattroff(window->win, A_BOLD);
        }
        box(window->win, 0, 0);
        len = strlen(window->title);
        len = len > window->width - 6 ? window->width : len;
        offset = (window->width - len) / 2;
        wmove(window->win, 0, offset);
        waddstr(window->win, "[ ");
        waddnstr(window->win, window->title, len);
        waddstr(window->win, " ]");
    }
    
    window_update(window);
}

void
window_update(Window *window)
{
    wnoutrefresh(window->win);
}
