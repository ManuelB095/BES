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
//void sig_handler (int sig)
//{
//    for(unsigned int i=0; i < 26; i++) // Kill them all
//    {
//        if(Cars[i].pid != -1)
//        {kill(Cars[i].pid, SIGKILL)};
//    }
//	exit(0);
//
//	if (msgctl(msgid, IPC_RMID, NULL) == -1)
//	{
//		fprintf(stderr,"Queue not deleted\n");
//	}
//
//	/** TO DO: Delete/Close Named Pipe and Field **/
//}

int main(int argc, char * argv[])
{

/** Input Read In Area ( Works too ! ) Uncomment, if you need/ want to use it **/

//    signal(SIGINT, sig_handler);
//	signal(SIGQUIT, sig_handler);
//	signal(SIGHUP, sig_handler);

//    int c = 0;
//    int Counter_Option_x = 0;
//    int Counter_Option_y = 0;
//
//    char * x_val;
//    char * y_val;
//
//    while ( (c = getopt( argc, argv, "x:y:" )) != EOF ) {
//        switch (c) {
//            case '?':
//                fprintf( stderr, "%s error: Unknown option.\n", programm_name );
//                print_usage( programm_name );
//                exit(1);
//                /* Das break ist nach exit() eigentlich nicht mehr notwendig. */
//                break;
//            case 'x':
//                Counter_Option_x++;
//                x_val = optarg;
//                break;
//            case 'y':
//                Counter_Option_y++;
//                y_val = optarg;
//                break;
//            default:
//                /* assert() dient nur zu Debugzwecken und sollte nur dort eingesetzt sein,
//                   wo etwas sicher niemals passieren darf. 0 ist in C immer gleich "logisch falsch". */
//                assert(0);
//        }
//    }
//
//    char * pointer;
//
//    int x_num = strtol(x_val, &pointer, 10); // Variable, Some pointer for idk what reason, base ( 10 for decimals ) = Most Convoluted function ever
//    int y_num = strtol(y_val, &pointer, 10);
//
//    printf("x:%d y:%d", x_num, y_num);
//
//    if ( ( Counter_Option_x =! 1 ) || ( Counter_Option_y != 1 ) ) {
//        fprintf( stderr, "%s Fehler: Ungültige Anzahl an Optionen.\n", programm_name );
//        exit(1);
//    }





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
         if (msgctl(msgid, IPC_RMID, NULL) == -1) // Delete Message Queue if necessary
          {
             fprintf(stderr,"Queue not deleted\n");
          }
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
        fprintf(stderr, "You betrayed me! You are not a car at all ! You are our Doom !");
        for(unsigned int i=0; i < 26; i++) // Kill them all
        {
            kill(Cars[i].pid, SIGKILL);
        }
      }


      /** MSG-Q: Process Received Data **/
      int msgType = msg.mType;
      char typeChar = msgType + 'A' - 1;
      std::string msgText = msg.mText;
      long newPid;

      /** MSG-Q: 'R for Registration '**/

      if( msgText[0] == 'R')
      {
        if(in_array(typeChar, Cars)) /** Car is already registered ! Such insolence deserves sudden and complete Termination !**/
        {
            kill(Cars[msgType-1].pid, SIGKILL);
        }
        else /** Oh my, a new Car ! Let`s Register and give it Coordinates right away ! **/
        {
             if ( Cars[msgType-1].name == '\0') /** Great, we have a free spot for you ! **/
              {
                    Cars[msgType-1].name = typeChar;
                    string sub = msgText.substr(1,8); // First Position was 'R'; Then comes the pid
                    newPid = std::stoul(sub, nullptr, 10); // This is honestly such a strange way of casting
                    Cars[msgType-1].pid = newPid; // Thankfully long can be converted to pid_t effortlessly..
                    printf("Car '%c' on position(in array) %d OK\n", typeChar, msgType-1);

                    /** TO DO: Check for free space to occupy for given Car. Do with: Cars[msgType-1].x / .y **/





              }
              else
              {
                    fprintf(stderr, "But.. this should not happen. No It can`t be. Arggh...\n");
                    for(unsigned int i=0; i < 26; i++) // Kill them all
                    {
                        //kill(Cars[i].pid, SIGKILL);
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
            //kill(Cars[msgType-1].pid, SIGKILL); // Vehicle sent weird data. Better terminate it.
        }
        else
        {
            char whereTo = msgText[1]; // Where to move
            printf("Car %c wants to move %c\n", typeChar, whereTo);
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


}
    // Code should actually never reach this though
    return EXIT_SUCCESS;
}
