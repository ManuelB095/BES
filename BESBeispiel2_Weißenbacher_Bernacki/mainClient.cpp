#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <errno.h>

#include <sstream>
#include <string>

#include "clientLogic.h"

#define KEY 420257999		/* eindeutiger Key z.B. Matrikelnummer */
#define PERM 0660

#define MAX_DATA 255

char sig_carName = '\0';

using namespace std;

typedef struct
{
	long mType;
	char mText[MAX_DATA];
} message_t;

void sig_handler (int sig)
{
	int msgid = -1;
	message_t msg;
	std::string t = "t";
	strcpy(msg.mText, t.c_str()); // This seems weird, but using char * t throws a warning

	if(sig_carName != '\0') // Gets updated to car name`s char in main function
	{
        msg.mType = sig_carName;
	}
	else
	{
        fprintf(stderr, "ERROR: sig_carName not set to send Message. Program will be terminated.\n");
        exit(EXIT_FAILURE);
	}

	if( (msgid = msgget(KEY,PERM))==-1 )
	{
      		/* error handling */
      		fprintf(stderr,"Can't access message queue\n");
      		//return EXIT_FAILURE;
   	}

	if (msgsnd(msgid,&msg,sizeof(msg)-sizeof(long), 0) == -1)
	{
	      	/* error handling */
	      	fprintf(stderr,"Can't send message\n");
	      	//return EXIT_FAILURE;
	}
    fprintf(stdout,"\nVehicle has been eliminated\n");
    exit(EXIT_SUCCESS); // Or return EXIT_SUCCESS ?
}


/**
    This VehicleClient accesses the Message Queue and Sends its name ( via msg.mType ), as well as it`s Pid ( via msg.mText );
    After that, it listens to a Message Queue answer.
    TO DO: If it got the OK, it prompts the User for movement
**/


int main(int argc, char* argv[])
{
   signal(SIGTERM, sig_handler);
   signal(SIGINT, sig_handler);
   signal(SIGQUIT, sig_handler);
   signal(SIGHUP, sig_handler);

   message_t msg;	/* Buffer fuer Message */
   int msgid = -1;	/* Message Queue ID */
   char carName = argv[1][0]; // The char that specifies the vehicleName

   if(carName < 'A' || carName > 'Z')
   {
        fprintf(stderr,"%c is not a valid Car name !\n",carName);
        return EXIT_FAILURE;
   }

   sig_carName = carName;
   //char recvType = msg.mType * 10; // Lazy way to see, if this message is for me

   /* Argument Handling */
   if (argc!=2)
   {
      fprintf(stderr,"Usage: %s <Car Name>\n",argv[0]);
      return EXIT_FAILURE;
   }

   /** FIRST, TRY TO REGISTRATE VEHICLE **/

   // Set msg.mType to Client Name
   msg.mType=(long)(carName - 'A' + 1); // mType = Name of the Vehicle ( A to Z ) normalized to 1-27

   // Set msg.mText to Process ID
   pid_t thisPid;
   thisPid = getpid();
   char pidChars [MAX_DATA];
   printf("%s\n", msg.mText);
   for(unsigned int i=0; i < MAX_DATA;i++)
   {
        msg.mText[i] = '\0';
   }
   msg.mText[0] = 'R'; // R for Registration
   printf("%s\n", msg.mText);

   sprintf(pidChars, "%d", thisPid);
   strncat(msg.mText,pidChars,MAX_DATA);
   printf("%c\n",msg.mText[2]);
   printf("Car %c with pid %ul is created and trying to registrate.\n", carName, thisPid);
   // Send the message
   /* Message Queue oeffnen */
   if( (msgid = msgget(KEY,PERM))==-1 )
   {
      /* error handling */
      fprintf(stderr,"%s: Can't access message queue\n",argv[0]);
      return EXIT_FAILURE;
   }

   if (msgsnd(msgid,&msg,sizeof(msg)-sizeof(long), 0) == -1)
   {
         /* error handling */
         perror("msgsnd");
         fprintf(stderr,"%s: Can't send message\n",argv[0]);
         return EXIT_FAILURE;
   }
   printf("Message sent: %s\n",msg.mText);

    /** LISTEN FOR CONSOLE INPUT AND SEND THAT TO MQ **/

    while (42) {
        // Get Console Input
        char consoleInput[MAX_DATA];
        fprintf(stdout, "Enter Move: ");
        fgets(consoleInput, MAX_DATA, stdin);

        // If Vehicle wants to move
        if (consoleInput[0] == 'W' || consoleInput[0] == 'E' || consoleInput[0] == 'S' || consoleInput[0] == 'N')
        {
            msg.mType  = (long)(carName - 'A' + 1);
            strcpy(msg.mText, "M");
            strncat(msg.mText, &consoleInput[0], 1);

            if (msgsnd(msgid, &msg, sizeof(msg) - sizeof(long), 0) == -1)
            {
                /* error handling */
                fprintf(stderr, "%s: Can't send message\n", argv[0]);
                return EXIT_FAILURE;
            }
        }
        else {
            continue;
        }

            // If Vehicle wants to terminate
            if (consoleInput[0] == 't')
            {
                msg.mType = (long)(carName - 'A' + 1);
                strncpy(msg.mText, "t", MAX_DATA);

                if (msgsnd(msgid, &msg, sizeof(msg) - sizeof(long), 0) == -1) {
                    /* error handling */
                    fprintf(stderr, "%s: Can't send message\n", argv[0]);
                }
                return EXIT_FAILURE;
            }
    } // END OF WHILE LOOP
   return EXIT_SUCCESS;
}

