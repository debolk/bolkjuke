#include "bolkjuke.h"
#include "engine.h"

#include <xine.h>

static xine_t *xine;
static xine_stream_t *stream;
static xine_video_port_t *vo_port;
static xine_audio_port_t *ao_port;
static xine_event_queue_t *event_queue;
static xine_stream_t *meta_stream;
static xine_video_port_t *meta_vo_port;
static xine_audio_port_t *meta_ao_port;

static void event_listener(void *user_data, const xine_event_t *event);

void
bolkjuke_engine_init()
{
    xine = xine_new();
    xine_init(xine);

    ao_port = xine_open_audio_driver(xine, "auto", NULL );
    vo_port = xine_open_video_driver(xine, NULL, XINE_VISUAL_TYPE_NONE, NULL );
    stream = xine_stream_new(xine, ao_port, vo_port);

    event_queue = xine_event_new_queue(stream);
    xine_event_create_listener_thread(event_queue, event_listener, NULL );

    meta_ao_port = xine_open_audio_driver(xine, NULL, NULL );
    meta_vo_port = xine_open_video_driver(xine, NULL, XINE_VISUAL_TYPE_NONE, NULL );
    meta_stream = xine_stream_new(xine, meta_ao_port, meta_vo_port);
}

static void
event_listener(void *user_data, const xine_event_t *event)
{
    bolkjuke_log("xine event: %d", event->type);
    switch (event->type) {
    case XINE_EVENT_UI_PLAYBACK_FINISHED:
        bolkjuke_next();
        break;
    }
}

void
bolkjuke_engine_destroy()
{
    xine_close(stream);
    xine_event_dispose_queue(event_queue);
    xine_dispose(stream);
    xine_close_audio_driver(xine, ao_port);
    xine_close_video_driver(xine, vo_port);

    xine_dispose(meta_stream);
    xine_close_audio_driver(xine, meta_ao_port);
    xine_close_video_driver(xine, meta_vo_port);

    xine_exit(xine);
}

void
bolkjuke_engine_open(char *mrl)
{
    xine_open(stream, mrl);
}

void
bolkjuke_engine_play()
{
    xine_play(stream, 0, 0);
}

void
bolkjuke_engine_pause()
{
    xine_set_param(stream, XINE_PARAM_SPEED, XINE_SPEED_PAUSE);
}

void
bolkjuke_engine_resume()
{
    xine_set_param(stream, XINE_PARAM_SPEED, XINE_SPEED_NORMAL);
}

void
bolkjuke_engine_stop()
{
    xine_stop(stream);
}
