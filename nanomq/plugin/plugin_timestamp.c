#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "include/plugin.h"
#include "include/nanomq.h"
#include "nng/supplemental/nanolib/utils.h"
#include "nng/supplemental/util/platform.h"
#include "nng/supplemental/nanolib/cvector.h"
#include <dlfcn.h>

/*
 * Compile:
 * gcc -I.. -I../../nng/include -fPIC -shared plugin_timestamp.c -o plugin_timestamp.so
 */

int timestamp_cb(void *data)
{
    char **property = (char **)data;

    if (property == NULL) {
        log_debug("Property is NULL, return 0\n");
        return 0;
    }

    // Get current time in milliseconds
    struct timespec ts;
    unsigned long long milliseconds;

    clock_gettime(CLOCK_REALTIME, &ts);
    milliseconds = (unsigned long long)ts.tv_sec * 1000ULL +
                   ts.tv_nsec / 1000000ULL;

    // Convert to string
    char time_buf[32];
    snprintf(time_buf, sizeof(time_buf), "%llu", milliseconds);

    // Allocate and assign key/value
    property[0] = malloc(strlen("timestamp") + 1);
    strcpy(property[0], "timestamp");

    property[1] = malloc(strlen(time_buf) + 1);
    strcpy(property[1], time_buf);

    return 0;
}

int nano_plugin_init(void)
{
    
    if (plugin_hook_register(HOOK_USER_PROPERTY, timestamp_cb) != 0) {
        log_error("Failed to register hook\n");
        return -1;
    }
    
    return 0;
}
