#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define KEY 420257999		/* eindeutiger Key z.B. Matrikelnummer */
#define PERM 0660 // PERMISSIONS = Berechtigungen der Message Queue. 6 = rw für Owner, 6 = rw für User in der File Group, 0 = nichts für User außerhalb

#define MAX_DATA 255

typedef struct
{
	long mType;
	char mText[MAX_DATA];
} message_t;

struct Car
{
    char name;
    long pid;
    int x;
    int y;
    Car()
    {
        x=0; y=0; pid = -1; name = '\0';
    }

};
