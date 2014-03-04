#ifndef BOLKJUKE_EVENT_H_
#define BOLKJUKE_EVENT_H_

typedef enum {
    BOLKJUKE_EVENT_STOPPED,
    BOLKJUKE_EVENT_PLAYING,
    BOLKJUKE_EVENT_PAUSED,
} bolkjuke_event_type_t;

typedef struct {
    bolkjuke_event_type_t type;

    struct {
    } stopped;

    struct {
        int index;
    } playing;

    struct {
    } paused;
} bolkjuke_event_t;

typedef void (*bolkjuke_callback_t)(bolkjuke_event_t *event);

void
bolkjuke_events_init();

void
bolkjuke_events_destroy();

void
bolkjuke_events_attach(bolkjuke_callback_t cb);

void
bolkjuke_events_callback(bolkjuke_event_t *event);

#endif /* BOLKJUKE_EVENT_H_ */
