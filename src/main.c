#include "header.h"
#include <unistd.h>
#include <signal.h>
;

void handleSigInt(int sig)
{
    printf("\n\nGoodbye :(\n\n");
    fflush(stdout); // Flushes the output buffer

    exit(1);
}

void mainMenu(struct User u)
{

mainMenu:
    int option;
    system("clear");
    printf("\n\n\t\t======= ATM =======\n\n");
    printf("\n\t\t-->> Feel free to choose one of the options below <<--\n");
    printf("\n\t\t[1]- Create a new account\n");
    printf("\n\t\t[2]- Update account information\n");
    printf("\n\t\t[3]- Check accounts\n");
    printf("\n\t\t[4]- Check list of owned account\n");
    printf("\n\t\t[5]- Make Transaction\n");
    printf("\n\t\t[6]- Remove existing account\n");
    printf("\n\t\t[7]- Transfer ownership\n");
    printf("\n\t\t[8]- Exit\n");
    scanf(" %c", &option);

    if (hasNonDigitChars(&option, false, false))
    {
        printf("Invalid operation!\n");
        fflush(stdout);
        while (getchar() != '\n'); // clear the buffer
        sleep(1);
        goto mainMenu;
    }

    int optionNumber = atoi(&option);

    switch (optionNumber)
    {
    case 1:
        createNewAcc(u);
        break;
    case 2:
        updateAccountInfo(u);
        break;
    case 3:
        // student TODO : add your **Check the details of existing accounts** function
        checkAccountDetails(u);
        break;
    case 4:
        checkAllAccounts(u);
        break;
    case 5:
        // student TODO : add your **Make transaction** function
        makeTransaction(u);
        break;
    case 6:
        // student TODO : add your **Remove existing account** function
        removeAccount(u);
        break;
    case 7:
        // student TODO : add your **Transfer owner** function
        transferOwner(u);
        break;
    case 8:
        exit(1);
        break;
    default:
        printf("Invalid operation!\n");
        sleep(1);
        goto mainMenu;
    }
};

void initMenu(struct User *u)
{
initMenu:
    int r = 0;
    char option[2];
    system("clear");
    printf("\n\n\t\t======= ATM =======\n");
    printf("\n\t\t-->> Feel free to login / register :\n");
    printf("\n\t\t[1]- login\n");
    printf("\n\t\t[2]- register\n");
    printf("\n\t\t[3]- exit\n");
    while (!r)
    {
        scanf("%s", &option);
        int optionNum = atoi(option);
        switch (optionNum)
        {
        case 1:
            loginMenu(u->name, u->password);
            r = 1;
            break;
        case 2:
            // student TODO: add your **Registration** function
            registerMenu(u->name, u->password);
            r = 1;
            break;
        case 3:
            exit(1);
            break;
        default:
            system("clear");
            printf("Insert a valid operation!\n");
            fflush(stdout);
            sleep(1);
            goto initMenu;
        }
    }
};

int main()
{

    signal(SIGINT, handleSigInt);

    struct User u;

    initMenu(&u);
    mainMenu(u);
    return 0;
}
