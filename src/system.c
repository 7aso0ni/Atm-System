#include "header.h"
#include <unistd.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

const char *RECORDS = "./data/records.txt";

int getAccountFromFile(FILE *ptr, char name[50], struct Record *r)
{
    return fscanf(ptr, "%d %d %s %d %d/%d/%d %s %d %lf %s",
                  &r->id,
                  &r->userId,
                  name,
                  &r->accountNbr,
                  &r->deposit.month,
                  &r->deposit.day,
                  &r->deposit.year,
                  r->country,
                  &r->phone,
                  &r->amount,
                  r->accountType) != EOF;
}

void saveAccountToFile(FILE *ptr, struct User u, struct Record r)
{

    fprintf(ptr, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n",
            r.id,
            u.id,
            u.name,
            r.accountNbr,
            r.deposit.month,
            r.deposit.day,
            r.deposit.year,
            r.country,
            r.phone,
            r.amount,
            r.accountType);
}

void stayOrReturn(int notGood, void f(struct User u), struct User u)
{
    int option;
    if (notGood == 0)
    {
        system("clear");
        printf("\n✖ Record not found!!\n");
    invalid:
        printf("\nEnter 0 to try again, 1 to return to main menu and 2 to exit:");
        scanf("%d", &option);
        if (option == 0)
            f(u);
        else if (option == 1)
            mainMenu(u);
        else if (option == 2)
            exit(0);
        else
        {
            printf("Insert a valid operation!\n");
            goto invalid;
        }
    }
    else
    {
        printf("\nEnter 1 to go to the main menu and 0 to exit:");
        scanf("%d", &option);
    }
    if (option == 1)
    {
        system("clear");
        mainMenu(u);
    }
    else
    {
        system("clear");
        exit(1);
    }
}

void success(struct User u)
{
    int option;
    printf("\n✔ Success!\n\n");
invalid:
    printf("Enter 1 to go to the main menu and 0 to exit!\n");
    scanf("%d", &option);
    system("clear");
    if (option == 1)
    {
        mainMenu(u);
    }
    else if (option == 0)
    {
        exit(1);
    }
    else
    {
        printf("Insert a valid operation!\n");
        goto invalid;
    }
}

