#define _POSIX_C_SOURCE 200809L

// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Ali Rajabi Nekoo

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifndef RUN_NVIDIA_VERSION
#define RUN_NVIDIA_VERSION "dev"
#endif

static const char *const PROGRAM_NAME = "run-nvidia";

static void print_usage(FILE *stream) {
    fprintf(stream,
            "Usage: %s [--] <command> [args...]\n"
            "       %s --help\n"
            "       %s --version\n"
            "\n"
            "Run a command with NVIDIA PRIME Render Offload enabled.\n"
            "\n"
            "Options:\n"
            "  -h, --help       Show this help message\n"
            "  -V, --version    Show version information\n"
            "  --               Stop option parsing\n",
            PROGRAM_NAME, PROGRAM_NAME, PROGRAM_NAME);
}

static int set_environment_variable(const char *name, const char *value) {
    if (setenv(name, value, 1) == -1) {
        fprintf(stderr, "%s: failed to set %s: %s\n",
                PROGRAM_NAME, name, strerror(errno));
        return -1;
    }

    return 0;
}

int main(int argc, char *argv[]) {
    int command_index = 1;

    if (argc == 2 &&
        (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)) {
        print_usage(stdout);
        return 0;
    }

    if (argc == 2 &&
        (strcmp(argv[1], "-V") == 0 || strcmp(argv[1], "--version") == 0)) {
        printf("%s %s\n", PROGRAM_NAME, RUN_NVIDIA_VERSION);
        return 0;
    }

    if (argc > 1 && strcmp(argv[1], "--") == 0) {
        command_index = 2;
    }

    if (argc <= command_index) {
        print_usage(stderr);
        return 2;
    }

    if (set_environment_variable("__NV_PRIME_RENDER_OFFLOAD", "1") == -1 ||
        set_environment_variable("__GLX_VENDOR_LIBRARY_NAME", "nvidia") == -1 ||
        set_environment_variable("__VK_LAYER_NV_optimus", "NVIDIA_only") == -1) {
        return 1;
    }

    execvp(argv[command_index], &argv[command_index]);

    const int error_number = errno;
    fprintf(stderr, "%s: failed to execute '%s': %s\n",
            PROGRAM_NAME, argv[command_index], strerror(error_number));

    return error_number == ENOENT ? 127 : 126;
}
