#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"

char *trim_whitespace(char *str) {
    char *end;

    while (*str == ' ') str++;

    if (*str == 0) return str;

    end = str + strlen(str) - 1;
    while (end > str && (*end == ' ' || *end == '\n' || *end == '\r')) end--;

    *(end + 1) = 0;
    return str;
}

Config load_config(const char *filename) {
    Config config;
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Could not open %s\n", filename);
        exit(1);
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char *key = strtok(line, "=");
        char *value = strtok(NULL, "\n");
        if (key && value) {
            key = trim_whitespace(key);
            value = trim_whitespace(value);

            if (strcmp(key, "SOURCE_CALLSIGN") == 0) {
                strncpy(config.source_callsign, value, sizeof(config.source_callsign) - 1);
            } else if (strcmp(key, "DEST_CALLSIGN") == 0) {
                strncpy(config.dest_callsign, value, sizeof(config.dest_callsign) - 1);
            } else if (strcmp(key, "WAIT_TIME_SEC") == 0) {
                config.wait_time_sec = atoi(value);
            }
        }
    }

    fclose(file);
    return config;
}

