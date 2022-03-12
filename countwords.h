#include <string>
#include <string.h>

#ifndef countwords_h
#define countwords_h
    bool lastNode(struct dictNode *root);
    int recursiveOccurance(struct dictNode *root, int index);
    int occurance(struct dictNode *root, std::string word);
    void* countwordfunc(void *argument);
#endif