#ifndef UTIL_H
#define UTIL_H

#include "TaskManager.h"
#include "stdlib.h"

inline int millis()
{
    int s = (int)TaskManager::Time();
    return s*1000;
}

inline void delay(int ms)
{
    TaskManager::DelayMs(ms);
}

inline int random(int min, int max)
{
    return rand()%(max-min+1)+min;
}

#define htons(x) ( ((x)<< 8 & 0xFF00) | \
                   ((x)>> 8 & 0x00FF) )
#define ntohs(x) htons(x)

#define htonl(x) ( ((x)<<24 & 0xFF000000UL) | \
                   ((x)<< 8 & 0x00FF0000UL) | \
                   ((x)>> 8 & 0x0000FF00UL) | \
                   ((x)>>24 & 0x000000FFUL) )
#define ntohl(x) htonl(x)

#endif
