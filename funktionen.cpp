#include "kopf.h"


// überprüft ob der angegebene Pfad ein relativer Pfad ist oder ein absoluter Pfad und gibt den absoluten Pfad aus
string pathfinder (string weg, string filename)
{
    int lang = weg.length(); //die Länge des Pfads wird überprüft (
    string anfang = weg.substr (0,2); //die Ersten zwei Zeichen werden in einen eigenen String gespeichert
    string rest;

    bool check=false;



    if(string(anfang)=="./") //hier wird gecheckt ob der Pfad ein relativer Pfad ist, wenn ja wird der absolute Pfad ermittelt
    {
        if(lang>2) // falls nach dem "./ der Pfad tiefer geht wird dieser Rest extra in "rest" gespeichert"
        {
            rest=weg.substr (2);
            check=true;
        }
        char *mycwdp;
        long maxpath;
        if ((maxpath = pathconf("/home/gigi/", _PC_PATH_MAX)) == -1)
        {
          perror("Failed to determine the pathname length");

        }
        if ((mycwdp = (char *) malloc(maxpath)) == NULL)
        {
          perror("Failed to allocate space for pathname");

        }
        if (getcwd(mycwdp, maxpath) == NULL)
        {
          perror("Failed to get current working directory");

        }
        weg = mycwdp; //hier wird der absolute Pfad zum laufenden File gespeichert
        filename = filename.substr (2);
        size_t found = weg.find(filename);
        if (found!=std::string::npos)  //hier wird der filename vom pfad weggeschnitten
        {
            weg = weg.substr(0,found);
        }

        }
    if(check) //wenn es nach dem "./" einen tieferen Pfad gegeben hat, wird di
    {
        weg+=rest;
    }
    lang = weg.length(); //am Ende wird noch geprüft ob das letzte Zeichen ein "/" ist. Wenn nicht, wird es noch an den "weg" angehängt.
    string ende = weg.substr (lang-1);
    if(string(ende)!="/")
    {
        weg+="/";
    }

    return weg;
}