void createNewAcc(struct User u)
{
    struct Record r;
    struct Record cr;
    char userName[50];

    char accountTypeID;
    int interestRate;
    char dateInStr[11]; // 10 characters for mm/dd/yyyy and 1 for the null terminator
    char accNumInStr[8];
    char phoneInStr[9];
    char amountInStr[100];
    FILE *pf = fopen(RECORDS, "a+");

noAccount:
    system("clear");
    printf("\t\t\t===== New record =====\n");

    printf("\nEnter today's date(mm/dd/yyyy):");
    scanf("%s", dateInStr);
    if (hasNonDigitChars(dateInStr, true))
    {
        printf("\n\n\n\n\n\t\t\tOnly valid numbers are allowed.\n\n\n");
        fflush(stdout);
        sleep(2);
        system("clear");
        goto noAccount;
    }

    int month, day, year;
    int assigned = sscanf(dateInStr, "%d/%d/%d", &month, &day, &year);

    if (assigned < 3)
    {
        printf("\n\n\n\n\n\t\t\tIncorrect date format. Please use mm/dd/yyyy.\n\n\n\n");
        fflush(stdout);
        sleep(2);
        goto noAccount;
    }

    if (month < 1 || month > 12 || day < 1 || day > 31 || year < 1900 || year > 2024)
    {
        printf("\n\n\n\n\n\t\t\t\tInvalid date!\n\n\n\n");
        fflush(stdout);
        sleep(2);
        goto noAccount;
    }
    r.deposit.month = month;
    r.deposit.day = day;
    r.deposit.year = year;

InvalidAccountNumber:
    printf("\nEnter the account number:");
    scanf("%s", &accNumInStr);
    if (hasNonDigitChars(accNumInStr, false))
    {
        printf("\n\n\n\n\n\t\t\tOnly valid numbers are allowed.\n\n\n");
        fflush(stdout);
        sleep(2);
        system("clear");
        goto InvalidAccountNumber;
    }
    r.accountNbr = atoi(accNumInStr);

    if (r.accountNbr < 0)
    {
        printf("\n\n\n\n\t\t\t\tInvalid Account Number!\n");
        printf("\t\t\t\tHas to be a positive number\n\n\n\n");
        fflush(stdout);
        sleep(3);
        system("clear");
        goto InvalidAccountNumber;
    }

    if (r.accountNbr < 1000000 || r.accountNbr > 9999999)
    {
        printf("\n\n\n\n\t\t\t\tInvalid Account Number!\n");
        printf("\t\t\t\tHas to be 7 digits\n\n\n\n");
        fflush(stdout);
        sleep(3);
        system("clear");
        goto InvalidAccountNumber;
    }

    while (getAccountFromFile(pf, userName, &cr))
    {
        if (strcmp(userName, u.name) == 0 && cr.accountNbr == r.accountNbr)
        {
            printf("\n\n\n\n\n\t\t\t✖ This Account already exists for this user\n\n\n");
            fflush(stdout);
            sleep(2);
            system("clear");
            goto InvalidAccountNumber;
        }
    }
    printf("\nEnter the country:");
    scanf("%s", r.country);

InvalidPhoneNumber:
    printf("\nEnter the phone number:");
    scanf("%s", &phoneInStr);
    if (hasNonDigitChars(phoneInStr, false))
    {
        printf("\n\n\n\n\t\t\tOnly valid numbers are allowed.\n\n\n");
        fflush(stdout);
        sleep(2);
        system("clear");
        goto InvalidPhoneNumber;
    }

    r.phone = atoi(phoneInStr);
    if (r.phone < 9999999 || r.phone > 100000000)
    {
        printf("\n\n\n\n\t\t\t\tInvalid Phone Number!\n");
        printf("\t\t\t\tHas to be 8 digit number\n\n\n\n");
        fflush(stdout);
        sleep(3);
        system("clear");
        goto InvalidPhoneNumber;
    }
InvalidAmount:
    printf("\nEnter amount to deposit: $");
    scanf("%s", &amountInStr);

    if (hasNonDigitChars(amountInStr, false))
    {
        printf("\n\n\n\n\t\t\tOnly valid numbers are allowed.\n\n\n");
        fflush(stdout);
        sleep(2);
        system("clear");
        goto InvalidAmount;
    }
    r.amount = atof(amountInStr);

    if (r.amount < 0 || r.amount > 10000000)
    {
        printf("\n\n\n\n\n\t\t\t\tInvalid amount\n\n\n\n");
        fflush(stdout);
        sleep(2);
        system("clear");
        goto InvalidAmount;
    }

    sprintf(amountInStr, "%.2f", r.amount);
invalidAccountType:
    printf("\nChoose the type of account:\n\t1: saving\n\t2: current\n\t3: fixed01(for 1 year)\n\t4: fixed02(for 2 years)\n\t5: fixed03(for 3 years)\n\n\tEnter your choice:");
    scanf("%s", &accountTypeID);

    if (hasNonDigitChars(&accountTypeID, false))
    {
        printf("\n\n\n\n\t\t\tOnly numbers are allowed.\n\n\n");
        fflush(stdout);
        sleep(2);
        system("clear");
        goto invalidAccountType;
    }

    int num = atoi(&accountTypeID);
    switch (num)
    {
    case 1:
        strcpy(r.accountType, "saving");
        interestRate = 0.17;
        break;
    case 2:
        strcpy(r.accountType, "current");
        printf("\nYou will not get interests because the account is of type current\n");
        sleep(2);
        break;
    case 3:
        strcpy(r.accountType, "fixed01");
        interestRate = 0.14;
        break;
    case 4:
        strcpy(r.accountType, "fixed02");
        interestRate = 0.15;
        break;
    case 5:
        strcpy(r.accountType, "fixed03");
        interestRate = 0.18;
        break;
    default:
        printf("\n\n\n\n\n\n\t\t\t\tInvalid Account Type\n");
        printf("\t\t\t\tPlease Enter a valid type\n\n\n\n\n");
        fflush(stdout);
        sleep(2);
        system("clear");
        goto invalidAccountType;
    }

    // TODO: make it fixed digits. currently it is not
    r.id = generateUniqueRecordID();

    saveAccountToFile(pf, u, r);

    fclose(pf);
    success(u);
}

