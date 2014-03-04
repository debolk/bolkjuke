#ifndef LIST_H
#define	LIST_H

#include <curses.h>

#include "lib/bolkjuke.h"

#include "window.h"

typedef struct {
    Window *window;
    int width;
    int height;
    int x;
    int y;
    WINDOW *win;
    bolkjuke_song_list_t *songlist;
    int size;
    int top;
    int selected;
    int prev_selected;
    int prev_added;
    int playing;
} List;

typedef enum {
    LIST_HOME = 256,
    LIST_END,
    LIST_UP,
    LIST_DOWN,
    LIST_PAGE_UP,
    LIST_PAGE_DOWN,
    LIST_PATTERN_DELETE,
    LIST_PATTERN_CLEAR
} ListDriver;

List *list_new(Window *window);
void list_destroy(List *list);
void list_set(List *list, bolkjuke_song_list_t *songlist);
void list_draw(List *list);
void list_driver(List *list, int c);
int list_get_selected(List *list);
bolkjuke_song_t *list_get_selected_song(List *list);

#endif	/* LIST_H */
