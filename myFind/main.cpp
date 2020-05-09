/*
 * =====================================================================================
 *
 *       Filename:  getopt.c
 *
 *    Description:  getopt() Übungsbeispiel
 *
 *        Version:  1.0
 *        Created:  21/04/16 23:27:49 CEST
 *       Revision:  Tue 23 Apr 13:10:32 CEST 2019
 *       Compiler:  gcc
 *
 *         Author:  R. Pfeiffer (Mr), pfeiffer@technikum-wien.at
 *        Company:  https://www.technikum-wien.at/
 *
 * =====================================================================================
 */

/* GNU C Compiler  : gcc -Wall -o getopt_test getopt.c
   Clang C Compiler: clang -Wall -o getopt_test getopt.c
 */

#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <assert.h>
/* Include Datei für getopt(); getopt.h geht auch, ist aber älter. */
#include <unistd.h>
#include <string.h>

#ifndef PATH_MAX
#define PATH_MAX 255
#endif

char * returnCWDP();
char * sanitizePath(char* path);

void printResult(bool& fileFound, const char* path);
void simpleSearch(const char* fileName, const char* path, bool& fileFound);
void simpleSearch_CSInsensitive(const char* fileName, const char* path, bool& fileFound);
void searchFile(const char* fileName, const char* basePath, bool& fileFound);
void searchFile_CSInsensitive(const char* fileName, const char* basePath, bool& fileFound);

/* Verbose Flag wird global gesetzt, damit der komplette Code es sehen kann. */
unsigned short opt_verbose = 0;

/* Hilfsfunktion */
void print_usage( char *programm_name ) {
    printf("Usage: %s [-R] [-i] searchpath filename1 [filename2]...[filenameN]\n\n",programm_name);
    return;
}

/* Entry Point */
int main( int argc, char*argv[] ) {

    bool fileFound = false;

    int c;
    char *dateiname;
    char *programm_name;
    unsigned short Counter_Option_R = 0;
    unsigned short Counter_Option_i = 0;

    programm_name = argv[0];

    while ( (c = getopt( argc, argv, "Ri" )) != EOF ) {
        switch (c) {
            case '?':
                fprintf( stderr, "%s error: Unknown option.\n", programm_name );
                print_usage( programm_name );
                exit(1);
                /* Das break ist nach exit() eigentlich nicht mehr notwendig. */
                break;
            case 'R':
                Counter_Option_R++;
                printf("%s\n", argv);
//                print_usage( programm_name );
                printf("Parsing Option R with OPTIND = %i, OPTARG = %s\n", optind, optarg);

                break;
            case 'i':
                Counter_Option_i++;
                printf("Parsing Option i with OPTIND = %i, OPTARG = %s, ARGC = %i\n", optind, optarg, argc );
//                dateiname = optarg;
                break;
            default:
                /* assert() dient nur zu Debugzwecken und sollte nur dort eingesetzt sein,
                   wo etwas sicher niemals passieren darf. 0 ist in C immer gleich "logisch falsch". */
                assert(0);
        }
    }

    if ( ( Counter_Option_R > 1 ) || ( Counter_Option_i > 1 ) ) {
        fprintf( stderr, "%s Fehler: Optionen wurden mehrfach verwendet.\n", programm_name );
        exit(1);
    }

    if(Counter_Option_R == 1 && Counter_Option_i != 1)
    {
        printf("Base Path: %s\n", argv[optind+1]);
        printf("File Name: %s\n", argv[optind]);
        char *basePath = argv[optind+1];
        char *nameOfFile = argv[optind];

        searchFile(nameOfFile,basePath, fileFound);
        if(fileFound == false)
        {
            printf("File not found.\n");
        }
        printf("End of Program\n");
    }

    if(Counter_Option_R == 1 && Counter_Option_i == 1)
    {
        printf("Base Path: %s\n", argv[optind+1]);
        printf("File Name: %s\n", argv[optind]);
        char *basePath = argv[optind+1];
        char *nameOfFile = argv[optind];

        searchFile_CSInsensitive(nameOfFile,basePath, fileFound);
        if(fileFound == false)
        {
            printf("File not found.\n");
        }
        printf("End of Program\n");
    }

    if(Counter_Option_R != 1 && Counter_Option_i != 1)
    {
        printf("Base Path: %s\n", argv[optind+1]);
        printf("File Name: %s\n", argv[optind]);
        char *basePath = argv[optind+1];
        char *nameOfFile = argv[optind];

        simpleSearch(nameOfFile,basePath, fileFound);
        if(fileFound == false)
        {
            printf("File not found.\n");
        }
        printf("End of Program\n");
    }

    if(Counter_Option_R != 1 && Counter_Option_i == 1)
    {
        printf("Base Path: %s\n", argv[optind+1]);
        printf("File Name: %s\n", argv[optind]);
        char *basePath = argv[optind+1];
        char *nameOfFile = argv[optind];

        simpleSearch_CSInsensitive(nameOfFile,basePath, fileFound);
        if(fileFound == false)
        {
            printf("File not found.\n");
        }
        printf("End of Program\n");
    }




    /*
    if ( optind < argc ) {
        printf("ARGV Elemente ohne Optionen: ");
        while ( optind < argc ) {
            printf("%s ", argv[optind++]);
        }
        printf("\n");
    }
    */
    /* if ( optind >= argc ) {
        fprintf(stderr, "Fehler: Es wurden Optionen, die Argumente erwarten, ohne Argument angegeben.\n");
        exit(EXIT_FAILURE);
    } */

    /*
    printf("Es wurden %u Argumente angeben.\n", argc );
    printf("Verbose Modus ist");
    if ( opt_verbose == 0 ) {
        printf(" nicht");
    }
    printf(" gesetzt.\n"); */
    /* In main() sind return() und exit() gleichwertig. */
    return(0);
}

