#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTS 100
#define MAX_NAME_LENGTH 50
#define FILENAME "student_data.txt"

struct Student {
    int studentID;
    char name[50];
    char surname[50];
    int grades[6];
    float average;
    int pass; 
};

void calculateGradeAndPass(struct Student *student);

struct Student students[MAX_STUDENTS];
int totalStudents = 0;

void saveDataToFile() {
    FILE *file;
    file = fopen("student_data.txt", "w");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        exit(1); 
    }

    for (int i = 0; i < totalStudents; i++) {
        fprintf(file, "%d %s %s", students[i].studentID, students[i].name, students[i].surname);
        for (int j = 0; j < 6; j++) {
            fprintf(file, " %d", students[i].grades[j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
}


void loadDataFromFile() {
    FILE *file = fopen("student_data.txt", "r");
    if (file == NULL) {
        printf("Error opening file for reading.\n");
        return;
    }
    totalStudents = 0;
    while (fscanf(file, "%d %s %s", &students[totalStudents].studentID, students[totalStudents].name, students[totalStudents].surname) == 3) {
        for (int j = 0; j < 6; j++) {
            fscanf(file, " %d", &students[totalStudents].grades[j]);
        }

        // Calculate pass or fail status for the loaded student
        calculateGradeAndPass(&students[totalStudents]);

        totalStudents++;
        if (totalStudents >= MAX_STUDENTS) {
            break;
        }
    }

    fclose(file);
}

void calculateGradeAndPass(struct Student *student) {
    float average = 0.0;
    for (int i = 0; i < 6; i++) {
        average += student->grades[i];
    }
    average /= 6.0;
    

    if (average >= 50) {
        student->pass = 1;
    }
	else {
        
        student->pass = 0;
    }
}

void addStudent() {
    if (totalStudents >= MAX_STUDENTS) {
        printf("System capacity is full, no more students can be added.\n");
        return;
    }

    struct Student newStudent;
    newStudent.studentID = totalStudents + 1;
    printf("Name: ");
    scanf("%s", newStudent.name);
    printf("Surname: ");
	scanf("%s", newStudent.surname);

    printf("Enter grades for 6 lectures:\n");
    for (int i = 0; i < 6; i++) {
        printf("Grade %d: ", i + 1);
        scanf("%d", &newStudent.grades[i]);
    }

    calculateGradeAndPass(&newStudent);

    students[totalStudents++] = newStudent;
    saveDataToFile();
    printf("Student successfully added.\n");
}

void removeStudent(int index) {
    if (index < 0 || index >= totalStudents) {
        printf("Invalid index for student removal.\n");
        return;
    }

    for (int i = index; i < totalStudents - 1; i++) {
        students[i] = students[i + 1];
    }

    totalStudents--;
    // Update student IDs
    for (int i = 0; i < totalStudents; i++) {
        students[i].studentID = i + 1;
    }
    
    saveDataToFile();
    printf("Student successfully removed.\n");
}

void printStudentList() {
    printf("\nStudent List:\n");
    printf("ID\tName\t\tGrades\t\t\tAverage\t\tPass\n");
    for (int i = 0; i < totalStudents; i++) {
        printf("%d %2s %2s \t\t", students[i].studentID, students[i].name, students[i].surname);
        for (int j = 0; j < 6; j++) {
            printf("%d ", students[i].grades[j]);
        }
        float average = 0.0;
        for (int j = 0; j < 6; j++) {
            average += students[i].grades[j];
        }
        average /= 6.0;
        printf("\t%.2f\t\t%s\n", average, students[i].pass ? "Yes" : "No");
    }
}

int main() {
    loadDataFromFile();

    int choice;

    do {
        printf("\n1. Add Student\n2. Remove Student\n3. Print Student List\n4. Exit\nYour choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addStudent();
                break;
            case 2: {
                int index;
                printf("Enter the index of the student to remove: ");
                scanf("%d", &index);
                removeStudent(index - 1);
                break;
            }
            case 3:
                printStudentList();
                break;
            case 4:
                printf("Exiting the program.\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 4);

    return 0;
}
