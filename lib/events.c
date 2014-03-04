#include "events.h"

#include <assert.h>
#include <stdlib.h>

static bolkjuke_callback_t callback;

void
bolkjuke_events_init()
{
}

void
bolkjuke_events_destroy()
{
}

void
bolkjuke_events_attach(bolkjuke_callback_t cb)
{
    assert(callback == NULL); // TODO

    callback = cb;
}

void
bolkjuke_events_callback(bolkjuke_event_t *event)
{
    if (callback) {
        callback(event);
    }
}
