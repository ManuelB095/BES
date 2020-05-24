#include "gridserver.h"

using namespace std;

bool in_array(char c, const Car myCars[])
{
    unsigned int arrSize = 26; //sizeof(myCars)/sizeof(myCars[0]); // Should be 26
    for(unsigned int i=0; i<arrSize;i++)
    {
        if(myCars[i].name == c)
            return true;
    }
    return false;
}

int charToNumType(const char c) // Casts char to integer; 'A' = 0; 'Z' = 26
{
    int result = c - 'A';
    return result;
}
