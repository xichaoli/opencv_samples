#include <cstddef>
#include <cstdio>
#include <cstdlib>

#include <mpv/client.h>

static inline void check_error(int status)
{
    if (status < 0) {
        printf("mpv API error: %s\n", mpv_error_string(status));
        exit(1);
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        printf("pass a single media file as argument\n");
        return 1;
    }

    mpv_handle *ctx = mpv_create();
    if (!ctx) {
        printf("failed creating context\n");
        return 1;
    }

    // Enable default key bindings, so the user can actually interact with
    // the player (and e.g. close the window).
    check_error(mpv_set_option_string(ctx, "input-default-bindings", "yes"));
    check_error(mpv_set_option_string(ctx, "input-vo-keyboard", "yes"));
    check_error(mpv_set_option_string(ctx, "hwdec", "vaapi"));
    check_error(mpv_set_option_string(ctx, "demuxer-lavf-o", "input_format=mjpeg,video_size=1920x1080,framerate=30"));
    check_error(mpv_set_option_string(ctx, "vo", "gpu"));
    check_error(mpv_set_option_string(ctx, "profile", "low-latency"));
    check_error(mpv_set_option_string(ctx, "cache", "yes"));
    check_error(mpv_set_option_string(ctx, "untimed", "yes"));
    int val = 1;
    check_error(mpv_set_option(ctx, "osc", MPV_FORMAT_FLAG, &val));

    // Done setting up options.
    check_error(mpv_initialize(ctx));

    // Play this file.
    const char *cmd[] = {"loadfile", argv[1], nullptr};
    check_error(mpv_command(ctx, cmd));

    // Let it play, and wait until the user quits.
    while (true) {
        mpv_event *event = mpv_wait_event(ctx, 10000);
        printf("event: %s\n", mpv_event_name(event->event_id));
        if (event->event_id == MPV_EVENT_SHUTDOWN)
            break;
    }

    mpv_terminate_destroy(ctx);
    return 0;
}
