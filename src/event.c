// vim:ts=4:sw=4:expandtab
#include "all.h"

static struct ev_loop *loop;
static ev_io x_watcher;
static ev_check x_check;

/* Forward declarations */
static void event_enter_x_loop(void);
static void xcb_cb(EV_P_ ev_io *w, int revents);
static void xcb_check_cb(EV_P_ ev_check *w, int revents);

void event_enter_loop(void) {
    loop = EV_DEFAULT;
    event_enter_x_loop();
    ev_run(loop, 0);
}

static void event_enter_x_loop(void) {
    ev_io_init(&x_watcher, xcb_cb, xcb_get_file_descriptor(conn), EV_READ);
    ev_io_start(loop, &x_watcher);

    ev_check_init(&x_check, xcb_check_cb);
    ev_check_start(loop, &x_check);

    // TODO add ev_prepare

    /* Run the loop once to get all events we already received. */
    ev_invoke(loop, xcb_check_cb, 0);
    ev_loop(loop, 0);
}

static void xcb_cb(EV_P_ ev_io *w, int revents) {
    // TODO
}

static void xcb_check_cb(EV_P_ ev_check *w, int revents) {
    // TODO
}
