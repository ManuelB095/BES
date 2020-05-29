#ifndef GRIDSERVER_H
#define GRIDSERVER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include <vector>  //Für Vectoren
#include <fcntl.h>  //um die Pipe zu öffnen
#include <sys/stat.h> //um die Pipe zu erstellen
#include <sys/types.h> //um die Pipe zu erstellen

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
public:
    char name;
    long pid;
    int x;
    int y;
    Car()
    {
        x=0; y=0; pid = -1; name = '\0';
    }

};

// GLOBALS

extern Car Cars[26];
extern int msgid;

bool in_array(char c, const Car myCars[]);
int charToNumType(const char c); // Casts char to integer; 'A' = 0; 'Z' = 26


//void levelpipe(const char* myfifo, std::vector<std::vector<char> >, int x, int y);
#endif
