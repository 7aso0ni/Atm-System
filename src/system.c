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

int getAccountFromFileByAccountID(FILE *ptr, struct Record *r)
{
    return fscanf(ptr, "%d %d %s %d %d/%d/%d %s %d %lf %s",
                  &r->id,
                  &r->userId,
                  r->name,
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
    // float amount;
    char amountInStr[100];
    FILE *pf = fopen(RECORDS, "a+");

noAccount:
    system("clear");
    printf("\t\t\t===== New record =====\n");

    printf("\nEnter today's date(mm/dd/yyyy):");
    scanf("%s", dateInStr);
    if (hasNonDigitChars(dateInStr, true, false))
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
    if (hasNonDigitChars(accNumInStr, false, false))
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
InvalidCountry:
    printf("\nEnter the country:");
    scanf("%s", r.country);

    if (!hasNonDigitChars(r.country, false, false))
    {
        printf("\n\n\n\n\t\t\t\tInvalid Country Name!\n");
        printf("\t\t\t\tHas to be a string\n\n\n\n");
        fflush(stdout);
        sleep(3);
        system("clear");
        goto InvalidCountry;
    }

    if (strlen(r.country) < 2 || strlen(r.country) > 99)
    {
        printf("\n\n\n\n\t\t\t\tInvalid Country Name!\n");
        printf("\t\t\t\tHas to be between 2 and 99 characters\n\n\n\n");
        fflush(stdout);
        sleep(3);
        system("clear");
        goto InvalidCountry;
    }

InvalidPhoneNumber:
    printf("\nEnter the phone number:");
    scanf("%s", &phoneInStr);
    if (hasNonDigitChars(phoneInStr, false, false))
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

    if (hasNonDigitChars(amountInStr, false, true))
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

    if (hasNonDigitChars(&accountTypeID, false, false))
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
    FILE *fp;
    struct Record records[100]; // Assuming a maximum of 100 records for simplicity
    struct Record record;

    char accType[20]; // save account type of modified user account
    char input[50];
    int accountId;
    char userName[50];
    int totalRecords = 0;
    int foundIndex = -1;

    while (foundIndex == -1)
    {
        printf("Enter the account ID you want to update:");
        scanf("%s", input);

        accountId = atoi(input); // Convert string to int
        fp = fopen(RECORDS, "r");
        if (fp == NULL)
        {
            printf("Error opening file!\n");
            return;
        }
        totalRecords = 0;
        bool found = false;

        while (getAccountFromFile(fp, userName, &record))
        {
            strcpy(record.name, userName); // Store the user's name in the record
            records[totalRecords] = record;
            if (record.accountNbr == accountId && strcmp(userName, u.name) == 0)
            {
                strcpy(accType, record.accountType);
                foundIndex = totalRecords;
                found = true;
            }
            totalRecords++;
        }
        fclose(fp);

        if (!found)
        {
            printf("This account does not exist\n");
            sleep(3);
        }
    }

invaildChoice:
    printf("Select the field to update:\n 1: Country\n 2: Phone number\n");
    int choice;
    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
    invalidCountry:
        printf("Enter new country: ");
        scanf("%s", records[foundIndex].country);
        // Validate country name
        if (strlen(records[foundIndex].country) < 2 || strlen(records[foundIndex].country) > 99)
        {
            printf("Invalid country name! Country name should be between 2 and 99 characters.\n");
            fflush(stdout);
            sleep(3);
            goto invalidCountry;
        }
        // Check if country name contains any digits
        for (int i = 0; records[foundIndex].country[i]; i++)
        {
            if (isdigit(records[foundIndex].country[i]))
            {
                printf("Invalid country name! Country name should not contain any digits.\n");
                fflush(stdout);
                sleep(3);
                goto invalidCountry;
            }
        }
        break;

    case 2:
    invalidPhoneNumber:
        printf("Enter new phone number: ");
        scanf("%d", &records[foundIndex].phone);
        // Validate phone number
        if (records[foundIndex].phone < 9999999 || records[foundIndex].phone > 100000000)
        {
            printf("Invalid phone number! Phone number should be a exactly 8 digits.\n");
            fflush(stdout);
            sleep(3);
            system("clear");
            goto invalidPhoneNumber;
        }
        // Check if phone number contains any letters
        char phoneNumberString[20];
        sprintf(phoneNumberString, "%d", records[foundIndex].phone);
        for (int i = 0; phoneNumberString[i]; i++)
        {
            if (isalpha(phoneNumberString[i]))
            {
                printf("Invalid phone number! Phone number should not contain any letters.\n");
                fflush(stdout);
                sleep(3);
                system("clear");
                goto invalidPhoneNumber;
            }
        }
        break;

    default:
        printf("Invalid choice!\n Please enter a valid choice\n");
        fflush(stdout);
        sleep(3);
        system("clear");
        goto invaildChoice;
    }

    // Write the entire array back to the file
    fp = fopen(RECORDS, "w");
    if (fp == NULL)
    {
        printf("Error opening file!\n");
        sleep(5);
        return;
    }

    for (int i = 0; i < totalRecords; i++)
    {
        struct User tempUser; // Temporary User to pass the correct name to saveAccountToFile
        strcpy(tempUser.name, records[i].name);
        tempUser.id = records[i].userId; // Assuming you have a userId field
        // ... copy other necessary User fields if there are any ...
        if (foundIndex == i)
        {
            strcpy(records[i].accountType, accType);
        }
        saveAccountToFile(fp, tempUser, records[i]);
    }
    fclose(fp);
    printf("Account information updated successfully!\n");
    success(u);
}

