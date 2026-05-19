#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    int id;
    char studentName[50];
    float marks;
} Student;

int main(){

int n, pos, deleteId;

    printf("Enter number of students: ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Invalid number of students.\n");
        return 1;
    }

    Student *students = malloc(n * sizeof(Student));
    if (students == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        printf("Student %d ID: ", i + 1);
        scanf("%d", &students[i].id);
        printf("Student %d Name: ", i + 1);
        scanf("%49s", students[i].studentName);
        printf("Student %d Marks: ", i + 1);
        scanf("%f", &students[i].marks);
    }

    const char *filename = "students.txt";
    writeToFile(filename, students, n);

    int count = 0;
    long *positions = createSeekIndex(filename, &count);
    if (positions == NULL || count == 0) {
        free(students);
        printf("Failed to build seek index.\n");
        return 1;
    }

    printf("\nRecord seek positions:\n");
    for (int i = 0; i < count; i++) {
        printf("Record %d starts at byte offset %ld\n", i + 1, positions[i]);
    }

    int recordNumber;
    printf("\nEnter record number to display (1-%d): ", count);
    if (scanf("%d", &recordNumber) == 1 && recordNumber >= 1 && recordNumber <= count) {
        displayRecordAtPosition(filename, positions[recordNumber - 1]);
    } else {
        printf("Invalid record number.\n");
    }

    free(students);
    free(positions);
    return 0;
}

void writeToFile(const char *filename, Student s[], int n) {
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("There was an error opening the file '%s'.\n", filename);
        return;
    }   
    for(int i = 0; i < n; i++){
        fprintf(fp, "%d %s %.2f\n", s[i].id, s[i].studentName, s[i].marks);
    }

    fclose(fp);
}

long *createSeekIndex(const char *filename, int *count) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Failed to open '%s' for reading.\n", filename);
        *count = 0;
        return NULL;
    }

    long *positions = NULL;
    char buffer[256];
    int idx = 0;

    while (1) {
        long pos = ftell(fp);
        if (pos < 0) {
            break;
        }

        if (fgets(buffer, sizeof(buffer), fp) == NULL) {
            break;
        }

        long *temp = realloc(positions, (idx + 1) * sizeof(long));
        if (temp == NULL) {
            free(positions);
            fclose(fp);
            *count = 0;
            return NULL;
        }

        positions = temp;
        positions[idx++] = pos;
    }

    fclose(fp);
    *count = idx;
    return positions;
}

void displayRecordAtPosition(const char *filename, long pos) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Failed to open '%s' for reading.\n", filename);
        return;
    }

    if (fseek(fp, pos, SEEK_SET) != 0) {
        printf("Failed to seek to position %ld.\n", pos);
        fclose(fp);
        return;
    }

    Student s;
    char line[256];
    if (fgets(line, sizeof(line), fp) == NULL) {
        printf("No record found at position %ld.\n", pos);
        fclose(fp);
        return;
    }

    if (sscanf(line, "%d %49s %f", &s.id, s.studentName, &s.marks) == 3) {
        printf("\nRecord at position %ld:\n", pos);
        printf("ID: %d\n", s.id);
        printf("Name: %s\n", s.studentName);
        printf("Marks: %.2f\n", s.marks);
    } else {
        printf("Failed to parse record at position %ld.\n", pos);
    }

    fclose(fp);
}

