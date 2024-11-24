#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TASKS 100
#define MAX_NAME_LENGTH 50
#define MAX_DESC_LENGTH 256
#define TODO_FILE "todo.txt"

// Definition des Task-Structs
typedef struct {
    int id; // nummer der Aufgabe (Task)
    char assignee[MAX_NAME_LENGTH]; // Name der Person, die der Aufgabe zugewiesen ist
    char description[MAX_DESC_LENGTH]; // Beschreibung der Aufgabe
} Task;

// Funktion zum Laden der Aufgaben aus der Datei
int loadTasks(Task tasks[]) {
    FILE* file = fopen(TODO_FILE, "r");
    if (file == NULL) {
        printf("Keine vorhandene Aufgabenliste gefunden. Es wird eine neue Datei erstellt.\n");
        return 0;
    }

    int taskCount = 0;
    while (fscanf(file, "%d,%49[^,],%255[^\n]\n", &tasks[taskCount].id, tasks[taskCount].assignee, tasks[taskCount].description) == 3) {
        taskCount++;
        if (taskCount >= MAX_TASKS) {
            printf("Maximale Anzahl an Aufgaben erreicht.\n");
            break;
        }
    }

    fclose(file);
    return taskCount;
}

// Funktion zum Speichern der Aufgaben in die Datei
void saveTasks(Task tasks[], int taskCount) {
    FILE* file = fopen(TODO_FILE, "w");
    if (file == NULL) {
        printf("Fehler beim Speichern der Aufgaben.\n");
        return;
    }

    for (int i = 0; i < taskCount; i++) {
        fprintf(file, "%d,%s,%s\n", tasks[i].id, tasks[i].assignee, tasks[i].description);
    }

    fclose(file);
    printf("\x8enderungen wurden gespeichert.\n");
}

// Funktion zum Hinzufügen einer Aufgabe
void addTask(Task tasks[], int* taskCount) {
    if (*taskCount >= MAX_TASKS) {
        printf("Die Aufgabenliste ist voll. Keine weiteren Aufgaben k\x94nnen hinzugef\x81gt werden.\n");
        return;
    }

    Task newTask;
    newTask.id = (*taskCount > 0) ? tasks[*taskCount - 1].id + 1 : 1;

    printf("Gib den Bearbeiter der Aufgabe ein: ");
    getchar();  // Eingabepuffer leeren
    fgets(newTask.assignee, MAX_NAME_LENGTH, stdin);
    newTask.assignee[strcspn(newTask.assignee, "\n")] = 0;  // Entfernt das Zeilenende

    printf("Gib die Beschreibung der Aufgabe ein: ");
    fgets(newTask.description, MAX_DESC_LENGTH, stdin);
    newTask.description[strcspn(newTask.description, "\n")] = 0;  // Entfernt das Zeilenende

    tasks[*taskCount] = newTask;
    (*taskCount)++;
    printf("Aufgabe hinzugef\x81gt.\n");

    saveTasks(tasks, *taskCount);
}

// Funktion zum Anzeigen aller Aufgaben
void listTasks(Task tasks[], int taskCount) {
    if (taskCount == 0) {
        printf("Keine Aufgaben vorhanden.\n");
        return;
    }

    printf("TODO-Liste:\n");
    for (int i = 0; i < taskCount; i++) {
        printf("%d. %s - %s\n", tasks[i].id, tasks[i].assignee, tasks[i].description);
    }
}

// Funktion zum Entfernen einer Aufgabe
void deleteTask(Task tasks[], int* taskCount) {
    if (*taskCount == 0) {
        printf("Keine Aufgaben vorhanden.\n");
        return;
    }

    listTasks(tasks, *taskCount);
    printf("\nGib die ID der Aufgabe ein, die du l\x94schen m\x94\x63htest: ");
    int taskId;
    scanf("%d", &taskId);

    int found = 0;
    for (int i = 0; i < *taskCount; i++) {
        if (tasks[i].id == taskId) {
            found = 1;
            for (int j = i; j < *taskCount - 1; j++) {
                tasks[j] = tasks[j + 1];
            }
            (*taskCount)--;
            printf("Aufgabe mit ID %d gel\x94scht.\n", taskId);
            break;
        }
    }

    if (!found) {
        printf("Aufgabe mit ID %d nicht gefunden.\n", taskId);
    }
    else {
        saveTasks(tasks, *taskCount);
    }
}

// Hauptmenü
int main() {
    Task tasks[MAX_TASKS];
    int taskCount = loadTasks(tasks);  // Lädt die Aufgaben einmalig beim Start

    int choice;
    do {
        printf("\nTODO-Liste Verwaltung\n");
        printf("1. Aufgabe hinzuf\x81gen\n");
        printf("2. Aufgaben anzeigen\n");
        printf("3. Aufgabe l\x94schen\n");
        printf("4. Programm beenden\n");
        printf("W\x84hle eine Option: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            addTask(tasks, &taskCount);
            break;
        case 2:
            listTasks(tasks, taskCount);
            break;
        case 3:
            deleteTask(tasks, &taskCount);
            break;
        case 4:
            printf("Programm beendet.\n");
            break;
        default:
            printf("Ung\x81ltige Auswahl. Bitte versuchen Sie es erneut.\n");
            break;
        }
    } while (choice != 4);

    return 0;
}
