#include <stdio.h>
#include <string.h>
#include "events.h"

int add_event(AccessEvent *log, int *count, EventType type,
              const char *door_id, const char *credential_id) {
    if (*count >= MAX_EVENTS) {
        printf("Log full. Cannot add event.\n");
        return -1;
    }

    AccessEvent *e = &log[*count];
    e->event_id = *count + 1;
    e->type = type;
    strncpy(e->door_id, door_id, DOOR_ID_LEN - 1);
    e->door_id[DOOR_ID_LEN - 1] = '\0';  // ensure null termination
    strncpy(e->credential_id, credential_id, CRED_ID_LEN - 1);
    e->credential_id[CRED_ID_LEN - 1] = '\0';
    e->timestamp = time(NULL);

    (*count)++;
    return e->event_id;
}

const char *event_type_string(EventType type) {
    switch (type) {
        case GRANTED:     return "GRANTED";
        case DENIED:      return "DENIED";
        case DOOR_FORCED: return "DOOR FORCED";
        case DOOR_HELD:   return "DOOR HELD";
        default:          return "UNKNOWN";
    }
}

void print_event(const AccessEvent *event) {
    char time_buf[26];
    // ctime_r is thread-safe version of ctime
    ctime_r(&event->timestamp, time_buf);
    time_buf[24] = '\0';  // strip newline ctime adds

    printf("[%04d] %-12s | Door: %-15s | Cred: %-15s | %s\n",
           event->event_id,
           event_type_string(event->type),
           event->door_id,
           event->credential_id,
           time_buf);
}

void print_log(const AccessEvent *log, int count) {
    if (count == 0) {
        printf("No events logged.\n");
        return;
    }
    printf("\n--- ACCESS EVENT LOG (%d events) ---\n", count);
    for (int i = 0; i < count; i++) {
        print_event(&log[i]);
    }
    printf("------------------------------------\n\n");
}

int search_by_door(const AccessEvent *log, int count,
                   const char *door_id, AccessEvent *results, int max_results) {
    int found = 0;
    for (int i = 0; i < count && found < max_results; i++) {
        if (strncmp(log[i].door_id, door_id, DOOR_ID_LEN) == 0) {
            results[found++] = log[i];
        }
    }
    return found;
}

int check_lockout(const AccessEvent *log, int count,
                  const char *door_id, const char *credential_id) {
    int consecutive_denials = 0;

    // Walk backwards through log for this door/credential combo
    for (int i = count - 1; i >= 0; i--) {
        if (strncmp(log[i].door_id, door_id, DOOR_ID_LEN) == 0 &&
            strncmp(log[i].credential_id, credential_id, CRED_ID_LEN) == 0) {

            if (log[i].type == DENIED) {
                consecutive_denials++;
                if (consecutive_denials >= 3) return 1;  // lockout
            } else {
                break;  // chain broken by non-denial event
            }
        }
    }
    return 0;
}