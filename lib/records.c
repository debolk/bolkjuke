#include "records.h"
#include "song.h"
#include "song_list.h"

#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

void
bolkjuke_records_init()
{
}

void
bolkjuke_records_destroy()
{
}

static int scandir_select(const struct dirent *d);
static int scandir_compare(const struct dirent **d1, const struct dirent **d2);

bolkjuke_song_list_t *
bolkjuke_records_read(char *path)
{
    bolkjuke_song_list_t *songlist;
    bolkjuke_song_t *song;
    char *extension;
    struct dirent **dirents;
    int num_dirents;
    int i;

    bolkjuke_log("read: %s", path);

    songlist = bolkjuke_song_list_new();
    songlist->path = strdup(path);
    num_dirents = scandir(path, &dirents, scandir_select, scandir_compare);
    bolkjuke_song_list_grow(songlist, num_dirents);

    for (i = 0; i < num_dirents; i++) {
        song = bolkjuke_song_new();
        asprintf(&song->path, "%s/%s", path, dirents[i]->d_name);

        if (dirents[i]->d_type & DT_DIR) {
            song->type = BOLKJUKE_DIRECTORY;
            song->name = strdup(dirents[i]->d_name);
        } else /*if (dirents[i]->d_type & DT_REG)*/ {
            song->type = BOLKJUKE_FILE;
            extension = strrchr(dirents[i]->d_name, '.');
            song->name = strndup(dirents[i]->d_name, extension - dirents[i]->d_name);
            // TODO get metadata
        }

        bolkjuke_song_list_append(songlist, song);
        bolkjuke_song_release(song);
    }

    return (songlist);
}

static int
scandir_select(const struct dirent *d)
{
    char *extension;

    if (d->d_type == DT_DIR) {
        if (d->d_name[0] != '.' || strcmp(d->d_name, "..") == 0) {
            return (1);
        }
    } else if (d->d_type == DT_REG) {
        extension = strrchr(d->d_name, '.');
        if (extension && strcasestr("wav mp3 acc ogg flac", ++extension)) {
            return (1);
        }
    }

    return (0);
}

static int
scandir_compare(const struct dirent **d1, const struct dirent **d2)
{
    if (strcmp((*d1)->d_name, "..") == 0) {
        return (-1);
    } else if (strcmp((*d2)->d_name, "..") == 0) {
        return (1);
    }

    if ((*d1)->d_type & DT_DIR) {
        if ((*d2)->d_type & DT_DIR) {
            return (strcasecmp((*d1)->d_name, (*d2)->d_name));
        } else {
            return (-1);
        }
    } else {
        if ((*d2)->d_type & DT_DIR) {
            return (1);
        } else {
            return (strcasecmp((*d1)->d_name, (*d2)->d_name));
        }
    }
}

bolkjuke_song_list_t *
bolkjuke_records_search(char *query)
{
    bolkjuke_song_list_t *results = bolkjuke_song_list_new();

    // TODO

    return (results);
}

void
bolkjuke_records_save(bolkjuke_song_list_t *list, char *path)
{
    // TODO
}
