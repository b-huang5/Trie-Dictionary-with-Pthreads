#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <string>
#include <string.h>
#include <fstream>
#include <cstring>
#include <cctype>

using namespace std;

#include "countwords.h"
#include "exec.h"

bool lastNode(struct dictNode *root) //if all child nodes are NULL, must be the last level of the path.
{
    for(int i = 0; i < NUMOfCHARS; i++)
    {
        if(!(root->children[i] == NULL))
        {
            return false;
        }
    }
    return true;
}

int recursiveOccurance(struct dictNode *root, int index)
{
    struct dictNode *map = root;
    int count = 0;
    map = map->children[index];
    if(map->isWord) //increment count if node isWord is true
    {
        count += 1;
    }
    if(lastNode(map)) //base case
    {
        return count;
    }
    for(int i = 0; i < NUMOfCHARS; i++)
    {
        if(!(map->children[i] == NULL))
        {
            count += recursiveOccurance(map, i); //recursive calling until last level
        }
    }
    return count;
}

int occurance(struct dictNode *root, std::string word)
{
    struct dictNode *map = root;
    for(int i = 0; i < word.length(); i++)
    {
        int index = word[i] - 'a';
        if(index == -58) //apostrophe to index 26
        {
            index = 26;
        }
        if(map->children[index] == NULL) //if the start of the node is NULL, the word cannot exist in the dictionary
        {
            return 0;
        }
        map = map->children[index];
    }
    int count = 0;
    if(map->isWord)
    {
        count += 1;
    }
    if(lastNode(map))
    {
        return count;
    }
    for(int i = 0; i < NUMOfCHARS; i++)
    {
        if(!(map->children[i] == NULL))
        {
            count += recursiveOccurance(map, i);
        }
    }
    return count;
}

void* countwordfunc(void *argument)
{
    EXEC_STATUS *countthread = (EXEC_STATUS*)argument;

    while(countthread->taskCompleted[DICTSRCFILEINDEX] != true)
    {
        sleep(1);
    }

    const char *delimiters = "\n\r !\"#$%&()*+,-./0123456789:;<=>?@[\\]^_`{|}~";
    std::ifstream file2(countthread->filePath[TESTFILEINDEX]);
    if(!file2.is_open())
    {
        std::cout << "Unable to open <<" << countthread->filePath[TESTFILEINDEX] << ">>";
        exit(EXIT_FAILURE);
    }

    std::ofstream outputFile("countwords_output.txt");
    std::string line;
    while(getline(file2,line))
    {
        countthread->numOfCharsProcessedFromFile[TESTFILEINDEX] = countthread->numOfCharsProcessedFromFile[TESTFILEINDEX] + line.length() + 1;
        char *cstr = new char[line.length() + 1];
        std::strcpy (cstr, line.c_str());
        char *p = std::strtok (cstr, delimiters);
        while(p!= nullptr)
        {
            std::string temp = std::string(p);
            for(int i = 0; i < temp.length(); i++)
            {
                temp[i] = tolower(temp[i]);
            }
            int holder = occurance(countthread->root, temp);
            if(holder >= countthread->minNumOfWordsWithAPrefixForPrinting)
            {
                outputFile << temp << " " << holder << '\n';
            }
            countthread->wordCountInFile[TESTFILEINDEX] += 1;
            p = std::strtok(NULL, delimiters);
        }
    }
    file2.close();
    outputFile.close();
    countthread->taskCompleted[TESTFILEINDEX] = true;
}
