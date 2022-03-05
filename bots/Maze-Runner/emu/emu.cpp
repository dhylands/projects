/****************************************************************************
 *
 *   Copyright (c) 2022 Dave Hylands     <dhylands@gmail.com>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License version 2 as
 *   published by the Free Software Foundation.
 *
 *   Alternatively, this software may be distributed under the terms of BSD
 *   license.
 *
 *   See README and COPYING for more details.
 *
 ****************************************************************************/
/**
 *
 *   @file   emu.cpp
 *
 *   @brief  This program is used to emulate the robot for emulating the serial
 *           comms using a socket connection.
 *
 *           The robot acts like a server and waits for incoming connections.
 *
 *****************************************************************************/

/* ---- Include Files ----------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <getopt.h>
#include <netdb.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "DumpMem.h"
#include "Log.h"
#include "BioloidGadget.h"

/* ---- Public Variables -------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------- */

constexpr in_port_t DEFAULT_PORT = 8888;

enum {
    // Options assigned a single character code can use that charater code
    // as a short option.

    OPT_DEBUG = 'd',
    OPT_PORT = 'p',
    OPT_VERBOSE = 'v',
    OPT_HELP = 'h',

    // Options from this point onwards don't have any short option equivalents

    OPT_FIRST_LONG_OPT = 0x80,
};

/* ---- Private Variables ------------------------------------------------- */

static const char* g_pgm_name;

struct option g_long_option[] = {
    // option       A  Flag   V  (has_arg, flag, val)
    // -----------  -  ----  ---
    {"debug", 0, NULL, OPT_DEBUG},
    {"help", 0, NULL, OPT_HELP},
    {"port", 0, NULL, OPT_PORT},
    {"verbose", 0, NULL, OPT_VERBOSE},
    {0},
};

int g_verbose = 0;
int g_debug = 0;

int g_listen_socket;
int g_socket;

Bioloid::Gadget g_gadget(Bioloid::as_ID(0), 20, 10);

/* ---- Private Function Prototypes --------------------------------------- */

static void usage(void);

/* ---- Functions --------------------------------------------------------- */

//***************************************************************************
/**
 *   Main program
 */

int main(int argc, char** argv) {
    in_port_t port = DEFAULT_PORT;

    // Figure out which directory our executable came from

    g_pgm_name = basename(argv[0]);

    // Figure out the short options from our options structure
    //
    // The * 2 is to allow for the case that all of the short arguments
    // require an argument (and hence take up 2 characters in the short string)
    //
    // The + 1 is for the trailing null character.

    char short_opts_str[sizeof(g_long_option) / sizeof(g_long_option[0]) * 2 + 1];
    char* short_opts = short_opts_str;
    struct option* scan_opt;
    int opt;

    for (scan_opt = g_long_option; scan_opt->name != NULL; scan_opt++) {
        if ((scan_opt->flag == NULL) && (scan_opt->val < OPT_FIRST_LONG_OPT)) {
            *short_opts++ = (char)scan_opt->val;

            if (scan_opt->has_arg != no_argument) {
                *short_opts++ = ':';
            }
        }
    }
    *short_opts++ = '\0';

    // Parse the command line options

    while ((opt = getopt_long(argc, argv, "dhv", g_long_option, NULL)) > 0) {
        switch (opt) {
            case OPT_DEBUG: {
                g_debug = true;
                break;
            }

            case OPT_PORT: {
                const char* port_str = optarg;
                port = atoi(port_str);
                break;
            }

            case OPT_VERBOSE: {
                g_verbose = true;
                break;
            }
            case '?':
            case OPT_HELP: {
                usage();
                return 1;
            }
        }
    }

    if (g_verbose) {
        Log("g_debug = %d\n", g_debug);
        Log("port = %d\n", port);
    }

    g_gadget.InitializeControlTable();

    if ((g_listen_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        LogError("Call to socket failed: '%s'\n", strerror(errno));
        exit(1);
    }
    int enable = 1;
    if (setsockopt(g_listen_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) < 0) {
        LogError("Failed to set REUSEADDR socket option: %s\n", strerror(errno));
        exit(1);
    }

    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    server.sin_port = htons(port);
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_family = AF_INET;

    if (bind(g_listen_socket, (const sockaddr*)&server, sizeof(server)) < 0) {
        LogError("Failed to bind to port: %d: %s\n", port, strerror(errno));
        exit(1);
    }

    Log("Listening on port %d ...\n", port);
    if (listen(g_listen_socket, 1) < 0) {
        LogError("Failed to listen for incoming connection: %s\n", strerror(errno));
    }

    struct sockaddr_in client;
    memset(&client, 0, sizeof(client));
    socklen_t client_len = sizeof(client);
    if ((g_socket = accept(g_listen_socket, (sockaddr*)&client, &client_len)) < 0) {
        LogError("Failed to accept incoming connection: %s\n", strerror(errno));
        exit(1);
    }
    printf("Accepted connection from %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

    ssize_t bytesRcvd;
    uint8_t buf[1024];
    while ((bytesRcvd = recv(g_socket, buf, sizeof(buf), 0)) > 0) {
        DumpMem("R", 0, buf, bytesRcvd);
        g_gadget.ProcessBytes(buf, bytesRcvd);
    }

    close(g_socket);
    close(g_listen_socket);

    if (g_verbose) {
        Log("Done\n");
    }

    exit(0);
    return 0;  // Get rid of warning about not returning anything
}  // main

//***************************************************************************
/**
 *  usage
 */

void usage() {
    Log("Usage: %s [option(s)] host port\n", g_pgm_name);
    Log("\n");
    Log("Connect to a network port\n");
    Log("\n");
    Log("  -d, --debug       Turn on debug output\n");
    Log("  -h, --help        Display this message\n");
    Log("  -v, --verbose     Turn on verbose messages\n");
}
