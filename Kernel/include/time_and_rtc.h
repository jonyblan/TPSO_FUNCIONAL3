#ifndef _TIME_H_
#define _TIME_H_
#include <stdint.h>

typedef struct
{
    uint8_t seconds;
    uint8_t minutes;
    int8_t hours;
    uint16_t year;
    uint8_t month;
    uint8_t day;
} TimeStamp;

void timer_handler();
int ticks_elapsed();
int seconds_elapsed();
void sleep(uint32_t ticksToWait);
void setTickFrequency(uint16_t freq);
void getTime(TimeStamp *ts);
#endif
