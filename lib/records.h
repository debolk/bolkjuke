#ifndef BOLKJUKE_RECORDS_H_
#define BOLKJUKE_RECORDS_H_

#include "song_list.h"

void
bolkjuke_records_init();

void
bolkjuke_records_destroy();

bolkjuke_song_list_t *
bolkjuke_records_read(char *path);

bolkjuke_song_list_t *
bolkjuke_records_search(char *query);

void
bolkjuke_records_save(bolkjuke_song_list_t *list, char *path);

#endif /* BOLKJUKE_RECORDS_H_ */
