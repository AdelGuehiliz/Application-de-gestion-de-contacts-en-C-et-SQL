#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>

#define MAX_LEN 100

void initialize_database(sqlite3 *db);
void load_contacts(sqlite3 *db);
void add_contact(sqlite3 *db, const char *name, const char *phone, const char *email);
void clear_input_buffer();

int main() {
    sqlite3 *db;
    int rc = sqlite3_open("contacts.db", &db);
    if (rc) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    initialize_database(db);

    int choice;
    char name[MAX_LEN], phone[MAX_LEN], email[MAX_LEN];

    do {
        printf("\nMenu:\n");
        printf("1. Afficher les contacts\n");
        printf("2. Ajouter un contact\n");
        printf("3. Quitter\n");
        printf("Votre choix: ");
        scanf("%d", &choice);
        clear_input_buffer();

        switch (choice) {
            case 1:
                load_contacts(db);
                break;
            case 2:
                printf("Nom: ");
                fgets(name, MAX_LEN, stdin);
                name[strcspn(name, "\n")] = 0;

                printf("Numéro de téléphone: ");
                fgets(phone, MAX_LEN, stdin);
                phone[strcspn(phone, "\n")] = 0;

                printf("Email: ");
                fgets(email, MAX_LEN, stdin);
                email[strcspn(email, "\n")] = 0;

                add_contact(db, name, phone, email);
                break;
            case 3:
                printf("Au revoir !\n");
                break;
            default:
                printf("Choix invalide, veuillez réessayer.\n");
        }
    } while (choice != 3);

    sqlite3_close(db);
    return 0;
}

void initialize_database(sqlite3 *db) {
    const char *sql = "CREATE TABLE IF NOT EXISTS Contacts (Id INTEGER PRIMARY KEY, Name TEXT, PhoneNumber TEXT, Email TEXT);";
    char *errMsg = NULL;
    if (sqlite3_exec(db, sql, 0, 0, &errMsg) != SQLITE_OK) {
        fprintf(stderr, "Erreur SQL: %s\n", errMsg);
        sqlite3_free(errMsg);
    }
}

void load_contacts(sqlite3 *db) {
    const char *sql = "SELECT * FROM Contacts";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Erreur de lecture de la base de données.\n");
        return;
    }

    printf("\nListe des contacts:\n");
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        printf("%s - %s - %s\n",
               sqlite3_column_text(stmt, 1),
               sqlite3_column_text(stmt, 2),
               sqlite3_column_text(stmt, 3));
    }
    sqlite3_finalize(stmt);
}

void add_contact(sqlite3 *db, const char *name, const char *phone, const char *email) {
    const char *sql = "INSERT INTO Contacts (Name, PhoneNumber, Email) VALUES (?, ?, ?);";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Erreur de préparation SQL.\n");
        return;
    }
    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, phone, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, email, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        fprintf(stderr, "Erreur lors de l'ajout du contact.\n");
    } else {
        printf("Contact ajouté avec succès !\n");
    }
    sqlite3_finalize(stmt);
}

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
