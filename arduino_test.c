#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define MAX_COMP 15
#define ANGLE 11

typedef struct {
    int compartment;
    char date[11];    // Format: YYYY-MM-DD
    char time[6];     // Format: HH:MM
    char name[50];
    char effect[100];
    unsigned long reminderTime; // simulated timestamp
} ScheduleEntry;

ScheduleEntry schedule[MAX_COMP];
int entries = 0;

// Simulate time conversion from date/time strings to a single value
unsigned long convertToMillis(const char* date, const char* time) {
    int year, month, day, hour, minute;
    sscanf(date, "%d-%d-%d", &year, &month, &day);
    sscanf(time, "%d:%d", &hour, &minute);
    return ((year * 12 + month) * 31 + day) * 24 * 60 + hour * 60 + minute;
}

void addEntry() {
    if (entries >= MAX_COMP) {
        printf("Schedule is full.\n");
        return;
    }

    char buffer[100];
    int compartment;
    char date[11], time[6], name[50], effect[100];

    // Get compartment number
    printf("Enter compartment number (1–14): ");
    fgets(buffer, sizeof(buffer), stdin);
    sscanf(buffer, "%d", &compartment);

    printf("Enter date (YYYY-MM-DD): ");
    fgets(date, sizeof(date), stdin);
    date[strcspn(date, "\n")] = '\0';
    getchar(); // clear newline

    printf("Enter time (HH:MM): ");
    fgets(time, sizeof(time), stdin);
    time[strcspn(time, "\n")] = '\0';
    getchar(); // clear newline

    printf("Enter medication name: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0';
    getchar(); // clear newline

    printf("Enter medication effect: ");
    fgets(effect, sizeof(effect), stdin);
    effect[strcspn(effect, "\n")] = '\0';

    ScheduleEntry entry;
    entry.compartment = compartment;
    strncpy(entry.date, date, 10);
    strncpy(entry.time, time, 5);
    strncpy(entry.name, name, 49);
    strncpy(entry.effect, effect, 99);
    entry.reminderTime = convertToMillis(date, time);

    schedule[entries++] = entry;

    printf("Entry stored successfully.\n\n");
}

void searchAndSimulate() {
    LARGE_INTEGER frequency, start, end;
    QueryPerformanceFrequency(&frequency);

    int targetCompartment;
    printf("Enter compartment number to search for: ");
    scanf("%d", &targetCompartment);
    getchar(); // clear newline

    QueryPerformanceCounter(&start);

    for (int i = 0; i < entries; i++) {
        if (schedule[i].compartment == targetCompartment) {

            QueryPerformanceCounter(&end);
            double elapsedTime = (double)(end.QuadPart - start.QuadPart) * 1000.0 / frequency.QuadPart;

            printf("\nFound matching schedule:\n");
            printf("Compartment: %d\n", schedule[i].compartment);
            printf("Medication: %s\n", schedule[i].name);
            printf("Effect: %s\n", schedule[i].effect);
            printf("Rotation angle: %d degrees\n", ANGLE * targetCompartment);
            printf("Elapsed time to find: %.5f ms\n", elapsedTime);
            return;
        }
    }

    printf("No matching compartment found.\n");
}

int main() {
    printf("Medication Scheduler (C Simulation)\n");
    printf("====================================\n\n");

    int numEntries;
    printf("How many entries do you want to input? ");
    scanf("%d", &numEntries);
    getchar(); // clear newline

    for (int i = 0; i < numEntries; i++) {
        printf("\n--- Entry %d ---\n", i + 1);
        addEntry();
    }

    searchAndSimulate();

    return 0;
}