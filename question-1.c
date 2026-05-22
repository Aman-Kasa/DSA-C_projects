#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN 100

typedef struct {
    char firstName[MAX_NAME_LEN];
    char lastName[MAX_NAME_LEN];
    char studentID[MAX_NAME_LEN];
    double grade;
} Student;

int compareStudents(const Student *a, const Student *b) {
    if (a->grade < b->grade) return -1;
    if (a->grade > b->grade) return 1;
    return strcmp(a->firstName, b->firstName);
}

void swap(Student *a, Student *b) {
    Student temp = *a;
    *a = *b;
    *b = temp;
}

int partition(Student arr[], int low, int high) {
    Student pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j < high; j++) {
        if (compareStudents(&arr[j], &pivot) <= 0) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quickSort(Student arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

int main() {
    FILE *inFile = fopen("students.txt", "r");
    if (!inFile) {
        perror("Error: Missing students.txt. Please create it first.");
        return EXIT_FAILURE;
    }

    int capacity = 10, n = 0;
    Student *students = malloc(capacity * sizeof(Student));

    while (fscanf(inFile, "%99s %99s %99s %lf", 
                  students[n].firstName, students[n].lastName, 
                  students[n].studentID, &students[n].grade) == 4) {
        n++;
        if (n >= capacity) {
            capacity *= 2;
            students = realloc(students, capacity * sizeof(Student));
        }
    }
    fclose(inFile);

    if (n > 0) quickSort(students, 0, n - 1);

    FILE *outFile = fopen("sorted_students.txt", "w");
    for (int i = 0; i < n; i++) {
        fprintf(outFile, "%s %s %s %.2f\n", 
                students[i].firstName, students[i].lastName, 
                students[i].studentID, students[i].grade);
    }

    fclose(outFile);
    free(students);
    printf("Successfully sorted %d records into sorted_students.txt\n", n);
    return EXIT_SUCCESS;
}
