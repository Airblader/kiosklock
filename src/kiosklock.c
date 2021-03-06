// vim:ts=4:sw=4:expandtab
#include "all.h"
#include <getopt.h>
#include <unistd.h>
#include <sys/wait.h>

#ifndef __VERSION
#define __VERSION "unknown"
#endif

xcb_connection_t *conn;
typedef void (*callback)(void);

Config config = {
    .fork = false
};

/* Forward declarations */
static void run(void);
static void at_exit_cb(void);
static void parse_args(int argc, char *argv[]);
static void safe_fork(callback cb);
static void print_usage(void);

int main(int argc, char *argv[]) {
    atexit(at_exit_cb);
    parse_args(argc, argv);

    if (config.fork) {
        safe_fork(run);
    } else {
        run();
    }
}

static void run(void) {
    int screens;
    conn = xcb_connect(NULL, &screens);
    if (conn == NULL || xcb_connection_has_error(conn))
        errx(EXIT_FAILURE, "Failed to connect to X11.\n");
}

static void at_exit_cb(void) {
    if (conn != NULL)
        xcb_disconnect(conn);
}

static void parse_args(int argc, char *argv[]) {
    int c,
        opt_index = 0;
    static struct option long_options[] = {
        { "version", no_argument, 0, 'v' },
        { "help", no_argument, 0, 'h' },
        { "background", no_argument, 0, 'b' },
        { 0, 0, 0, 0 }
    };

    while ((c = getopt_long(argc, argv, "bvh", long_options, &opt_index)) != -1) {
        switch (c) {
            case 0:
                /* Example for a long-named option.
                if (strcmp(long_options[opt_index].name, "parameter") == 0) {
                    break;
                }
                */

                print_usage();
                break;
            case 'b':
                config.fork = true;
                break;
            case 'v':
                fprintf(stderr, "kiosklock version %s\n", __VERSION);
                exit(EXIT_SUCCESS);
                break;
            case 'h':
            default:
                print_usage();
                break;
        }
    }
}

static void safe_fork(callback cb) {
    pid_t pid = fork();
    if (!pid) {
        if (!fork()) {
            (*cb)();
        } else {
            exit(EXIT_SUCCESS);
        }
    } else {
        waitpid(pid, NULL, 0);
    }
}

static void print_usage(void) {
    fprintf(stderr, "Usage: kiosklock [-b] [-v|--version] [-h|--help]");
    fprintf(stderr, "\n");
    exit(EXIT_FAILURE);
}
