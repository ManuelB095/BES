#ifndef KOPF_H_INCLUDED
#define KOPF_H_INCLUDED
#include <iostream>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <assert.h>



#ifndef PATH_MAX
#define PATH_MAX 255
#endif

using namespace std;

// überprüft ob der angegebene Pfad ein relativer Pfad ist oder ein absoluter Pfad und gibt den absoluten Pfad aus
string pathfinder (string weg, string filename);


// einfache filesuche
void simpleSearch(const char* fileName, const char* path, bool& fileFound);

//Einfache Suche mit Groß- und Kleinschreibung
void simpleSearch_CSInsensitive(const char* fileName, const char* path, bool& fileFound);

//rekursive Suche mit Gorß- und Kleinschreibung
void searchFile_CSInsensitive(const char* fileName, const char* basePath, bool& fileFound);

//rekursive Suche
void searchFile(const char* fileName, const char* basePath, bool& fileFound);


void printResult(bool& fileFound, const char* path);
void searchFile(const char* fileName, const char* basePath, bool& fileFound);


/* Hilfsfunktion */
void print_usage( char *programm_name );

char * sanitizePath(char* path);

#endif // KOPF_H_INCLUDED
