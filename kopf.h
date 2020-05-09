#ifndef KOPF_H_INCLUDED
#define KOPF_H_INCLUDED
#include <iostream>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string>
using namespace std;

// überprüft ob der angegebene Pfad ein relativer Pfad ist oder ein absoluter Pfad und gibt den absoluten Pfad aus
string pathfinder (string weg, string filename);

#endif // KOPF_H_INCLUDED
