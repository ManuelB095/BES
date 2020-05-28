<<<<<<< HEAD
<<<<<<< HEAD
#include "kopf.h"


// überprüft ob der angegebene Pfad ein relativer Pfad ist oder ein absoluter Pfad und gibt den absoluten Pfad aus
=======
=======
>>>>>>> 912251e9fffe17bd68a18abcf7e2a7175c5dc6cc

#include "kopf.h"


// ---------------------------------------------------Pfad chek--------------------------------------------------------------
<<<<<<< HEAD
>>>>>>> e006d68cff267b185ab090027fb11ded0ae9716c
=======
>>>>>>> 912251e9fffe17bd68a18abcf7e2a7175c5dc6cc
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
<<<<<<< HEAD
<<<<<<< HEAD
    if(check) //wenn es nach dem "./" einen tieferen Pfad gegeben hat, wird di
=======
    if(check) //wenn es nach dem "./" einen tieferen Pfad gegeben hat, wird dieser hier angehängt
>>>>>>> e006d68cff267b185ab090027fb11ded0ae9716c
=======
    if(check) //wenn es nach dem "./" einen tieferen Pfad gegeben hat, wird dieser hier angehängt
>>>>>>> 912251e9fffe17bd68a18abcf7e2a7175c5dc6cc
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
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> 912251e9fffe17bd68a18abcf7e2a7175c5dc6cc



//----------------------------einfache filesuche-------------------------------------------------------//

void simpleSearch(const char* fileName, const char* path, bool& fileFound)
{
    struct dirent *direntp;
    DIR *dirp;

    if ((dirp = opendir(path)) == NULL) {
      perror ("Failed to open directory");
      return;
    }

    while ((direntp = readdir(dirp)) != NULL)
    {
        if(fileFound == true)
        {
            return;
        }

        if(strcmp(direntp->d_name, fileName) == 0)
        {

            string name = fileName;
            string way = path;
            string solid = pathfinder(way, name);
            solid = solid + name;
            //char* currentWDP = returnCWDP();
            //strcat(currentWDP, path);
            pid_t pid = getpid();
            cout<<"PID: "<<pid<<" reports -> File "<<fileName<<" found at: "<<solid<<endl;
            fileFound = true;
            return;
        }

    }
    while ((closedir(dirp) == -1) && (errno == EINTR)) ;
}



// --------------------einfache Suche mit groß und Kleinschreibung---------------------------

void simpleSearch_CSInsensitive(const char* fileName, const char* path, bool& fileFound)
{
    struct dirent *direntp;
    DIR *dirp;

    if ((dirp = opendir(path)) == NULL) {
      perror ("Failed to open directory");
      return;
    }

    while ((direntp = readdir(dirp)) != NULL)
    {
        if(fileFound == true)
        {
            return;
        }

        if(strcasecmp(direntp->d_name, fileName) == 0)
        {
            string name = fileName;
            string way = path;
            string solid = pathfinder(way, name);
            solid = solid + name;
            //char* currentWDP = returnCWDP();
            //strcat(currentWDP, path);
            pid_t pid = getpid();
            cout<<"PID: "<<pid<<" reports -> File "<<fileName<<" found at: "<<solid<<endl;
            //printf("PID: %ld reports -> File %s found at: %s\n", (long)pid, fileName ,currentWDP);
            fileFound = true;
            return;
        }

    }
    while ((closedir(dirp) == -1) && (errno == EINTR)) ;
}


//--------rekursive Suche mit groß und Kleinschreibung--------------

void searchFile_CSInsensitive(const char* fileName, const char* basePath, bool& fileFound)
{
    char path[PATH_MAX]; // 255, since d_name is a char array that holds a maximum of 256 characters
    struct dirent *direntp;
    DIR *dir = opendir(basePath);

    if(!dir)
    {
        return;
    }

    while((direntp = readdir(dir)) != NULL)
    {
        if(strcasecmp(direntp->d_name, ".") != 0 && strcasecmp(direntp->d_name, "..") != 0) // strcmp = StringCompare
        {
            if(fileFound == true)
            {
                return;
            }
//            printf("%s\n", direntp->d_name);

            // Construct new Path for our base Path
            strcpy(path, basePath);
            strcat(path, "/");
            strcat(path, direntp->d_name);

            if(strcasecmp(direntp->d_name, fileName) == 0)
            {
                string name = fileName;
                string way = path;
                string solid = pathfinder(way, name);


                //char* currentWDP = returnCWDP();
                //strcat(currentWDP, path);
                pid_t pid = getpid();
                cout<<"PID: "<<pid<<" reports -> File "<<fileName<<" found at: "<<solid<<endl;
                //printf("PID: %ld reports -> File %s found at: %s\n", (long)pid, fileName ,currentWDP);
                fileFound = true;
                return;
            }

            searchFile_CSInsensitive(fileName, path, fileFound);
        }
    }

}



//-----------------------------rekursive Suche--------------------------

void searchFile(const char* fileName, const char* basePath, bool& fileFound)
{
    char path[PATH_MAX]; // 255, since d_name is a char array that holds a maximum of 256 characters
    struct dirent *direntp;
    DIR *dir = opendir(basePath);

    if(!dir)
    {
        return;
    }

    while((direntp = readdir(dir)) != NULL)
    {
        if(strcmp(direntp->d_name, ".") != 0 && strcmp(direntp->d_name, "..") != 0) // strcmp = StringCompare
        {
            if(fileFound == true)
            {
                return;
            }
//            printf("%s\n", direntp->d_name);

            // Construct new Path for our base Path
            strcpy(path, basePath);
            strcat(path, "/");
            strcat(path, direntp->d_name);

            if(strcmp(direntp->d_name, fileName) == 0)
            {
                string name = fileName;
                string way = path;
                string solid = pathfinder(way, name);

                //char* currentWDP = returnCWDP();
                //strcat(currentWDP, path);
                pid_t pid = getpid();
                cout<<"PID: "<<pid<<" reports -> File "<<fileName<<" found at: "<<solid<<endl;
                //printf("PID: %ld reports -> File %s found at: %s\n", (long)pid, fileName ,currentWDP);
                fileFound = true;
                return;
            }

            searchFile(fileName, path, fileFound);
        }
    }

}


void print_usage( char *programm_name ) {
    printf("Usage: %s [-R] [-i] searchpath filename1 [filename2]...[filenameN]\n\n",programm_name);
    return;
}
<<<<<<< HEAD
>>>>>>> e006d68cff267b185ab090027fb11ded0ae9716c
=======
>>>>>>> 912251e9fffe17bd68a18abcf7e2a7175c5dc6cc
