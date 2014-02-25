#include <stdlib.h>
#include <curses.h>

#include "list.h"

List *
list_new(Window *window)
{
    List *list = calloc(1, sizeof (List));

    list->window = window;
    list->height = list->window->height - 2;
    list->width = list->window->width - 4;
    list->y = 1;
    list->x = 2;
    list->win = derwin(list->window->win,
            list->height, list->width, list->y, list->x);

    return (list);
}

void
list_destroy(List *list)
{
    if (list->songlist)
        bolkjuke_song_list_release(list->songlist);

    delwin(list->win);

    free(list);
}

void
list_normalize(List *list)
{
    list->size = bolkjuke_song_list_get_size(list->songlist);

    if (list->size) {
        if (list->selected < 0)
            list->selected = 0;
        else if (list->selected > list->size - 1)
            list->selected = list->size - 1;
        if (list->selected < list->top)
            list->top = list->selected;
        else if (list->selected > list->top + list->height - 1)
            list->top = list->selected - list->height + 1;
        if (list->top > list->size - list->height)
            list->top = list->size - list->height;
        if (list->selected < 0)
            list->selected = 0;
        if (list->top < 0)
            list->top = 0;
    } else {
        list->selected = -1;
        list->top = 0;
    }
}

void
list_set(List *list, bolkjuke_song_list_t *songlist)
{
    if (list->songlist)
        bolkjuke_song_list_release(list->songlist);

    list->songlist = songlist;
    list->top = 0;
    list->selected = 0;
    list->prev_selected = -1;
    list->prev_added = -1;
    list->playing = -1;
}

void
list_draw(List *list)
{
    int i;
    int bottom = list->top + list->height;
    char *name;
    bolkjuke_song_t *song;

    wclear(list->win);
    wmove(list->win, 0, 0);

    list_normalize(list);

    for (i = list->top; i < bottom && i < list->size; i++) {
        song = bolkjuke_song_list_get(list->songlist, i);

        name = bolkjuke_song_get_name(song);
        if (i == list->selected)
            wattron(list->win, A_REVERSE);
        if (bolkjuke_song_get_type(song) || i == list->playing)
            wattron(list->win, A_BOLD);
        mvwaddnstr(list->win, i, 0, name, list->width);
        wattroff(list->win, A_REVERSE | A_BOLD);

        bolkjuke_song_release(song);
    }

    wsyncup(list->win);

    window_update(list->window);
}

int
list_get_selected(List *list)
{
    list_normalize(list);

    return (list->selected);
}

bolkjuke_song_t *
list_get_selected_song(List *list)
{
    list_normalize(list);

    return (bolkjuke_song_list_get(list->songlist, list->selected));
}

void
list_driver(List *list, int c)
{
    switch (c) {
    case LIST_HOME:
        list->selected = 0;
        break;
    case LIST_END:
        list->selected = bolkjuke_song_list_get_size(list->songlist) - 1;
        break;
    case LIST_UP:
        list->selected--;
        break;
    case LIST_DOWN:
        list->selected++;
        break;
    case LIST_PAGE_UP:
        list->selected -= list->height / 2;
        list->top = list->selected - list->height / 2;
        break;
    case LIST_PAGE_DOWN:
        list->selected += list->height / 2;
        list->top = list->selected - list->height / 2;
        break;
    case '0'...'9':
    case 'a'...'z':
    case 'A'...'Z':
    case ' ':
    case '-':
        // TODO
        break;
    case LIST_PATTERN_DELETE: // backspace
        // TODO
        break;
    case LIST_PATTERN_CLEAR:
        // TODO
        break;
    }
}
