#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_PHONENUMBER_LENGTH 20
#define MAX_NAME_LENGTH 50
#define MAX_PACKAGE_LENGTH 50
#define MAX_TIMESTAMP_LENGTH 50

struct subscriber {
    char phonenumber[MAX_PHONENUMBER_LENGTH];
    char name[MAX_NAME_LENGTH];
    char state[18];
    float amount;
    char package[MAX_PACKAGE_LENGTH];
    char timestamp[MAX_TIMESTAMP_LENGTH];
};

// Function prototypes
void listRecords();
void searchRecords();
void storePaymentInfo(char *phonenumber, float amount, char *package, char *timestamp);
void payment();
void registerSubscriber();
void displayPackages();
void displayPaymentRecords();

void listRecords() {
    struct subscriber s;
    FILE *f;
    int i;

    f = fopen("hello.txt", "rb");
    if (f == NULL) {
        printf("Error opening records file.\n");
        exit(1);
    }

    system("cls"); // This clears the screen, platform dependent (Windows)
    printf("Phone Number\t\tUser Name\t\t\tState\t\tTimestamp\n");
    for (i = 0; i < 99; i++)
        printf("-");
    printf("\n");

    while (fread(&s, sizeof(s), 1, f) == 1) {
        printf("%-10s\t\t%-20s\t\t%s\t%20s\n", s.phonenumber, s.name, s.state, s.timestamp);
    }

    printf("\n");
    for (i = 0; i < 99; i++)
        printf("-");

    fclose(f);
}

void searchRecords() {
    struct subscriber s;
    FILE *f;
    char phonenumber[MAX_PHONENUMBER_LENGTH];
    int found = 0;

    f = fopen("hello.txt", "rb");
    if (f == NULL) {
        printf("Error opening records file.\n");
        exit(1);
    }

    printf("Enter Phone Number to search in our database: ");
    scanf("%s", phonenumber);

    printf("Search Results:\n");
    printf("Phone Number\t\tUser Name\t\t\tState\t\tTimestamp\n");
    for (int i = 0; i < 99; i++)
        printf("-");

    while (fread(&s, sizeof(s), 1, f) == 1) {
        if (strcmp(s.phonenumber, phonenumber) == 0) {
            printf("\n%-10s\t\t%-20s\t\t%s\t%20s\n", s.phonenumber, s.name, s.state, s.timestamp);
            found = 1;
        }
    }

    if (!found) {
        printf("\nRequested Phone Number Not found in our database\n");
    }

    printf("\n");
    for (int i = 0; i < 99; i++)
        printf("-");

    fclose(f);
}

void storePaymentInfo(char *phonenumber, float amount, char *package, char *timestamp) {
    FILE *paymentFile;
    paymentFile = fopen("poisaa_records.txt", "a");
    if (paymentFile == NULL) {
        printf("Error opening payment records file.\n");
        exit(1);
    }

    fprintf(paymentFile, "%-10s\t\t%.2f\t\t%s\t\t%s\n", phonenumber, amount, package, timestamp);
    fclose(paymentFile);
}

void payment() {
    struct subscriber s;
    FILE *f;
    char phonenumber[MAX_PHONENUMBER_LENGTH];
    float amt;
    int found = 0;

    f = fopen("hello.txt", "r+");
    if (f == NULL) {
        printf("Error opening records file.\n");
        exit(1);
    }

    printf("Enter your phone number: ");
    scanf("%s", phonenumber);

    while (fread(&s, sizeof(s), 1, f) == 1) {
        if (strcmp(s.phonenumber, phonenumber) == 0) {
            found = 1;
            printf("\nPhone No.: %s", s.phonenumber);
            printf("\nName\t: %s", s.name);
            printf("\nState\t: %s", s.state);
            printf("\n\nAvailable Packages:\n");
            displayPackages();
            printf("\nEnter the package number you want to recharge: ");
            scanf("%s", s.package);
            printf("\nEnter the amount for recharge: ");
            scanf("%f", &amt);
            s.amount = amt;

            time_t t = time(NULL);
            char *timeString = ctime(&t);
            timeString[strcspn(timeString, "\n")] = '\0';
            strcpy(s.timestamp, timeString);

            printf("\nRecharge successful. Updated balance: %.2f\n", s.amount);

            storePaymentInfo(s.phonenumber, amt, s.package, s.timestamp);

            fseek(f, -sizeof(s), SEEK_CUR);
            fwrite(&s, sizeof(s), 1, f);

            break;
        }
    }

    if (!found) {
        printf("Subscriber with the given phone number not found. Please register first.\n");
    }

    fclose(f);
}

