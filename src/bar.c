#include <config.h>

#include <stdlib.h>
#include <string.h>
#include <curses.h>

#include "bar.h"

Bar *
bar_new(Window *window)
{
    Bar *bar = calloc(1, sizeof (Bar));

    bar->window = window;
    bar->win = window->win;

    return (bar);
}

void
bar_destroy(Bar *bar)
{
    free(bar);
}

void
bar_draw(Bar *bar)
{

    wclear(bar->win);
    wmove(bar->win, 0, 0);

    waddstr(bar->win, "BolkJuke v" VERSION);

    wsyncup(bar->win);

    window_update(bar->window);
}

char *
bar_ask(Bar *bar, char *question)
{
    char *answer = malloc(129);

    wmove(bar->win, 0, 0);
    waddstr(bar->win, question);
    waddstr(bar->win, ": ");

    echo();
    wgetnstr(bar->win, answer, 128);
    noecho();

    wclear(bar->win);
    wrefresh(bar->win);

    if (!strlen(answer) > 0) {
        free(answer);
        answer = NULL;
    }

    return (answer);
}

int
bar_ask_yes_or_no(Bar *bar, char *question)
{
    int c;

    wmove(bar->win, 0, 0);
    waddstr(bar->win, question);
    waddstr(bar->win, "? (y/n): ");

    c = wgetch(bar->win);

    wclear(bar->win);
    wrefresh(bar->win);

    return (c == 'y' || c == 'Y');
}
