#ifndef BOLKJUKE_H
#define	BOLKJUKE_H

#include "lib/bolkjuke.h"

#include "settings.h"
#include "window.h"
#include "list.h"
#include "info.h"
#include "bar.h"

int running;

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

#endif	/* BOLKJUKE_H */