void checkAccountDetails(struct User u)
{
    FILE *fp;
    struct Record records[100]; // Assuming a maximum of 100 records for simplicity
    struct Record record;

    char input[50];
    int accountId;
    char userName[50];
    int totalRecords = 0;
    int foundIndex = -1;

    while (foundIndex == -1)
    {
        printf("Enter the account ID you want to check:");
        scanf("%s", input);

        accountId = atoi(input); // Convert string to int
        fp = fopen(RECORDS, "r");
        if (fp == NULL)
        {
            printf("Error opening file!\n");
            return;
        }
        totalRecords = 0;
        bool found = false;

        while (getAccountFromFile(fp, userName, &record))
        {
            strcpy(record.name, userName); // Store the user's name in the record
            records[totalRecords] = record;
            if (record.accountNbr == accountId && strcmp(userName, u.name) == 0)
            {
                foundIndex = totalRecords;
                found = true;
            }
            totalRecords++;
        }
        fclose(fp);

        if (!found)
        {
            printf("This account does not exist\n");
            sleep(3);
        }
    }

    // Calculate interest rate
    double interestAmount;
    int yearFromDeposit;

    printf("==============Account details=================\n");
    printf("Account number: %d\n", records[foundIndex].accountNbr);
    printf("Deposit date: %d/%d/%d\n", records[foundIndex].deposit.month, records[foundIndex].deposit.day, records[foundIndex].deposit.year);
    printf("Country: %s\n", records[foundIndex].country);
    printf("Phone number: %d\n", records[foundIndex].phone);
    printf("Amount deposited: $%.2f\n", records[foundIndex].amount);
    printf("Type of account: %s\n", records[foundIndex].accountType);

    if (strcmp(records[foundIndex].accountType, "saving") == 0)
    {
        // calculate monthly interest rate
        yearFromDeposit = records[foundIndex].deposit.year;
        interestAmount = (records[foundIndex].amount * 0.07 / 12);
        printf("You will gain $%.2lf of interest on day 10 of every month.\n", interestAmount);
    }
    else if (strcmp(records[foundIndex].accountType, "fixed01") == 0)
    {
        // calculate 1 year from account creation
        yearFromDeposit = records[foundIndex].deposit.year + 1;
        interestAmount = records[foundIndex].amount * 0.04;
        printf("You will gain $%.2lf interest on %02d/%02d/%04d (1 year from account creation).\n", interestAmount, record.deposit.month, record.deposit.day, yearFromDeposit);
    }
    else if (strcmp(records[foundIndex].accountType, "fixed02") == 0)
    {
        // calculate 2 years from account creation
        yearFromDeposit = records[foundIndex].deposit.year + 2;
        interestAmount = (records[foundIndex].amount * 0.05 * 2);
        printf("You will gain $%.2lf interest on %02d/%02d/%04d (2 year from account creation).\n", interestAmount, record.deposit.month, record.deposit.day, yearFromDeposit);
    }
    else if (strcmp(records[foundIndex].accountType, "fixed03") == 0)
    {
        // calculate 3 years from account creation
        yearFromDeposit = records[foundIndex].deposit.year + 3;
        interestAmount = records[foundIndex].amount * 0.08 * 3;
        printf("You will gain $%.2lf interest on %02d/%02d/%04d (3 year from account creation).\n", interestAmount, record.deposit.month, record.deposit.day, yearFromDeposit);
    }
    else
    {
        printf("You will not get any interest because the account is of type current\n");
    }
    sleep(5);
    success(u);
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

void makeTransaction(struct User u)
{
    FILE *fp;
    struct Record records[100]; // Assuming a maximum of 100 records for simplicity
    struct Record record;

    char input[50];
    int accountId;
    char userName[50];
    int totalRecords = 0;
    int foundIndex = -1;

    while (foundIndex == -1)
    {
        printf("Enter the account ID you want to make a transaction:");
        scanf("%s", input);

        accountId = atoi(input); // Convert string to int
        fp = fopen(RECORDS, "r");
        if (fp == NULL)
        {
            printf("Error opening file!\n");
            return;
        }
        totalRecords = 0;
        bool found = false;

        while (getAccountFromFile(fp, userName, &record))
        {
            strcpy(record.name, userName); // Store the user's name in the record
            records[totalRecords] = record;
            if (record.accountNbr == accountId && strcmp(userName, u.name) == 0)
            {
                foundIndex = totalRecords;
                found = true;
            }
            totalRecords++;
        }
        fclose(fp);

        if (!found)
        {
            printf("This account does not exist\n");
            sleep(3);
        }
    }

    char transactionType;
    double amount;
    char amountInStr[100];
    char transactionTypeInStr[2];
    char transactionDate[11]; // 10 characters for mm/dd/yyyy and 1 for the null terminator

    printf("Enter the transaction type (D for deposit, W for withdraw): ");
    scanf("%s", transactionTypeInStr);
    transactionType = transactionTypeInStr[0];

    if (transactionType != 'D' && transactionType != 'W')
    {
        printf("Invalid transaction type! Please enter D for deposit or W for withdraw.\n");
        sleep(3);
        makeTransaction(u);
        return;
    }

invalidAmount:
    if (transactionType == 'D')
    {
        printf("Enter the amount to deposit: $");
    }
    else
    {
        printf("Enter the amount to withdraw: $");
    }
    scanf("%s", amountInStr);

    if (hasNonDigitChars(amountInStr, false, true))
    {
        printf("Invalid amount! Please enter a valid amount.\n");
        sleep(3);
        system("clear");
        goto invalidAmount;
        return;
    }

    amount = atof(amountInStr); // Convert string to double

    if (amount < 0 || amount > 10000000)
    {
        printf("Invalid amount! Please enter an amount between $0 and $10,000,000.\n");
        sleep(3);
        system("clear");
        goto invalidAmount;
        return;
    }

    if (transactionType == 'W' && amount > records[foundIndex].amount)
    {
        printf("Insufficient funds! You cannot withdraw more than the current balance.\n");
        sleep(3);
        system("clear");
        goto invalidAmount;
        return;
    }
    else if (transactionType == 'W' && amount == records[foundIndex].amount)
    {
        printf("Warning: Your account balance will be zero after this withdrawal.\n");
        sleep(3);
    }

    // Update the account balance
    if (transactionType == 'D')
    {
        records[foundIndex].amount += amount;
    }
    else
    {
        records[foundIndex].amount -= amount;
    }

    // Write the entire array back to the file
    fp = fopen(RECORDS, "w");
    if (fp == NULL)
    {
        printf("Error opening file!\n");
        fflush(stdout);
        sleep(5);
        return;
    }

    for (int i = 0; i < totalRecords; i++)
    {
        struct User tempUser; // Temporary User to pass the correct name to saveAccountToFile
        strcpy(tempUser.name, records[i].name);
        tempUser.id = records[i].userId; // Assuming you have a userId field
        saveAccountToFile(fp, tempUser, records[i]);
    }
    fclose(fp);
    printf("Transaction completed successfully!\n");
    fflush(stdout);
    success(u);
}

void removeAccount(struct User u)
{
    FILE *fp;
    struct Record records[100]; // Assuming a maximum of 100 records for simplicity
    struct Record record;

    char input[50];
    int accountId;
    char userName[50];
    int totalRecords = 0;
    int foundIndex = -1;

    while (foundIndex == -1)
    {
        printf("Enter the account ID you want to remove:");
        scanf("%s", input);

        accountId = atoi(input); // Convert string to int
        fp = fopen(RECORDS, "r");
        if (fp == NULL)
        {
            printf("Error opening file!\n");
            return;
        }
        totalRecords = 0;
        bool found = false;

        while (getAccountFromFile(fp, userName, &record))
        {
            strcpy(record.name, userName); // Store the user's name in the record
            records[totalRecords] = record;
            if (record.accountNbr == accountId && strcmp(userName, u.name) == 0)
            {
                foundIndex = totalRecords;
                found = true;
            }
            totalRecords++;
        }
        fclose(fp);

        if (!found)
        {
            printf("This account does not exist\n");
            sleep(3);
        }
    }

    // Remove the account from the file
    fp = fopen(RECORDS, "w");
    if (fp == NULL)
    {
        printf("Error opening file!\n");
        sleep(5);
        return;
    }
    for (int i = 0; i < totalRecords; i++)
    {
        struct User tempUser; // Temporary User to pass the correct
        strcpy(tempUser.name, records[i].name);
        tempUser.id = records[i].userId; // Assuming you have a userId field
        // ... copy other necessary User fields if there are any ...
        if (foundIndex != i)
        {
            saveAccountToFile(fp, tempUser, records[i]);
        }
    }
    fclose(fp);
    printf("Account removed successfully!\n");
    success(u);
}

void transferOwner(struct User u)
{
    FILE *fp;
    struct Record records[100]; // Assuming a maximum of 100 records for simplicity
    struct Record record;

    char input[50];
    int accountId;
    char userName[50];
    int totalRecords = 0;
    int foundIndex = -1;

    // Loop until a valid account ID and owner is found
    while (foundIndex == -1)
    {
        printf("Enter the account ID you want to transfer ownership: ");
        scanf("%49s", input);

        accountId = atoi(input); // Convert string to int
        fp = fopen(RECORDS, "r");
        if (fp == NULL)
        {
            printf("Error opening file!\n");
            fflush(stdout);
            return;
        }
        totalRecords = 0;
        bool found = false;

        // Read records from the file and find the target account
        while (getAccountFromFile(fp, userName, &record))
        {

            strcpy(records[totalRecords].name, userName); // Store the user's name in the record array
            records[totalRecords] = record;               // Store the record in the record array

            if (record.accountNbr == accountId && strcmp(userName, u.name) == 0)
            {
                foundIndex = totalRecords;
                found = true;
                printf("Debug: Match found - AccountNbr: %d, Index: %d\n", accountId, foundIndex);
            }
            totalRecords++;
            if (totalRecords >= 100)
                break; // Ensure we do not exceed array bounds
        }
        fclose(fp);

        if (!found)
        {
            printf("This account does not exist or does not belong to you.\n");
            fflush(stdout);
            sleep(3);
        }
    }

    char newOwner[50];
    char newOwnerName[50];
    struct Record newOwnerUser;
    int ownerFoundIndex = 0; // Flag to indicate if the new owner is found
    bool ownerFound = false;

ownerNotFound:
    printf("Enter the new owner's name: ");
    scanf("%49s", newOwner);

    // Open the file again to search for the new owner
    fp = fopen(RECORDS, "r");
    if (fp == NULL)
    {
        printf("Error opening file!\n");
        fflush(stdout);
        return;
    }

    while (getAccountFromFile(fp, newOwnerName, &newOwnerUser))
    {
        // Check if the new owner exists
        if (strcmp(newOwner, newOwnerName) == 0)
        {
            strcpy(records[ownerFoundIndex].name, newOwnerName); // Assign the new owner's name

            ownerFound = true;
            break; // Break once the correct user is found
        }
        ownerFoundIndex++;
    }
    fclose(fp);

    if (!ownerFound)
    {
        printf("The new owner does not exist.\n");
        fflush(stdout);
        goto ownerNotFound;
    }

    // Reopen the file in write mode to update the records
    fp = fopen(RECORDS, "w");
    if (fp == NULL)
    {
        printf("Error opening file for writing!\n");
        fflush(stdout);
        return;
    }

    // Update the ownership and save all records back to the file
    for (int i = 0; i < totalRecords; i++)
    {
        struct User tempUser; // Temporary User to pass the correct name to saveAccountToFile
        strcpy(tempUser.name, records[i].name);
        tempUser.id = records[i].userId;

        if (foundIndex == i)
        {
            strcpy(tempUser.name, newOwnerName);
            tempUser.id = newOwnerUser.userId; // Assign the new owner's ID
        }

        printf("Debug: Saving record - AccountNbr: %d, UserName: %s\n", records[i].accountNbr, tempUser.name);
        saveAccountToFile(fp, tempUser, records[i]);
    }

    fclose(fp);
    printf("Account ownership transferred successfully!\n");
    fflush(stdout);

    success(u);
}

// methods
bool hasNonDigitChars(const char *str, bool allowSlash, bool allowDot)
{
    while (*str)
    {
        if (!isdigit((unsigned char)*str) && *str != '-' && (!allowSlash || *str != '/') && (!allowDot || *str != '.'))
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

void appendToTransactionRecords(const char *line)
{
    FILE *file = fopen("data/transactionsrecords.txt", "a");
    if (file == NULL)
    {
        // Handle error opening the file
        printf("Error opening the transaction records file!\n");
        return;
    }
    // Append the line to the file
    fprintf(file, "%s\n", line);
    // Close the file
    fclose(file);
}
