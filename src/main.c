#include "main.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <curses.h>

int
main(int argc, char** argv)
{
    bolkjuke_new();
    controller_init();
    gui_init();
    gui_loop();
    gui_destroy();
    controller_destroy();
    bolkjuke_destroy();

    exit(0);
}
