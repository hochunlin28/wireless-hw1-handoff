#ifndef SIMULATE_H
#define SIMULATE_H

#define Px -50 //less than 1m
#define velocity 10
#define BLOCK 750
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//real distance is (x or y)/9
struct BS
{
    double x;
    double y;
};
struct Entry
{
    double x;
    double y;
};

struct Car
{
    double  x;
    double  y;
    int direction_x;
    int direction_y;
    double power;
    int BS_current; //current BS
    int time; //car exist time
    struct Car *next;
};

int entry_BS[12] = {0,0,1,0,2,2,2,3,3,3,1,1};
#endif  //ifndef COM_APP_H
