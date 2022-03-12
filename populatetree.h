#include <string>
#include <string.h>

#ifndef populatetree_h
#define populatetree_h
    struct dictNode *initializeNode();
    void insert(struct dictNode *root, std::string s);
    void *populatefunc(void *argument);
#endif