void registerSubscriber() {
    struct subscriber s;
    FILE *f;
    char phonenumber[MAX_PHONENUMBER_LENGTH];
    char name[MAX_NAME_LENGTH];
    char state[18];

    f = fopen("hello.txt", "ab");
    if (f == NULL) {
        printf("Error opening records file.\n");
        exit(1);
    }

    printf("Enter your phone number: ");
    scanf("%19s", phonenumber);
    printf("\nEnter your name: ");
    getchar();
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0';
    printf("\nEnter your state: ");
    fgets(state, sizeof(state), stdin);
    state[strcspn(state, "\n")] = '\0';

    strcpy(s.phonenumber, phonenumber);
    strcpy(s.name, name);
    strcpy(s.state, state);

    time_t t = time(NULL);
    char *timeString = ctime(&t);
    timeString[strcspn(timeString, "\n")] = '\0';
    strcpy(s.timestamp, timeString);

    fwrite(&s, sizeof(s), 1, f);

    printf("Registration successful.\n");

    fclose(f);
}

void displayPackages() {
    printf("+---------+------------+----------+----------+\n");
    printf("| Package | Data Limit | Price    | Validity |\n");
    printf("+---------+------------+----------+----------+\n");
    printf("|    1    | 1 GB/Day   | Rs. 200  | 30 Days  |\n");
    printf("|    2    | 2 GB/Day   | Rs. 300  | 30 Days  |\n");
    printf("|    3    | 2 GB/Day   | Rs. 500  | 3 Months |\n");
    printf("|    4    | 2 GB/Day   | Rs. 700  | 6 Months |\n");
    printf("|    5    | 2 GB/Day   | Rs. 1100 | 1 Year   |\n");
    printf("+---------+------------+----------+----------+\n");
}

void displayPaymentRecords() {
    FILE *paymentFile;
    char ch;
    paymentFile = fopen("poisaa_records.txt", "r");
    if (paymentFile == NULL) {
        printf("Error opening payment records file.\n");
        exit(1);
    }

    printf("Phone Number\t\tAmount\t\tPackage\t\tTimestamp\n");
    for (int i = 0; i < 82; i++)
        printf("-");
    printf("\n");

    while ((ch = fgetc(paymentFile)) != EOF) {
        putchar(ch);
    }
    printf("\n");
    for (int i = 0; i < 82; i++)
        printf("-");

    fclose(paymentFile);
}

int main() {
    int choice;

    printf("\n\n\n\n\n\n\n\n\n**********************");
    printf("\n\t\t------WELCOME TO THE GEETHANJALI TELECOM BILLING MANAGEMENT SYSTEM-----");
    printf("\n\t\t*********************");

    do {
        printf("\n Enter\n");
        printf(" 1 : for list of records\n");
        printf(" 2 : for payment\n");
        printf(" 3 : for searching records.\n");
        printf(" 4 : for registering a new subscriber\n");
        printf(" 5 : for displaying available packages\n");
        printf(" 6 : for payments records\n");
        printf(" 7 : for exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                listRecords();
                break;
            case 2:
                payment();
                break;
            case 3:
                searchRecords();
                break;
            case 4:
                registerSubscriber();
                break;
            case 5:
                displayPackages();
                break;
            case 6:
                displayPaymentRecords();
                break;
            case 7:
                printf("\n\n\t\t\t\tTHANK YOU FOR USING GEETHANJALI TELECOM SERVICES\n");
                exit(0);
            default:
                printf("Incorrect Input\n");
                break;
        }
    } while (choice != 7);

    return 0;
}
