#ifndef EVENTS_H
#define EVENTS_H

#include <time.h>

#define MAX_EVENTS 100
#define DOOR_ID_LEN 20
#define CRED_ID_LEN 20

typedef enum {
    GRANTED,
    DENIED,
    DOOR_FORCED,
    DOOR_HELD
} EventType;

typedef struct {
    int event_id;
    EventType type;
    char door_id[DOOR_ID_LEN];
    char credential_id[CRED_ID_LEN];
    time_t timestamp;
} AccessEvent;

// Add a new event to the log
int add_event(AccessEvent *log, int *count, EventType type,
              const char *door_id, const char *credential_id);

// Print all events to stdout
void print_log(const AccessEvent *log, int count);

// Print a single event formatted
void print_event(const AccessEvent *event);

// Search events by door ID, returns number of matches found
int search_by_door(const AccessEvent *log, int count,
                   const char *door_id, AccessEvent *results, int max_results);

// Check if a credential should be locked out (3 consecutive denials on same door)
int check_lockout(const AccessEvent *log, int count,
                  const char *door_id, const char *credential_id);

// Convert EventType enum to readable string
const char *event_type_string(EventType type);

#endif