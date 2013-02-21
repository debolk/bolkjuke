#ifndef BAR_H
#define	BAR_H

#include "window.h"

typedef struct {
    Window *window;
    WINDOW *win;
} Bar;

Bar *bar_new(Window *window);
void bar_destroy(Bar *bar);
void bar_draw(Bar *bar);
char *bar_ask(Bar *bar, char *question);
int bar_ask_yes_or_no(Bar *bar, char *question);

#endif	/* BAR_H */
