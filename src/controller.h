#ifndef CONTROLLER_H_
#define CONTROLLER_H_

void controller_init();
void controller_destroy();
int controller_is_running();
void controller_exit();
void controller_play_pause_playback();
void controller_stop_playback();
void controller_play_next();
void controller_play_previous();
void controller_fast_forward_playback();
void controller_fast_rewind_playback();
void controller_open_record();
void controller_close_record();
void controller_search_records();
void controller_add_song();
void controller_insert_song();
void controller_add_song_recursive();
void controller_insert_song_recursive();
void controller_delete_song();
void controller_move_song_up();
void controller_move_song_down();
void controller_save_playlist();
void controller_clear_playlist();
void controller_shuffle_playlist();

#endif /* CONTROLLER_H_ */
