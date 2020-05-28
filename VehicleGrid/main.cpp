#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string>
#include <string.h>
#include <iostream>

#include "gridserver.h"

/** GLOBALS **/

Car Cars[26];
int msgid = -1;
int fd = -1;
const char* myfifo = "../myfifo";  //der Weg, wo die Pipe erstellt wird


/** ATTENTION ! This WORKS, just exluded for now, since I want to test the Message Queue System
Uncomment, if you want to use my getopt Input. If not, just delete It. **/

//char * programm_name = "GridServer";
//
//void print_usage( char * programm_name )
//{
//    printf("gridserver -x dimension -y dimension\n"); // TO DO: Better version of this maybe
//}
//
using namespace std;
//

/** SIGNAL HANDLER **/
/** This creates some very weird ( and possibly dangerous ) behavior. Be cautious with the kill commands on your Laptop !! **/
void sig_handler (int sig)
{
    for(unsigned int i=0; i < 26; i++) // Kill them all
    {
        if(Cars[i].pid != -1)
            {kill(Cars[i].pid, SIGKILL);}
    }

	if (msgctl(msgid, IPC_RMID, NULL) == -1)
	{
		fprintf(stderr,"Queue not deleted\n");
	}

    if (remove(myfifo) != 0) {
        printf("FIFO not removed\n");
    }

    char terminate = 'x';
    if(fd != -1){
       write(fd, &terminate, sizeof(char));
    }

	//unlink(myfifo);

	exit(EXIT_SUCCESS);

	/** TO DO: Delete/Close Named Pipe and Field **/
}

