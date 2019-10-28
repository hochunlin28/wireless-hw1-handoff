#ifndef SIMULATE_H
#define SIMULATE_H

#define Px -60 //less than 1m
#define velocity_change 100
#define velocity_real 100/9
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//real distance is (x or y)/9
struct BS
{
    int x;
    int y;
};
struct Entry
{
    int x;
    int y;
};

struct Car
{
    int  x;
    int  y;
    int direction_x;
    int direction_y;
    struct Car *next;
};

int entry_bs[12] = {0,0,1,0,2,2,2,3,3,3,1,1};
#endif  //ifndef COM_APP_H
