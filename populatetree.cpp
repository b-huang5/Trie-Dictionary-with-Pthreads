#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <string>
#include <string.h>
#include <fstream>
#include <cstring>
#include <cctype>

using namespace std;

#include "populatetree.h"
#include "exec.h"

#define NUMOfCHARS 27

struct dictNode *initializeNode()
{
    dictNode *start = new dictNode();
    start->isWord = false;
    for(int i = 0; i < NUMOfCHARS; i++) 
    {
        start->children[i] = NULL; //sets each child node to NULL for each letter + apostrophe 
    }
    return start;
}

void insert(struct dictNode *root, std::string s) 
{
    struct dictNode *map = root;
    for(int i = 0; i < s.length(); i++) 
    {
        int index = s[i] - 'a'; //subtracting char 'a' results in corresponding index for letter in the alphabet except apostrophe
        if(index == -58) //apostrophe to index 26
        {
            index = 26;
        }
        if(map->children[index] == NULL) //if the node is empty, initialize node. Otherwise the path is already created from a previous string.
        {
            map->children[index] = initializeNode();
        }
        map = map->children[index]; //go down a level
    }
    map->isWord = true; //last letter of string is always a word
}

void *populatefunc(void *argument)
{
    EXEC_STATUS *popthread = (EXEC_STATUS*)argument;

    std::ifstream file(popthread->filePath[DICTSRCFILEINDEX]);
    if(!file.is_open())
    {
        std::cout << "Unable to open <<" << popthread->filePath[DICTSRCFILEINDEX] << ">>";
        exit(EXIT_FAILURE);
    }
    std::string line;
    const char *delimiters = "\n\r !\"#$%&()*+,-./0123456789:;<=>?@[\\]^_`{|}~";  
    while(getline(file,line))
    {
        char *cstr = new char[line.length() + 1];
        std::strcpy (cstr, line.c_str());
        char *p = std::strtok (cstr, delimiters);
        while(p!= nullptr)
        {
            std::string temp = std::string(p);
            for(int i = 0; i < temp.length(); i++)
            {
                temp[i] = tolower(temp[i]);
                popthread->numOfCharsProcessedFromFile[DICTSRCFILEINDEX] += 1;
            }
            popthread->numOfCharsProcessedFromFile[DICTSRCFILEINDEX] += 1;
            popthread->wordCountInFile[DICTSRCFILEINDEX] += 1;
            insert(popthread->root,temp);
            p = std::strtok(NULL, delimiters);
        }
    }
    popthread->taskCompleted[DICTSRCFILEINDEX] = true;
    file.close();
}