void updateAccountInfo(struct User u)
{
    struct Record r;
    struct Record cr;

    FILE *originalFile = fopen(RECORDS, "r");
    FILE *tempFile = fopen("./data/temp.txt", "w");

    if (originalFile == NULL || tempFile == NULL)
    {
        printf("Error opening file\n");
        exit(1);
    }

    int changeType;
    char newAccNumInStr[50];
    char userName[50];

InvalidAccountNumber:
    printf("\n\n\t\t\t===== Update Account =====\n");
    printf("\n\nEnter the following number to change: \n\n\t1: Account number\n\t2: Country\n\t3: Phone number\n\t4: Amount\n\t5: Account type\n\n\tEnter your choice:");
    scanf("%d", &changeType);
    switch (changeType)
    {
    case 1:
        printf("\nEnter the account number:");
        scanf("%s", &newAccNumInStr);
        if (hasNonDigitChars(newAccNumInStr, false))
        {
            printf("\n\n\n\n\n\t\t\tOnly valid numbers are allowed.\n\n\n");
            fflush(stdout);
            sleep(2);
            system("clear");
            goto InvalidAccountNumber;
        }
        r.accountNbr = atoi(newAccNumInStr);

        if (r.accountNbr < 0)
        {
            printf("\n\n\n\n\t\t\t\tInvalid Account Number!\n");
            printf("\t\t\t\tHas to be a positive number\n\n\n\n");
            fflush(stdout);
            sleep(3);
            system("clear");
            goto InvalidAccountNumber;
        }

        if (r.accountNbr < 1000000 || r.accountNbr > 9999999)
        {
            printf("\n\n\n\n\t\t\t\tInvalid Account Number!\n");
            printf("\t\t\t\tHas to be 7 digits\n\n\n\n");
            fflush(stdout);
            sleep(3);
            system("clear");
            goto InvalidAccountNumber;
        }

        while (getAccountFromFile(originalFile, userName, &cr))
        {
            if (strcmp(userName, u.name) == 0 && cr.accountNbr == r.accountNbr)
            {
                printf("\n\n\n\n\n\t\t\t✖ This Account already exists for this user\n\n\n");
                fflush(stdout);
                sleep(2);
                system("clear");
                goto InvalidAccountNumber;
            }
        }
        break;
    case 2:
    default:
        printf("\n\n\n\n\n\t\t\t\tInvalid Type!\n\n\n\n");
        fflush(stdout);
        sleep(2);
        system("clear");
        goto InvalidAccountNumber;
    }
}

void checkAllAccounts(struct User u)
{
    char userName[100];
    struct Record r;

    FILE *pf = fopen("./data/records.txt", "r");

    system("clear");
    printf("\t\t====== All accounts from user, %s =====\n\n", u.name);
    while (getAccountFromFile(pf, userName, &r))
    {
        if (strcmp(userName, u.name) == 0)
        {
            printf("_____________________\n");
            printf("\nAccount number:%d\nDeposit Date:%d/%d/%d \ncountry:%s \nPhone number:%d \nAmount deposited: $%.2f \nType Of Account:%s\n",
                   r.accountNbr,
                   r.deposit.day,
                   r.deposit.month,
                   r.deposit.year,
                   r.country,
                   r.phone,
                   r.amount,
                   r.accountType);
        }
    }
    fclose(pf);
    success(u);
}

// methods
bool hasNonDigitChars(const char *str, bool allowSlash)
{
    while (*str)
    {
        if (!isdigit((unsigned char)*str) && *str != '-' && (!allowSlash || *str != '/'))
        {
            return true;
        }
        str++;
    }
    return false;
}

int generateUniqueRecordID()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);

    // Use the current timestamp (seconds since epoch)
    int uniqueID = (int)tv.tv_sec;

    // Add microseconds to make it more unique
    uniqueID = uniqueID * 1000000 + (int)tv.tv_usec;

    // Add process ID to further ensure uniqueness
    uniqueID = uniqueID * 1000 + getpid();

    // Add a counter to handle cases where IDs are generated rapidly
    static int counter = 0;
    uniqueID = uniqueID * 1000 + counter;
    counter++;

    return abs(uniqueID);
}
