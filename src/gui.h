#ifndef GUI_H_
#define GUI_H_

#include "window.h"
#include "list.h"
#include "info.h"
#include "bar.h"

Window *activewin;

Window *filelistwin;
List *filelist;

Window *playlistwin;
List *playlist;

Window *playbackwin;
Info *playback;

Window *infowin;
Info *info;

Window *barwin;
Bar *bar;

void gui_init();
void gui_destroy();
void gui_loop();

#endif /* GUI_H_ */
