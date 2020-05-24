
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
/* Include Datei für getopt(); getopt.h geht auch, ist aber älter. */
#include <unistd.h>
#include <vector>
#include <fcntl.h>  //um die Pipe zu öffnen
#include <sys/stat.h> //um die Pipe zu erstellen
#include <sys/types.h> //um die Pipe zu erstellen
#include <string.h>
#include <errno.h>


int main( int argc, char*argv[] ) {
    int c;
    int x=0;
    int y=0;
    //unsigned short Counter_Option_f = 0;
    //unsigned short Counter_Option_h = 0;
    //unsigned short Counter_Option_v = 0;

    while ( (c = getopt( argc, argv, "x:y:" )) != EOF )
    {
        switch (c)
        {
            case '?':
                std::cout<<"unbekannte Option"<<std::endl;
                exit(1);
            case 'x':

                x = atoi(optarg);
                std::cout<<"x eingetragen"<<std::endl;
                break;
            case 'y':

                y = atoi(optarg);
                std::cout<<"y eingetragen"<<std::endl;
                break;

            default:
                /* assert() dient nur zu Debugzwecken und sollte nur dort eingesetzt sein,
                   wo etwas sicher niemals passieren darf. 0 ist in C immer gleich "logisch falsch". */
                assert(0);
        }
    }


    std::vector<std::vector<char> > spielfeld(x, std::vector<char>(y) );
    for (int i=0;i<x;i++)
    {
        for (int u =0; u<y; u++)
        {
            spielfeld[i][u]='.';
        }
    }


    const char* myfifo = "../myfifo";  //der Weg, wo die Pipe erstellt wird
    if(mkfifo(myfifo,0666)==-1)  //die Pipe wird erstellt
    {
        if(errno!= EEXIST) //Überprüft ob der Fehler beim erstellen der Pipe ein anderere ist als, dass die Pipe bereits existiert
        {
            std::cout<<"Named Pipe konnte nicht erstellt werden"<<std::endl;
            return 1;
        }

    }
    int fd = open(myfifo, O_WRONLY);
    write (fd,&x, sizeof(int));
    write (fd,&y, sizeof(int));
    //close(fd);
    char change[1];

    while(change[0]!='x')
    {
        //fd = open(myfifo, O_WRONLY);
        for (int i=0;i<x;i++)
        {
            for (int u =0; u<y; u++)
            {
                write (fd,&spielfeld[i][u], sizeof(char));
            }

        }

        std::cin>>change[0];
        if(change[0]=='x')
        {
            write (fd,&change[0], sizeof(char));
        }
        spielfeld[0][0]=change[0];
    }



    close(fd);

    unlink(myfifo);



    return(0);
}