int main(int argc, char * argv[])
{

/** Input Read In Area ( Works too ! ) Uncomment, if you need/ want to use it **/
    signal(SIGTERM, sig_handler);
    signal(SIGINT, sig_handler);
	signal(SIGQUIT, sig_handler);
	signal(SIGHUP, sig_handler);

    int c = 0;
    int Counter_Option_x = 0;
    int Counter_Option_y = 0;

    //char * x_val;
    //char * y_val;
    int x;   //Sice of the field in x
    int y;   //Sice of the field in y
    while ( (c = getopt( argc, argv, "x:y:" )) != EOF ) {
        switch (c) {
            case '?':
                printf("error: Unknown option.\n");
                //print_usage( programm_name );
                exit(1);
                /* Das break ist nach exit() eigentlich nicht mehr notwendig. */
                break;
            case 'x':
                Counter_Option_x++;
                x = atoi(optarg);
               break;
            case 'y':
                Counter_Option_y++;
                y = atoi(optarg);
                break;
            default:
                /* assert() dient nur zu Debugzwecken und sollte nur dort eingesetzt sein,
                   wo etwas sicher niemals passieren darf. 0 ist in C immer gleich "logisch falsch". */
                assert(0);
        }
    }

    //char * pointer;

    //int x_num = strtol(x_val, &pointer, 10); // Variable, Some pointer for idk what reason, base ( 10 for decimals ) = Most Convoluted function ever
    //int y_num = strtol(y_val, &pointer, 10);

   //printf("x:%d y:%d", x, y);
    std::cout<<"x Wert: "<<x<<" y Wert: "<<y<<std::endl;
    if ( ( Counter_Option_x =! 1 ) || ( Counter_Option_y != 1 ) ) {
        printf("Fehler: Wrong number of options.\n");
       exit(1);
    }
/**---------------------------Gigis Code/ Erstelltung des Spielfeldes--------------------------------**/
    std::vector<std::vector<char> > spielfeld(x, std::vector<char>(y) );
    for (int i=0;i<x;i++)
    {
        for (int u =0; u<y; u++)
        {
            spielfeld[i][u]='.';
        }
    }
//-----------------------------ertellen der named Pipe-------------------------------------------------
    if(mkfifo(myfifo,0666)==-1)  //die Pipe wird erstellt
    {
        if(errno!= EEXIST) //Überprüft ob der Fehler beim erstellen der Pipe ein anderere ist als, dass die Pipe bereits existiert
        {
            std::cout<<"Named Pipe could not be created\n"<<std::endl;
            return 1;
        }

    }
    fd = open(myfifo, O_WRONLY);
    write (fd,&x, sizeof(int));
    write (fd,&y, sizeof(int));
    //close(fd);
/**---------------------------------------------------------------------------------------------------**/




    /** MESSAGE QUEUE TESTING AREA **/

    /*
        Now this is the meat of the (Message Queue Part) of the Server.
        msg is a Struct that has a long msg.mType, that the Message Queue uses to differentiate between vehicles.
        Since it is a long type, it has to be converted to char before/after sending.
        I do this with msgType and typeChar.

        The actual transmitted text is saved in msg.mText. It can be one of two things:
        (1) R12345 -> R-egistrate, followed by the Vehicle Clients Process ID ( which gets saved into my struct Car via Cars[index].pid = ... )
        (2) MN ME MS MW -> M-ove, followed by a char ( N, E, S, W ) determining the direction in which the car wants to move. I save this in char whereTo

        The Car struct simply stores the Vehicle Clients name ( A to Z ), it`s PID, and it`s coordinates.
        The Cars Array is used to Access all stored Cars along with their Information.

        msgget -> Tries to access Message Queue. IPC_CREAT and IPC_EXCL trie to make a new one.*/
        /**This FAILS if the Message Queue already exists !!**/
        /* For this reason, please use ipcs -q in a terminal of your choice to view the Message Queues.
           And ipcrm -q <Process ID here> to Delete a Message Queue.

        msgrcv -> Tries to read ANY Messages in the Queue and stores the data in the msg struct. ( msg.mType and msg.mText ).
           I use this data to error check and register cars.
    */

    message_t msg;	/* Buffer fuer Message */
    msgid = -1;	/* Message Queue ID */


    /* Message Queue neu anlegen */
    if( (msgid = msgget(KEY,PERM | IPC_CREAT | IPC_EXCL ))==-1 ) // Key = ID to identify Queue; Hardcoded in the Header; IPC-CREAT = Create MsgQueue if none exists, IPC_EXCL = Function fails if Msg Queue already exists
    {
//        if (msgctl(msgid, IPC_RMID, NULL) == -1)
//        {
//            fprintf(stderr,"Queue not deleted\n");
//        }
      /* error handling */
      fprintf(stderr,"%s: Error creating message queue\n",argv[0]);
      //return EXIT_FAILURE;
    }

    /* In einer Endlosschleife Nachrichten empfangen */
    while (1)
    {
      if (msgrcv(msgid,&msg,sizeof(msg)-sizeof(long), 0 , 0) == -1) // Fourth argument is msgType ( mtype in struct )
      {
         /* error handling */
         fprintf(stderr,"%s: Can't receive from message queue\n",argv[0]);
         return EXIT_FAILURE;
      }
      printf("Message received: From Car: %ld with Text: %s\n",msg.mType, msg.mText);

      if(msg.mType < 1 || msg.mType > 26) // Cars go from 1-26 ( A to Z )
      {
        fprintf(stderr, "You betrayed me! You are not a car at all ! You are our Doom !\n");
        for(unsigned int i=0; i < 26; i++) // Kill them all
        {
            if(Cars[i].pid != -1)
            {kill(Cars[i].pid, SIGKILL);}
        }
      }


      /** MSG-Q: Process Received Data **/
      int msgType = msg.mType;
      std::cout<<msgType<<std::endl;   //Wichtig------------------------------------------------------------------------------------------------
      char typeChar = msgType + 'A' - 1;
      std::string msgText = msg.mText;

      /** MSG-Q: 'R for Registration '**/

      if( msgText[0] == 'R')
      {
        string sub = msgText.substr(1,8); // First Position was 'R'; Then comes the pid
        long newPid;
        newPid = std::stoul(sub, nullptr, 10); // This is honestly such a strange way of casting

        if(in_array(typeChar, Cars)) /** Car is already registered ! Such insolence deserves sudden and complete Termination !**/
        {
                kill(newPid, SIGKILL); // Kill new Car
        }
        else /** Oh my, a new Car ! Let`s Register and give it Coordinates right away ! **/
        {
             if ( Cars[msgType-1].name == '\0') /** Great, we have a free spot for you ! **/
              {


        /**-------------------------------Gigis Code//searche for a place on the Field --------------------------------------------**/
                int posx = (typeChar + rand())%x;
                int posy = (typeChar + rand())%y;

                for (int i=1; i<11; i++)
                {
                    if (spielfeld[posx][posy]=='.')
                    {
                        spielfeld[posx][posy] = typeChar;
                        std::cout<<"Position of "<<typeChar<<" : pos x "<<posx<<" pos y "<<posy<<std::endl;
                        i=20;


                        //levelpipe(fd, spielfeld, x, y);
                        for (int i=0;i<x;i++)
                        {
                            for (int u =0; u<y; u++)
                            {
                                write (fd,&spielfeld[i][u], sizeof(char));
                            }

                        }
                    }



                    else if(spielfeld[posx][posy]!='.')
                    {
                        posx = (posx + i +rand())%x;
                        posy = (posy + i + rand())%y;
                    }

                    else if(i==10)
                    {
                        std::cout<<"No Postion on the board was found\n"<<std::endl;
                        string sub = msgText.substr(1,8); // First Position was 'R'; Then comes the pid
                        newPid = std::stoul(sub, nullptr, 10); // This is honestly such a strange way of casting
                        kill(newPid, SIGKILL);
                    }


                }
                //close(fd);
                Cars[msgType-1].x = posx;   //Pos x wird im Car Struct gespeichert
                Cars[msgType-1].y = posy;   //Pos y wird im Car Struct gespeichert
    /**---------------------------------------------------------------------------------------------------------------------------**/
                Cars[msgType-1].name = typeChar;
                Cars[msgType-1].pid = newPid; // Thankfully long can be converted to pid_t effortlessly..
                printf("Car '%c' on position x: %d y: %d OK\n", typeChar, Cars[msgType-1].x,Cars[msgType-1].y);
                printf("New Car PID: %ld", Cars[msgType-1].pid );
              }
              else
              {
                fprintf(stderr, "But.. this should not happen. No It can`t be. Arggh...\n");
                for(unsigned int i=0; i < 26; i++) // Kill them all
                {
                    if(Cars[i].pid != -1)
                        {kill(Cars[i].pid, SIGKILL);}
                }
                break;
              }
        }
      }

    /** MSG-Q: 'M for Movement '**/

      else if(msgText[0] == 'M')
      {
        if(msgText[1] != 'N' && msgText[1] != 'E' && msgText[1] != 'S' && msgText[1] != 'W')
        {
            kill(Cars[msgType-1].pid, SIGKILL); // Vehicle sent weird data. Better terminate it.
        }
        else
        {
            char whereTo = msgText[1]; // Where to move
            printf("Car %c wants to move %c\n", typeChar, whereTo);

    /**--------------------------------------Gigis Code / Movement of the Car ------------------------------------------------------------**/
                if(whereTo == 'N')  //Bewegung nach Norden
                {

                    if(Cars[typeChar - 'A'].x==0)  //Wenn das Auto bereits am nördlichen Rand steht
                    {
                        std::cout<<"Car "<<typeChar<<" fell of the World"<<std::endl;
                        spielfeld[Cars[typeChar - 'A'].x][Cars[typeChar - 'A'].y] = '.';
                        kill(Cars[typeChar - 'A'].pid, SIGKILL);
                        Cars[typeChar - 'A'].name = '\0'; //Der Platz im Struct wird wieder frei gemacht

                        for (int i=0;i<x;i++)
                        {
                            for (int u =0; u<y; u++)
                            {
                                write (fd,&spielfeld[i][u], sizeof(char));
                            }

                        }
                    }
                    else if(spielfeld[Cars[typeChar - 'A'].x - 1][Cars[typeChar - 'A'].y]!='.')  //Wenn nördlich vom Auto schon a+ein anderes Auto steht
                    {
                        char otherCar = spielfeld[Cars[typeChar - 'A'].x - 1][Cars[typeChar - 'A'].y];
                        std::cout<<"Car "<<typeChar<< "crashed in "<<otherCar<<std::endl;
                        spielfeld[Cars[typeChar - 'A'].x][Cars[typeChar - 'A'].y] = '.';
                        spielfeld[Cars[typeChar - 'A'].x - 1][Cars[typeChar - 'A'].y] = '.';

                        kill(Cars[typeChar - 'A'].pid, SIGKILL);
                        kill(Cars[otherCar - 'A'].pid, SIGKILL);

                        Cars[typeChar - 'A'].name = '\0';   //Der Platz im Struct wird wieder frei gemacht
                        Cars[otherCar - 'A'].name = '\0';

                        for (int i=0;i<x;i++)
                        {
                            for (int u =0; u<y; u++)
                            {
                                write (fd,&spielfeld[i][u], sizeof(char));
                            }

                        }
                    }

                    else
                    {
                        spielfeld[Cars[typeChar - 'A'].x][Cars[typeChar - 'A'].y] = '.';
                        spielfeld[Cars[typeChar - 'A'].x - 1][Cars[typeChar - 'A'].y] = typeChar;
                        Cars[typeChar - 'A'].x -= 1;

                        for (int i=0;i<x;i++)
                        {
                            for (int u =0; u<y; u++)
                            {
                                write (fd,&spielfeld[i][u], sizeof(char));
                            }

                        }
                    }

                }

                else if(whereTo == 'E')  //Bewegung nach Osten
                {

                    if(Cars[typeChar - 'A'].y== y-1)  //Wenn das Atuo bereits am östlichen Rand steht
                    {
                        std::cout<<"Car "<<typeChar<<" fell of the World"<<std::endl;
                        /**Pseudocode  wo das gefallene Autos gelöscht wird  **/
                        spielfeld[Cars[typeChar - 'A'].x][Cars[typeChar - 'A'].y] = '.';
                        kill(Cars[typeChar - 'A'].pid, SIGKILL);

                        Cars[typeChar - 'A'].name = '\0'; //Der Platz im Struct wird wieder frei gemacht

                        for (int i=0;i<x;i++)
                        {
                            for (int u =0; u<y; u++)
                            {
                                write (fd,&spielfeld[i][u], sizeof(char));
                            }

                        }
                    }
                    else if (spielfeld[Cars[typeChar - 'A'].x][Cars[typeChar - 'A'].y+1]!='.') //Wenn östlich vom Auto bereits ein anderes Auto steht
                    {
                        char otherCar = spielfeld[Cars[typeChar - 'A'].x][Cars[typeChar - 'A'].y+1];
                        std::cout<<"Car "<<typeChar<< "crashed in "<<otherCar<<std::endl;
                        /**Pseudocode  wo beide Autos gelöscht werden**/
                        spielfeld[Cars[typeChar - 'A'].x][Cars[typeChar - 'A'].y] = '.';
                        spielfeld[Cars[typeChar - 'A'].x][Cars[typeChar - 'A'].y+1] = '.';
                        kill(Cars[typeChar - 'A'].pid, SIGKILL);
                        kill(Cars[otherCar - 'A'].pid, SIGKILL);

                        Cars[typeChar - 'A'].name = '\0'; //Der Platz im Struct wird wieder frei gemacht
                        Cars[otherCar - 'A'].name = '\0';

                        for (int i=0;i<x;i++)
                        {
                            for (int u =0; u<y; u++)
                            {
                                write (fd,&spielfeld[i][u], sizeof(char));
                            }

                        }
                    }
                    else
                    {
                        spielfeld[Cars[typeChar - 'A'].x][Cars[typeChar - 'A'].y] = '.';
                        spielfeld[Cars[typeChar - 'A'].x][Cars[typeChar - 'A'].y+1] = typeChar;
                        Cars[typeChar - 'A'].y += 1;

                        for (int i=0;i<x;i++)
                        {
                            for (int u =0; u<y; u++)
                            {
                                write (fd,&spielfeld[i][u], sizeof(char));
                            }

                        }
                    }
                }


                else if(whereTo == 'S') //Bewegung nach Süden
                {

                    if(Cars[typeChar - 'A'].x== x-1) //Wenn das Auto sich am südlichen Rand befindet
                    {
                        std::cout<<"Car "<<typeChar<<" fell of the World"<<std::endl;
                        /**Pseudocode  wo das gefallene Auto gelöscht wird  **/
                        spielfeld[Cars[typeChar - 'A'].x][Cars[typeChar - 'A'].y] = '.';
                        printf("PID to kill: %ld\n", Cars[typeChar - 'A'].pid);
                        kill(Cars[typeChar - 'A'].pid, SIGKILL);

                        Cars[typeChar - 'A'].name = '\0'; //Der Platz im Struct wird wieder frei gemacht

                        for (int i=0;i<x;i++)
                        {
                            for (int u =0; u<y; u++)
                            {
                                write (fd,&spielfeld[i][u], sizeof(char));
                            }

                        }
                    }
                    else if (spielfeld[Cars[typeChar - 'A'].x+1][Cars[typeChar - 'A'].y]!='.') //Wenn südlich vom Auto bereits ein anderes Auto steht
                    {
                        char otherCar = spielfeld[Cars[typeChar - 'A'].x+1][Cars[typeChar - 'A'].y];
                        std::cout<<"Car "<<typeChar<< "crashed in "<<otherCar<<std::endl;
                        /**Pseudocode  wo beide Autos gelöscht werden**/
                        spielfeld[Cars[typeChar - 'A'].x][Cars[typeChar - 'A'].y] = '.';
                        spielfeld[Cars[typeChar - 'A'].x+1][Cars[typeChar - 'A'].y] = '.';

                        kill(Cars[typeChar - 'A'].pid, SIGKILL);
                        kill(Cars[otherCar - 'A'].pid, SIGKILL);

                        Cars[typeChar - 'A'].name = '\0'; //Der Platz im Struct wird wieder frei gemacht
                        Cars[otherCar - 'A'].name = '\0';

                        for (int i=0;i<x;i++)
                        {
                            for (int u =0; u<y; u++)
                            {
                                write (fd,&spielfeld[i][u], sizeof(char));
                            }

                        }
                    }
                    else
                    {
                        spielfeld[Cars[typeChar - 'A'].x][Cars[typeChar - 'A'].y] = '.';
                        spielfeld[Cars[typeChar - 'A'].x+1][Cars[typeChar - 'A'].y] = typeChar;
                        Cars[typeChar - 'A'].x += 1;

                        for (int i=0;i<x;i++)
                        {
                            for (int u =0; u<y; u++)
                            {
                                write (fd,&spielfeld[i][u], sizeof(char));
                            }

                        }
                    }
                }


                else if(whereTo== 'W') //Bewegung nach Westen
                {


                    if(Cars[typeChar - 'A'].y == 0) //wenn das Auto bereits am westlichen Rand steht
                    {
                        std::cout<<"Car "<<typeChar<<" fell of the World"<<std::endl;
                        /**Pseudocode  wo das gefallene Auto gelöscht wird  **/
                        spielfeld[Cars[typeChar - 'A'].x][Cars[typeChar - 'A'].y] = '.';
                        kill(Cars[typeChar - 'A'].pid, SIGKILL);

                        Cars[typeChar - 'A'].name = '\0'; //Der Platz im Struct wird wieder frei gemacht

                        for (int i=0;i<x;i++)
                        {
                            for (int u =0; u<y; u++)
                            {
                                write (fd,&spielfeld[i][u], sizeof(char));
                            }

                        }
                    }
                    else if (spielfeld[Cars[typeChar - 'A'].x][Cars[typeChar - 'A'].y-1]!='.') //wenn sich westlich vom Auto bereits ein anderes Auto befindet
                    {
                        char otherCar = spielfeld[Cars[typeChar - 'A'].x][Cars[typeChar - 'A'].y-1];
                        std::cout<<"Car "<<typeChar<< "crashed in "<<otherCar<<std::endl;
                        /**Pseudocode  wo beide Autos gelöscht werden**/
                        spielfeld[Cars[typeChar - 'A'].x][Cars[typeChar - 'A'].y] = '.';
                        spielfeld[Cars[typeChar - 'A'].x][Cars[typeChar - 'A'].y-1] = '.';

                        kill(Cars[typeChar - 'A'].pid, SIGKILL);
                        kill(Cars[otherCar - 'A'].pid, SIGKILL);

                        Cars[typeChar - 'A'].name = '\0'; //Der Platz im Struct wird wieder frei gemacht
                        Cars[otherCar - 'A'].name = '\0';

                        for (int i=0;i<x;i++)
                        {
                            for (int u =0; u<y; u++)
                            {
                                write (fd,&spielfeld[i][u], sizeof(char));
                            }

                        }
                    }
                    else
                    {
                        spielfeld[Cars[typeChar - 'A'].x][Cars[typeChar - 'A'].y] = '.';
                        spielfeld[Cars[typeChar - 'A'].x][Cars[typeChar - 'A'].y-1] = typeChar;
                        Cars[typeChar - 'A'].y -= 1;

                        for (int i=0;i<x;i++)
                        {
                            for (int u =0; u<y; u++)
                            {
                                write (fd,&spielfeld[i][u], sizeof(char));
                            }

                        }
                    }
                }






    /**----------------------------------------------------------------------------------------------------------------------------------**/
            /** TO DO: MOVEMENT LOGIC GOES HERE
            Pseudo-Code:

            if(movement != successfull)
            {
                // There was a crash. Kill both cars
                field[x][y] = some char, lets say 'A';
                'B' wanted to move to 'A'`s field, so do the sensible thing and kill both.

            }



            **/



        }


      }
      else if(msgText[0] == 't') // Has to be small t, since valid Car can also be called T
      {
        printf("Car %c with PID: %ld has terminated on it`s own and will be taken off grid.\n", typeChar, Cars[typeChar - 'A'].pid);
        spielfeld[Cars[typeChar - 'A'].x][Cars[typeChar - 'A'].y] = '.';

        Cars[typeChar - 'A'].name = '\0'; //Der Platz im Struct wird wieder frei gemacht

        for (int i=0;i<x;i++)
        {
            for (int u =0; u<y; u++)
            {
                write (fd,&spielfeld[i][u], sizeof(char));
            }

        }
      }


}
    // Code should actually never reach this though
    return EXIT_SUCCESS;
}
