#include "kopf.h"



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