void searchFile(const char* fileName, const char* basePath, bool& fileFound)
{
    char path[PATH_MAX]; // 255, since d_name is a char array that holds a maximum of 256 characters
    struct dirent *direntp;
    DIR *dir = opendir(basePath);

    if(!dir)
    {
        return;
    }

    while((direntp = readdir(dir)) != NULL)
    {
        if(strcmp(direntp->d_name, ".") != 0 && strcmp(direntp->d_name, "..") != 0) // strcmp = StringCompare
        {
            if(fileFound == true)
            {
                return;
            }
//            printf("%s\n", direntp->d_name);

            // Construct new Path for our base Path
            strcpy(path, basePath);
            strcat(path, "/");
            strcat(path, direntp->d_name);

            if(strcmp(direntp->d_name, fileName) == 0)
            {
                char* currentWDP = returnCWDP();
                strcat(currentWDP, path);
                printf("File found at: %s\n", currentWDP);
                fileFound = true;
                return;
            }

            searchFile(fileName, path, fileFound);
        }
    }

}

void searchFile_CSInsensitive(const char* fileName, const char* basePath, bool& fileFound)
{
    char path[PATH_MAX]; // 255, since d_name is a char array that holds a maximum of 256 characters
    struct dirent *direntp;
    DIR *dir = opendir(basePath);

    if(!dir)
    {
        return;
    }

    while((direntp = readdir(dir)) != NULL)
    {
        if(strcasecmp(direntp->d_name, ".") != 0 && strcasecmp(direntp->d_name, "..") != 0) // strcmp = StringCompare
        {
            if(fileFound == true)
            {
                return;
            }
//            printf("%s\n", direntp->d_name);

            // Construct new Path for our base Path
            strcpy(path, basePath);
            strcat(path, "/");
            strcat(path, direntp->d_name);

            if(strcasecmp(direntp->d_name, fileName) == 0)
            {
                char* currentWDP = returnCWDP();
                strcat(currentWDP, path);
                printf("File found at: %s\n", currentWDP);
                fileFound = true;
                return;
            }

            searchFile_CSInsensitive(fileName, path, fileFound);
        }
    }

}

char * returnCWDP()
{
    long maxpath;
    char *mycwdp;

    if ((maxpath = pathconf(".", _PC_PATH_MAX)) == -1) {
      perror("Failed to determine the pathname length");
    }
    if ((mycwdp = (char *) malloc(maxpath)) == NULL) {
      perror("Failed to allocate space for pathname");
    }
    if (getcwd(mycwdp, maxpath) == NULL) {
      perror("Failed to get current working directory");
    }
    printf("Current working directory: %s\n", mycwdp);

    return mycwdp;
}

void simpleSearch(const char* fileName, const char* path, bool& fileFound)
{
    struct dirent *direntp;
    DIR *dirp;

    if ((dirp = opendir(path)) == NULL) {
      perror ("Failed to open directory");
      return;
    }

    while ((direntp = readdir(dirp)) != NULL)
    {
        if(fileFound == true)
        {
            return;
        }

        if(strcmp(direntp->d_name, fileName) == 0)
        {
            char* currentWDP = returnCWDP();
            strcat(currentWDP, path);
            printf("File found at: %s\n", currentWDP);
            fileFound = true;
            return;
        }

    }
    while ((closedir(dirp) == -1) && (errno == EINTR)) ;
}

void simpleSearch_CSInsensitive(const char* fileName, const char* path, bool& fileFound)
{
    struct dirent *direntp;
    DIR *dirp;

    if ((dirp = opendir(path)) == NULL) {
      perror ("Failed to open directory");
      return;
    }

    while ((direntp = readdir(dirp)) != NULL)
    {
        if(fileFound == true)
        {
            return;
        }

        if(strcasecmp(direntp->d_name, fileName) == 0)
        {
            char* currentWDP = returnCWDP();
            strcat(currentWDP, path);
            printf("File found at: %s\n", currentWDP);
            fileFound = true;
            return;
        }

    }
    while ((closedir(dirp) == -1) && (errno == EINTR)) ;
}


//char * sanitizePath(char* path)
//{
//    char* newPath[PATH_MAX];
//    int i = 0;
//    if(path[0] == '.')
//        i = 1;
//    else
//        i = 0;
//
//    for(i; i < PATH_MAX;++i)
//    {
//        newPath[i] = path[i];
//    }
//    return newPath;
//}




