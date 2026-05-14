#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int id;
    char studentName[50];
    float marks;
} Student;

// Function declarations
void storeStudents(const char *fileName, int count);
void showRecord(const char *fileName, int position);
void removeRecord(const char *fileName, int roll);

int main()
{
    int n, pos, deleteId;

    printf("Enter number of students: ");
    scanf("%d", &n);

    storeStudents("students.dat", n);

    printf("\nEnter record number to display: ");
    scanf("%d", &pos);

    showRecord("students.dat", pos);

    printf("\nEnter roll number to delete: ");
    scanf("%d", &deleteId);

    removeRecord("students.dat", deleteId);

    return 0;
}

// Function to store student records
void storeStudents(const char *fileName, int count)
{
    FILE *fp = fopen(fileName, "wb");
    Student s;

    if (fp == NULL)
    {
        printf("There was an error opening this file.\n");
        return;
    }

    for (int i = 0; i < count; i++)
    {
        printf("\nStudent %d Details\n", i + 1);

        printf("Enter Roll Number: ");
        scanf("%d", &s.id);

        printf("Enter Student Name: ");
        scanf("%s", s.studentName);

        printf("Enter Marks: ");
        scanf("%f", &s.marks);

        fwrite(&s, sizeof(Student), 1, fp);
    }

    fclose(fp);

    printf("\nAll records stored successfully.\n");
}

// Function to access mth record using fseek
void showRecord(const char *fileName, int position)
{
    FILE *fp = fopen(fileName, "rb");
    Student s;

    if (fp == NULL){
        printf("File cannot be opened.\n");
        return;
    }

    // Move directly to required record
    fseek(fp, (position - 1) * sizeof(Student), SEEK_SET);

    if (fread(&s, sizeof(Student), 1, fp) == 1)
    {
        printf("\nStudent Record Found\n");
        printf("Student ID : %d\n", s.id);
        printf("Name        : %s\n", s.studentName);
        printf("Marks       : %.2f\n", s.marks);
    }
    else
    {
        printf("\nInvalid record position.\n");
    }

    fclose(fp);
}

// Function to delete a student record
void removeRecord(const char *fileName, int roll)
{
    FILE *mainFile, *tempFile;
    Student s;
    int found = 0;

    mainFile = fopen(fileName, "rb");

    if (mainFile == NULL || temp == NULL){
        printf("There was an error in opening this file.\n");
        return;
    }

    tempFile = fopen("backup.dat", "wb");

    while (fread(&s, sizeof(Student), 1, mainFile))
    {
        if (s.rollNo == roll){
            found = 1;
        }
        else{
            fwrite(&s, sizeof(Student), 1, tempFile);
        }
    }

    fclose(mainFile);
    fclose(tempFile);

    remove(fileName);
    rename("backup.dat", fileName);

    if (found){
        printf("\nRecord deleted successfully.\n");
    }
    else{
        printf("\nRoll number not found.\n");
    }
}