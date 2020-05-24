#include "kopf.h"


<<<<<<< HEAD

//---------------------------------------------------------//
int main(int argc, char *argv[])
{

    struct dirent* file; //hier wird das aktuelle File gespeichert (mit namen, filenummer usw.)
    DIR* direction;  //darin wird das file gespeichert, das durchsucht wird.
    int counter = 0;
    string search_file; //das gesuchte File wird in "search_file"
    string weg = argv[1]; //hier wird das Verzeichnis als String gespeichert, in dem gesucht wird
    string filename = argv[0];
    if (argc<2)
    {
        cout<<"Fehler"<<endl;
        return 1;
    }
    if ((direction = opendir(argv[1])) == NULL)
    {
        cout<<"Failed to open directory"<<endl;
        return 1;
    }
    search_file=argv[2];


        weg = pathfinder(weg, filename); //den Pfad nach vollständigkeit prüfen

    while ((file = readdir(direction)) != NULL)
    {
        string filename = file->d_name;
        if (filename == "." || filename == "..")
        {

            continue;
        }
        else
        {
            cout<<file->d_name;
            if(file->d_type == DT_DIR) //Überprüft ob das gelesene File ein eigener Ordner ist.
            {
                cout<<"  d"<<endl;
            }
            else
            {
                cout<<endl;
            }

            counter ++;
        }
        if(filename == search_file)
        {
            cout<<"Filename: "<<search_file<<"  Path: "<<weg+search_file<<endl;

            break;
        }


    }

    closedir(direction);

    return 0;
}
=======
/* Verbose Flag wird global gesetzt, damit der komplette Code es sehen kann. */
unsigned short opt_verbose = 0;

/* Entry Point */
int main( int argc, char*argv[] ) {

    bool fileFound = false;

    int c;
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
//                printf("%s\n", argv);
//                print_usage( programm_name );
//                printf("Parsing Option R with OPTIND = %i, OPTARG = %s\n", optind, optarg);

                break;
            case 'i':
                Counter_Option_i++;
//                printf("Parsing Option i with OPTIND = %i, OPTARG = %s, ARGC = %i\n", optind, optarg, argc );
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

    if(optind+1 >= argc) // If no filename or no path is given, cancel program and show error Message
    {
        fprintf( stderr, "%s Fehler: Zu wenige Optionen angegeben.\n", programm_name );
        exit(2);
    }


    if(Counter_Option_R == 1 && Counter_Option_i != 1)
    {
        for(int i=0; i<(argc - 3);++i) // Accounting for other variables and options in this case
        {
//            printf("Base Path: %s\n", argv[optind]);
//            printf("File Name: %s\n", argv[optind+1+i]);
            char *basePath = argv[optind];
            char *nameOfFile = argv[optind+1+i];

            pid_t myPid = fork();

            if(myPid == 0)
            {
                searchFile(nameOfFile,basePath, fileFound); // search Files recursively; Without fork: set fileFound to false after iteration.
                if(fileFound == false)
                {
                    pid_t childPID = getpid();
                    printf("PID: %ld reports -> File %s not found.. \n", (long)childPID, nameOfFile);printf("PID: %ld reports -> File %s not found.. \n", (long)myPid, nameOfFile);
                }
                exit(0); // Exit only the child process of fork in order to hava one parent alone, creating the forks
            }
            else if(myPid == -1)
            {
                fprintf(stderr, "%s Fehler: Generieren eines Kindprozesses fehlgeschlagen.\n", programm_name );
                exit(1);
            }
        }

        for(int i=0; i < (argc-3);++i)
        {
            wait(NULL);
        }
        printf("\nEnd of Program\n");
    }

    if(Counter_Option_R == 1 && Counter_Option_i == 1)
    {
        for(int i=0; i<(argc - 4);++i) // Accounting for other variables and options in this case
        {
//            printf("Base Path: %s\n", argv[optind]);
//            printf("File Name: %s\n", argv[optind+1+i]);
            char *basePath = argv[optind];
            char *nameOfFile = argv[optind+1+i];

            pid_t myPid = fork();

            if(myPid == 0)
            {
                searchFile_CSInsensitive(nameOfFile,basePath, fileFound); // search Files recursively; Without fork: set fileFound to false after iteration.
                if(fileFound == false)
                {
                    pid_t childPID = getpid();
                    printf("PID: %ld reports -> File %s not found.. \n", (long)childPID, nameOfFile);
                }
                exit(0); // Exit only the child process of fork in order to hava one parent alone, creating the forks
            }
            else if(myPid == -1)
            {
                fprintf(stderr, "%s Fehler: Generieren eines Kindprozesses fehlgeschlagen.\n", programm_name );
                exit(1);
            }
        }

        for(int i=0; i < (argc-4);++i)
        {
            wait(NULL);
        }
        printf("\nEnd of Program\n");
    }

    if(Counter_Option_R != 1 && Counter_Option_i != 1)
    {
         for(int i=0; i<(argc - 2);++i) // Accounting for other variables and options in this case
        {
//            printf("Base Path: %s\n", argv[optind]);
//            printf("File Name: %s\n", argv[optind+1+i]);
            char *basePath = argv[optind];
            char *nameOfFile = argv[optind+1+i];

            pid_t myPid = fork();

            if(myPid == 0)
            {
                simpleSearch(nameOfFile,basePath, fileFound); // search Files recursively; Without fork: set fileFound to false after iteration.
                if(fileFound == false)
                {
                    pid_t childPID = getpid();
                    printf("PID: %ld reports -> File %s not found.. \n", (long)childPID, nameOfFile);
                }
                exit(0); // Exit only the child process of fork in order to hava one parent alone, creating the forks
            }
            else if(myPid == -1)
            {
                fprintf(stderr, "%s Fehler: Generieren eines Kindprozesses fehlgeschlagen.\n", programm_name );
                exit(1);
            }
        }

        for(int i=0; i < (argc-2);++i)
        {
            wait(NULL);
        }
        printf("\nEnd of Program\n");
    }

    if(Counter_Option_R != 1 && Counter_Option_i == 1)
    {
       for(int i=0; i<(argc - 3);++i) // Accounting for other variables and options in this case
        {
//            printf("Base Path: %s\n", argv[optind]);
//            printf("File Name: %s\n", argv[optind+1+i]);
            char *basePath = argv[optind];
            char *nameOfFile = argv[optind+1+i];

            pid_t myPid = fork();

            if(myPid == 0)
            {
                simpleSearch_CSInsensitive(nameOfFile,basePath, fileFound); // search Files recursively; Without fork: set fileFound to false after iteration.
                if(fileFound == false)
                {
                    pid_t childPID = getpid();
                    printf("PID: %ld reports -> File %s not found.. \n", (long)childPID, nameOfFile);
                }
                exit(0); // Exit only the child process of fork in order to hava one parent alone, creating the forks
            }
            else if(myPid == -1)
            {
                fprintf(stderr, "%s Fehler: Generieren eines Kindprozesses fehlgeschlagen.\n", programm_name );
                exit(1);
            }
        }

        for(int i=0; i < (argc-3);++i)
        {
            wait(NULL);
        }
        printf("\nEnd of Program\n");
    }
}







>>>>>>> e006d68cff267b185ab090027fb11ded0ae9716c
