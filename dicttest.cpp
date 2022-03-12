// Huang, Brian
// CS 480 T/Th 16:00-17:15
// Professor Shen
// 1 March, 2022


#include <iostream>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <math.h>

using namespace std;

#include "exec.h"
#include "populatetree.h"
#include "countwords.h"


int main(int argc, char **argv) {
    if(argc >= 3) 
    {
        int option;
        int count = 0;
        struct EXEC_STATUS *start = new EXEC_STATUS();
        struct stat file_size; 

        //initialize
        start->root = new dictNode();
        start->numOfProgressMarks = DEFAULT_NUMOF_MARKS;
        start->hashmarkInterval = DEFAULT_HASHMARKINTERVAL;
        start->minNumOfWordsWithAPrefixForPrinting = DEFAULT_MINNUM_OFWORDS_WITHAPREFIX;

        start->filePath[DICTSRCFILEINDEX] = argv[optind];
        optind += 1;
        start->filePath[TESTFILEINDEX] = argv[optind];
        optind += 1;

        //optional command line arguements
        while((option = getopt(argc, argv, "p:h:n:")) != -1)
        {
            switch(option)
            {
                case 'p':
                    if(atoi(optarg) >= MIN_NUMOF_MARKS)
                    {
                        start->numOfProgressMarks = atoi(optarg);
                    }
                    else
                    {
                        std::cout << "Number has to be greater than 10";
                        exit(EXIT_FAILURE);
                    }
                    break;
                case 'h':
                    if(atoi(optarg) > 0 && atoi(optarg) <= DEFAULT_HASHMARKINTERVAL)
                    {
                        start->hashmarkInterval = atoi(optarg);
                    }
                    else
                    {
                        std::cout << "Number has to be between 1-10";
                        exit(EXIT_FAILURE);
                    }
                    break;
                case 'n':
                    if(atoi(optarg) > DEFAULT_MINNUM_OFWORDS_WITHAPREFIX)
                    {
                        start->minNumOfWordsWithAPrefixForPrinting = atoi(optarg);
                    }
                    else
                    {
                        std::cout << "Number has to be greater than 0";
                        exit(EXIT_FAILURE);
                    }
                    break;
            }
        }

        //start pthreads
        pthread_t populate;
        pthread_t countword;
        pthread_create(&populate,NULL,&populatefunc,start);
        pthread_create(&countword,NULL,&countwordfunc,start);

        //grab file sizes
        if(stat(start->filePath[DICTSRCFILEINDEX], &file_size) != -1)
        {
            start->totalNumOfCharsInFile[DICTSRCFILEINDEX] = file_size.st_size;
        }
        if(stat(start->filePath[TESTFILEINDEX], &file_size) != -1)
        {
            start->totalNumOfCharsInFile[TESTFILEINDEX] = file_size.st_size;
        }

        double placeMark =  (double) 1 / start->numOfProgressMarks; //gets each percentage to place mark
        int numberOfMarks;
        int hashCatcher = 0;
        while(!(start->taskCompleted[DICTSRCFILEINDEX]))
        {
            numberOfMarks = ((double) start->numOfCharsProcessedFromFile[DICTSRCFILEINDEX] / (double) start->totalNumOfCharsInFile[DICTSRCFILEINDEX]) / placeMark; //number of marks that should be shown
            for(int i = 0; i < numberOfMarks - count; i++) //numberOfMarks - counts checks if system misses some checks and adds the missing marks. If the same it prints nothing.
            {
                hashCatcher += 1;
                if(hashCatcher % start->hashmarkInterval == 0)
                {
                    std::cout << "#";
                }
                else
                {
                    std::cout << "-";
                }
            }
            count = numberOfMarks; //makes sure next run doesn't have the same percentage.
        }
        cout << '\n';
        cout << "There are " << start->wordCountInFile[DICTSRCFILEINDEX] << " words in " << start->filePath[DICTSRCFILEINDEX] << "." << '\n';

        hashCatcher = 0;
        count = 0;
        while(!(start->taskCompleted[TESTFILEINDEX]))
        {
            numberOfMarks = ((double) start->numOfCharsProcessedFromFile[TESTFILEINDEX] / (double) start->totalNumOfCharsInFile[TESTFILEINDEX]) / placeMark; //number of marks that should be shown
            for(int i = 0; i < numberOfMarks - count; i++) //numberOfMarks - counts checks if system misses some checks and adds the missing marks. If the same it prints nothing.
            {
                hashCatcher += 1;
                if(hashCatcher % start->hashmarkInterval == 0)
                {
                    std::cout << "#";
                }
                else
                {
                    std::cout << "-";
                }
            }
            count = numberOfMarks; //makes sure next run doesn't have the same percentage.
        }
        cout << '\n';
        std::cout << "There are " << start->wordCountInFile[TESTFILEINDEX] << " words in " << start->filePath[TESTFILEINDEX] << "." << '\n';

        pthread_exit(&populate);
        pthread_exit(&countword);

        return 0;
    }
    else
    {
        std::cout << "Error";
    }
}