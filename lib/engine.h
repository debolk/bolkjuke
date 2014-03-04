#ifndef BOLKJUKE_ENGINE_H_
#define BOLKJUKE_ENGINE_H_

void
bolkjuke_engine_init();

void
bolkjuke_engine_destroy();

void
bolkjuke_engine_open(char *mrl);

void
bolkjuke_engine_play();

void
bolkjuke_engine_pause();

void
bolkjuke_engine_resume();

void
bolkjuke_engine_stop();

#endif /* BOLKJUKE_ENGINE_H_ */
