#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_INCIDENTS 25

// --- Data Structures ---
typedef struct Incident {
    int id;
    char description[256];
    time_t timestamp;
    struct Incident *prev;
    struct Incident *next;
} Incident;

typedef struct {
    Incident *head;
    Incident *tail;
    Incident *active;
    int count;
    int next_id;
    int is_live;
    pthread_mutex_t lock;
} Tracker;

Tracker tracker;

// --- Core Functions ---
void init_tracker() {
    tracker.head = NULL;
    tracker.tail = NULL;
    tracker.active = NULL;
    tracker.count = 0;
    tracker.next_id = 1;
    tracker.is_live = 0;
    pthread_mutex_init(&tracker.lock, NULL);
}

void add_incident(const char* desc) {
    pthread_mutex_lock(&tracker.lock);

    Incident *new_inc = (Incident*)malloc(sizeof(Incident));
    if (!new_inc) {
        perror("[Error] Memory allocation failed for new incident");
        pthread_mutex_unlock(&tracker.lock);
        return;
    }
    new_inc->id = tracker.next_id++;
    strncpy(new_inc->description, desc, 255);
    new_inc->description[255] = '\0';
    new_inc->timestamp = time(NULL);
    new_inc->next = NULL;
    new_inc->prev = tracker.tail;

    if (tracker.tail == NULL) {
        tracker.head = new_inc;
        tracker.tail = new_inc;
        tracker.active = new_inc; // Start at oldest recorded incident per requirements
    } else {
        tracker.tail->next = new_inc;
        tracker.tail = new_inc;
    }

    tracker.count++;

    // Enforce 25 incident capacity
    if (tracker.count > MAX_INCIDENTS) {
        Incident *temp = tracker.head;
        tracker.head = tracker.head->next;
        if (tracker.head) tracker.head->prev = NULL;

        // If active pointer was on the deleted head, shift it forward
        if (tracker.active == temp) {
            tracker.active = tracker.head;
        }
        free(temp);
        tracker.count--;
    }

    pthread_mutex_unlock(&tracker.lock);
}

void delete_all() {
    pthread_mutex_lock(&tracker.lock);
    Incident *current = tracker.head;
    while (current != NULL) {
        Incident *next = current->next;
        free(current);
        current = next;
    }
    tracker.head = tracker.tail = tracker.active = NULL;
    tracker.count = 0;
    pthread_mutex_unlock(&tracker.lock);
    printf("\n[System] All incidents deleted.\n");
}

void print_active() {
    pthread_mutex_lock(&tracker.lock);
    if (tracker.active == NULL) {
        printf("\r[System] No incidents to display.          \n");
    } else {
        struct tm *tm_info = localtime(&tracker.active->timestamp);
        char time_buf[26];
        strftime(time_buf, 26, "%H:%M:%S", tm_info);
        printf("\r>> Incident #%d [%s]: %s (Showing %d of %d)       \n",
               tracker.active->id, time_buf, tracker.active->description,
               tracker.active->id - (tracker.head->id) + 1, tracker.count);
    }
    pthread_mutex_unlock(&tracker.lock);
}

// --- Live Monitoring Thread ---
void* live_feed(void* arg) {
    char buffer[256];
    int sim_count = 1;
    while (tracker.is_live) {
        // Responsive sleep loop: checks flag every 100ms for up to 5 seconds
        for (int i = 0; i < 50; i++) {
            if (!tracker.is_live) break;
            usleep(100000); // 100ms
        }
        
        if (!tracker.is_live) break;

        snprintf(buffer, sizeof(buffer), "Simulated Emergency Report Alpha-%d", sim_count++);
        add_incident(buffer);
        printf("\n[ALERT] New incident arrived! Press 'f' to view.\n");
        print_active();
    }
    return NULL;
}

// --- Main Execution ---
int main() {
    init_tracker();
    char command;
    pthread_t live_thread;

    printf("Emergency Dispatch Incident Tracker Initialized.\n");
    printf("Commands: [f] newer, [b] older, [l] live on, [s] live off, [d] delete all, [q] quit\n");

    // Seed some initial data
    add_incident("Vehicle collision on Main St");
    add_incident("Fire alarm triggered at City Hall");
    print_active();

    while (1) {
        command = getchar();
        if (command == '\n') continue; // consume newline

        switch (command) {
            case 'f':
                pthread_mutex_lock(&tracker.lock);
                if (tracker.active && tracker.active->next) {
                    tracker.active = tracker.active->next;
                }
                pthread_mutex_unlock(&tracker.lock);
                print_active();
                break;
            case 'b':
                pthread_mutex_lock(&tracker.lock);
                if (tracker.active && tracker.active->prev) {
                    tracker.active = tracker.active->prev;
                }
                pthread_mutex_unlock(&tracker.lock);
                print_active();
                break;
            case 'l':
                if (!tracker.is_live) {
                    tracker.is_live = 1;
                    printf("\n[System] Live monitoring ENABLED.\n");
                    if (pthread_create(&live_thread, NULL, live_feed, NULL) != 0) {
                        perror("[Error] Failed to create live monitoring thread");
                        tracker.is_live = 0;
                    }
                }
                break;
            case 's':
                if (tracker.is_live) {
                    tracker.is_live = 0;
                    pthread_join(live_thread, NULL);
                    printf("\n[System] Live monitoring DISABLED.\n");
                }
                break;
            case 'd':
                delete_all();
                break;
            case 'q':
                if (tracker.is_live) {
                    tracker.is_live = 0;
                    pthread_join(live_thread, NULL);
                }
                delete_all();
                pthread_mutex_destroy(&tracker.lock);
                printf("\nSession saved. Quitting...\n");
                return 0;
            default:
                printf("\nInvalid command.\n");
        }
    }
    return 0;
}
