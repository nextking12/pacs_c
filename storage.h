#ifndef STORAGE_H
#define STORAGE_H

#include "events.h"

#define LOG_FILE "access_log.dat"

// Save entire log to binary file, returns 0 on success
int save_log(const AccessEvent *log, int count);

// Load log from binary file, returns number of events loaded
int load_log(AccessEvent *log, int max_events);

#endif