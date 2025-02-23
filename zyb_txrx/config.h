// config.h
#ifndef CONFIG_H
#define CONFIG_H

// Configuration struct
typedef struct {
    char source_callsign[50];
    char dest_callsign[50];
    int wait_time_sec;
} Config;

// Function to load configuration from a file
Config load_config(const char *filename);

#endif // CONFIG_H

