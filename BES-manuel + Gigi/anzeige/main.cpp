#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <vector>

//#define MAX_BUF 2046


int main()
{

    int x=0;
    int y=0;

    int fd;
    const char * myfifo = "../myfifo";

    int ok=0;
    while (ok ==0 )
    {
        fd = open (myfifo, O_RDONLY);
        if(fd>0)
        {
            ok =1;
        }

    }
    ok = 0;
    read(fd, &x, sizeof(int));
    read(fd, &y, sizeof(int));
    std::cout<<"x Wert: "<<x<<" y Wert: "<<y<<std::endl;
    std::vector<std::vector<char> > spielfeld(x, std::vector<char>(y) );
    close(fd);
    fd = 0;

    int weiterlaufen= 0;
    while (weiterlaufen == 0)
    {
        while (ok == 0 )
        {
            fd = open (myfifo, O_RDONLY);
            if(fd>0)
            {
                ok =1;
            }

        }
        ok = 0;
        for (int i=0;i<x;i++)
        {
            for (int u =0; u<y; u++)
            {
                if (read (fd,&spielfeld[i][u], sizeof(char))==-1)
                {
                    std::cout<<"Fehler ist aufgetreten"<<std::endl;
                    break;

                }
                if (spielfeld[i][u]=='x')
                {
                    std::cout<<"Programm ist beendet"<<std::endl;
                    weiterlaufen=1;


                }


            }

        }
        close(fd);
        fd=0;
        if(weiterlaufen==0)
        {
           for (int i=0;i<y+2;i++)
            {
                std::cout<<"#";
            }
            std::cout<<std::endl;
            for (int i=0;i<x;i++)
            {
                std::cout<<"#";
                for (int u =0; u<y; u++)
                {
                    std::cout<<spielfeld[i][u];
                }
                std::cout<<"#";
                std::cout<<std::endl;
            }
            for (int i=0;i<y+2;i++)
            {
                std::cout<<"#";
            }
            std::cout<<std::endl;
            std::cout<<std::endl;
            std::cout<<std::endl;

        }
    }



    close(fd);


    return 0;
}
