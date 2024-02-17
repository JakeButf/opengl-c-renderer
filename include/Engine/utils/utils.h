#ifndef UTILS_H
#define UTILS_H
#include <stdlib.h>
#include <assert.h>

struct iVector2
{
    int x;
    int y;
}; 

//Returns random integer between 0 and arg max
int randint(int max)
{
    if((max - 1) == RAND_MAX)
        return rand();
    else 
    {
        assert(max <= RAND_MAX);

        int end = RAND_MAX / max;
        assert(end > 0);

        end *= max;

        int r;
        while ((r = rand()) >= end);

        return r % max;
    }
}
#endif