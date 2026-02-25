#include <stdio.h>
#include "events.h"

int main(void) {
    AccessEvent log[MAX_EVENTS];
    int count = 0;

    // Add a few hardcoded test events
    add_event(log, &count, GRANTED,     "DOOR-01", "CRED-1234");
    add_event(log, &count, DENIED,      "DOOR-01", "CRED-9999");
    add_event(log, &count, DENIED,      "DOOR-01", "CRED-9999");
    add_event(log, &count, DENIED,      "DOOR-01", "CRED-9999");
    add_event(log, &count, DOOR_FORCED, "DOOR-02", "UNKNOWN");

    // Test print
    print_log(log, count);

    // Test lockout check
    int locked = check_lockout(log, count, "DOOR-01", "CRED-9999");
    printf("Lockout status for CRED-9999 at DOOR-01: %s\n",
           locked ? "LOCKED OUT" : "OK");

    return 0;
}