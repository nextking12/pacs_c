#include <stdio.h>
#include "storage.h"

int save_log(const AccessEvent *log, int count) {
    FILE *f = fopen(LOG_FILE, "wb");
    if (!f) {
        printf("Error: could not open file for writing.\n");
        return -1;
    }

    fwrite(&count, sizeof(int), 1, f);
    fwrite(log, sizeof(AccessEvent), count, f);
    fclose(f);
    printf("Log saved (%d events).\n", count);
    return 0;
}

int load_log(AccessEvent *log, int max_events) {
    FILE *f = fopen(LOG_FILE, "rb");
    if (!f) {
        printf("No existing log file found. Starting fresh.\n");
        return 0;
    }

    int count = 0;
    fread(&count, sizeof(int), 1, f);
    if (count > max_events) count = max_events;
    fread(log, sizeof(AccessEvent), count, f);
    fclose(f);
    printf("Loaded %d events from log file.\n", count);
    return count;
}