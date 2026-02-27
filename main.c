#include <stdio.h>
#include <string.h>
#include <stdlib.h>    // add this line
#include "events.h"
#include "storage.h"

void get_input(const char *prompt, char *buf, int len) {
    printf("%s", prompt);
    fgets(buf, len, stdin);
    // Strip trailing newline
    buf[strcspn(buf, "\n")] = '\0';
}

void menu_add_event(AccessEvent *log, int *count) {
    char door_id[DOOR_ID_LEN];
    char cred_id[CRED_ID_LEN];
    char type_input[10];
    EventType type;

    get_input("Door ID: ", door_id, DOOR_ID_LEN);
    get_input("Credential ID: ", cred_id, CRED_ID_LEN);
    printf("Event type (0=GRANTED, 1=DENIED, 2=FORCED, 3=HELD): ");
    get_input("", type_input, 10);
    type = (EventType)atoi(type_input);

    // Check lockout before adding
    if (type == DENIED && check_lockout(log, *count, door_id, cred_id)) {
        printf("WARNING: Credential %s is locked out at door %s.\n",
               cred_id, door_id);
    }

    int id = add_event(log, count, type, door_id, cred_id);
    if (id > 0) printf("Event %d logged.\n", id);
}

void menu_search(const AccessEvent *log, int count) {
    char door_id[DOOR_ID_LEN];
    get_input("Search by door ID: ", door_id, DOOR_ID_LEN);

    AccessEvent results[MAX_EVENTS];
    int found = search_by_door(log, count, door_id, results, MAX_EVENTS);

    if (found == 0) {
        printf("No events found for door %s.\n", door_id);
    } else {
        printf("\n--- Results for door %s (%d events) ---\n", door_id, found);
        for (int i = 0; i < found; i++) {
            print_event(&results[i]);
        }
    }
}

int main(void) {
    AccessEvent log[MAX_EVENTS];
    int count = load_log(log, MAX_EVENTS);
    char choice[10];

    printf("\n=== Door Access Event Logger ===\n");

    while (1) {
        printf("\n1) Add event\n");
        printf("2) View all events\n");
        printf("3) Search by door\n");
        printf("4) Save and exit\n");
        get_input("Choice: ", choice, 10);

        switch (atoi(choice)) {
            case 1: menu_add_event(log, &count); break;
            case 2: print_log(log, count);       break;
            case 3: menu_search(log, count);     break;
            case 4:
                save_log(log, count);
                printf("Goodbye.\n");
                return 0;
            default:
                printf("Invalid choice.\n");
        }
    }
}