#include <termios.h>
#include "header.h"
#include <unistd.h>

char *USERS = "./data/users.txt";

void loginMenu(char a[50], char pass[50])
{
    struct termios oflags, nflags;

    system("clear");
    printf("\n\n\n\t\t\t\t   Bank Management System\n\t\t\t\t\t User Login:");
    scanf("%s", a);
    // fgets(a, 50, stdin); // fgets to avoid buffer overflow

    // disabling echo
    tcgetattr(fileno(stdin), &oflags);
    nflags = oflags;
    nflags.c_lflag &= ~ECHO;
    nflags.c_lflag |= ECHONL;

    if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0)
    {
        perror("tcsetattr");
        exit(1);
    }
    printf("\n\n\n\n\n\t\t\t\tEnter the password to login:");

    scanf("%s", pass);

    // fgets(pass, 50, stdin);

    // restore terminal
    if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0)
    {
        perror("tcsetattr");
        exit(1);
    }
};

void registerMenu(char a[50], char pass[50])
{
    struct termios oflags, nflags;
    char newPass[50]; // Temporary buffer for the password
    int newID = 0;
// Clear screen
userExists:
    system("clear");
    printf("\n\n\n\t\t\t\t   ATM System\n\t\t\t\t\t User Registration:");

    scanf("%49s", a); // Limit input to 49 characters to avoid buffer overflow

    // Disabling echo for password input
    tcgetattr(fileno(stdin), &oflags);
    nflags = oflags;
    nflags.c_lflag &= ~ECHO;
    nflags.c_lflag |= ECHONL;

    if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0)
    {
        perror("tcsetattr");
        exit(1);
    }

    // Prompt for password
    printf("\n\n\n\n\n\t\t\t\tEnter the password to login:");
    scanf("%49s", newPass); // Use newPass to avoid direct writing to pass argument

    // Restoring terminal settings
    if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0)
    {
        perror("tcsetattr");
        exit(1);
    }

    // Copy the new password into the pass argument
    strncpy(pass, newPass, 50);
    pass[49] = '\0'; // Ensure null-termination

    // check if user exists
    if (strcmp(a, getUsername(a)) == 0)
    {
        printf("\nUser already exists\n");
        printf("Please Choose another userame\n");
        sleep(2);
        goto userExists;
    }

    // TODO: save into the file
    FILE *fp;
    struct User u;

    fp = fopen("./data/users.txt", "a+");
    if (fp == NULL)
    {
        printf("Error opening file\n");
        exit(1);
    }

    while (fscanf(fp, "%d %s %s", &u.id, u.name, u.password) == 3)
    {
        newID = u.id;
    }

    if (newID == 0)
    {
        fprintf(fp, "%d %s %s", newID, a, pass);
    }
    else
    {
        fprintf(fp, "\n%d %s %s", newID + 1, a, pass);
    }
    fclose(fp);
}

const char *getPassword(struct User u)
{
    FILE *fp;
    struct User userChecker;

    if ((fp = fopen("./data/users.txt", "r")) == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }

    while (fscanf(fp, "%d %s %s", &userChecker.id, userChecker.name, userChecker.password) != EOF)
    {
        if (strcmp(userChecker.name, u.name) == 0)
        {
            fclose(fp);

            char *dup = strdup(userChecker.password);
            if (dup == NULL)
            {
                fprintf(stderr, "Memory allocation failed\n");
                exit(1);
            }
            return dup;
        }
    }

    fclose(fp);
    return "no user found";
}

char *getUsername(char a[50])
{
    FILE *fp;
    struct User userChecker;

    if ((fp = fopen("./data/users.txt", "r")) == NULL)
    {
        printf("Error Opening a file\n");
        exit(1);
    }
    while (fscanf(fp, "%d %s %s", &userChecker.id, userChecker.name, userChecker.password) == 3)
    {
        if (strcmp(userChecker.name, a) == 0)
        {
            fclose(fp);
            char *duplicate = strdup(userChecker.name);
            if (duplicate == NULL)
            {
                fprintf(stderr, "Memory allocation failed\n");
                exit(1);
            }
            return duplicate;
        }
    }
    fclose(fp);
    return "no user found";
}
