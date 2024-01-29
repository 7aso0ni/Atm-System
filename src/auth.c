#include <termios.h>
#include "header.h"
#include <unistd.h>

void loginMenu(char a[50], char pass[50])
{
    struct termios oflags, nflags;

loginMenu:
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

    // restore terminal
    if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0)
    {
        perror("tcsetattr");
        exit(1);
    }

    if (strcmp(a, getUsername(a)) != 0 || strcmp(pass, getPassword(pass)) != 0)
    {
        printf("\n\n\t\t\t\tIncorrect Username or Password\n");
        printf("\t\t\t\tPlease Try Again\n");
        sleep(2);
        goto loginMenu;
    }
    else
    {
        struct User u;
        if (getUserID(a) == -1)
        {
            printf("Error getting user id\n");
            exit(1);
        }
        u.id = getUserID(a);
        strcpy(u.name, getUsername(a));
        strcpy(u.password, getPassword(pass));

        success(u);
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
        printf("\n\n\n\n\t\t\tUser already exists\n");
        printf("\t\t\tPlease Choose another userame\n");
        sleep(2);
        goto userExists;
    }

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

    fseek(fp, 0, SEEK_END);
    if (ftell(fp) == 0) // if file is empty
    {
        fprintf(fp, "%d %s %s", newID, a, pass);
    }
    else
    {
        fprintf(fp, "\n%d %s %s", newID + 1, a, pass);
    }

    fseek(fp, 0, SEEK_SET); // set the file pointer to the beginning of the file
    fclose(fp);

    success(u);
}

const char *getPassword(char pass[50])
{
    FILE *fp;
    struct User userChecker;

    if ((fp = fopen("./data/users.txt", "r")) == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }

    while (fscanf(fp, "%d %s %s", &userChecker.id, userChecker.name, userChecker.password) == 3)
    {
        // if password is correct return the password
        if (strcmp(userChecker.password, pass) == 0)
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
    return "Incorrect Password";
}

const char *getUsername(char a[50])
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

        // if username is correct return the username
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

int getUserID(char a[50])
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

        // if username is correct return the username
        if (strcmp(userChecker.name, a) == 0)
        {
            fclose(fp);
            int duplicate = userChecker.id;
            if (duplicate <= 0)
            {
                fprintf(stderr, "Something went wrong\n");
                exit(1);
            }
            return duplicate;
        }
    }
    fclose(fp);
    return -1;
}
