#include "simulate.h"
#include <time.h>
struct BS BS_array[4];
struct Entry entry_array[12];
struct Car *car_head = NULL;
double spawn_prob;
float r_turn; //random turn probability
float r_spawn; //random spawn car probability
int car_num = 0;
void init()
{
    spawn_prob = (1/30.0)*exp(-1/30.0);
    //printf("%f\n",spawn_prob);
    BS_array[0].x = BLOCK;
    BS_array[0].y = BLOCK;
    BS_array[1].x = BLOCK*3;
    BS_array[1].x = BLOCK;
    BS_array[2].x = BLOCK;
    BS_array[2].y = BLOCK*3;
    BS_array[3].y = BLOCK*3;
    BS_array[3].y = BLOCK*3;
    entry_array[0].x = BLOCK;
    entry_array[0].y = 0;
    entry_array[1].x = BLOCK*2;
    entry_array[1].y = 0;
    entry_array[2].x = BLOCK*3;
    entry_array[2].y = 0;
    entry_array[3].x = 0;
    entry_array[3].y = BLOCK;
    entry_array[4].x = 0;
    entry_array[4].y = BLOCK*2;
    entry_array[5].x = 0;
    entry_array[5].y = BLOCK*3;
    entry_array[6].x = BLOCK;
    entry_array[6].y = BLOCK*4;
    entry_array[7].x = BLOCK*2;
    entry_array[7].y = BLOCK*4;
    entry_array[8].x = BLOCK*3;
    entry_array[8].y = BLOCK*4;
    entry_array[9].x = BLOCK*4;
    entry_array[9].y = BLOCK*3;
    entry_array[10].x = BLOCK*4;
    entry_array[10].y = BLOCK*2;
    entry_array[11].x = BLOCK*4;
    entry_array[11].y = BLOCK;
}

void simulate_car()
{
    int i;
    struct Car *cur;
    struct Car *car_tmp;

    for(i=0; i<12; i++)
    {
        r_spawn = (float)rand()/RAND_MAX;
        //printf("%f\n",r_spawn);
        if(r_spawn <= 0.03)  //spawn car
        {
            car_tmp = malloc(sizeof(struct Car));
            car_num++;
            car_tmp->x = entry_array[i].x;
            car_tmp->y = entry_array[i].y;
            car_tmp->BS_current = entry_BS[i];
            car_tmp->power = Px;
            car_tmp->time = 0;
            car_tmp->next = NULL;
            switch(i/3)
            {
            case 0:
                car_tmp->direction_x = 0;
                car_tmp->direction_y = 1;
                break;
            case 1:
                car_tmp->direction_x = 1;
                car_tmp->direction_y = 0;
                break;
            case 2:
                car_tmp->direction_x = 0;
                car_tmp->direction_y = -1;
                break;
            case 3:
                car_tmp->direction_x = -1;
                car_tmp->direction_y = 0;
                break;
            }
            if(car_head == NULL)
            {
                car_head = car_tmp;
            }
            else
            {
                for(cur = car_head; cur->next != NULL; cur = cur->next);
                cur->next = car_tmp;
            }
        }


    }
}



void check_boundary()
{
    struct Car *prev,*cur,*tmp;
    prev = NULL;
    cur = NULL;
    for(cur = car_head; cur != NULL;)
    {
        if(cur->x >= BLOCK*4 || cur->x <= 0 || cur->y >= BLOCK*4 ||cur->y <= 0 )  //out of boundary
        {
            car_num--;
            if(cur == car_head)
            {
                car_head = cur->next;
                tmp = cur;
                cur = cur->next;
                prev = NULL;
                free(tmp);
            }
            else
            {
                tmp = cur;
                prev->next = tmp->next;
                cur = cur->next;
                free(tmp);
            }
        }
        else
        {
            prev = cur;
            cur = cur->next;
        }
    }
}

void turn(struct Car *tmp)
{
    r_turn = (float)rand()/RAND_MAX;
    printf("%f %f time=%d\n",tmp->x,tmp->y,tmp->time);
    switch (tmp->direction_x)
    {
    case -1://left
        if(r_turn>0.5 && r_turn<=0.5+(1/3.0))
        {
            tmp->direction_x = 0;
            tmp->direction_y = -1;
        }
        else if(r_turn>0.5+(1/3.0))
        {
            tmp->direction_x = 0;
            tmp->direction_y = 1;
        }
        break;
    case 1: //right
        if(r_turn>0.5 && r_turn<=0.5+(1/3.0))  //turn right
        {
            tmp->direction_x = 0;
            tmp->direction_y = 1;
        }
        else if(r_turn>0.5+(1/3.0))  //turn left
        {
            tmp->direction_x = 0;
            tmp->direction_y = -1;
        }
        break;
    case 0:
        if(tmp->direction_y==1)  //down
        {
            if(r_turn>0.5 && r_turn<=0.5+(1/3.0))  //turn right
            {
                tmp->direction_x = -1;
                tmp->direction_y = 0;
            }
            else if(r_turn>0.5+(1/3.0))  //turn left
            {
                tmp->direction_x = 1;
                tmp->direction_y = 0;
            }

        }
        else  //up
        {
            if(r_turn>0.5 && r_turn<=0.5+(1/3.0))  //turn right
            {
                tmp->direction_x = 1;
                tmp->direction_y = 0;
            }
            else if(r_turn>0.5+(1/3.0))  //turn left
            {
                tmp->direction_x = -1;
                tmp->direction_y = 0;
            }
        }
        break;
    }
}

void move()
{
    struct Car *tmp;
    for(tmp=car_head; tmp!=NULL; tmp=tmp->next)
    {
        tmp->time = tmp->time+1;
        tmp->x = tmp->x + velocity*tmp->direction_x;
        tmp->y = tmp->y + velocity*tmp->direction_y;
        if(tmp->time % 75 == 0)
        {
            turn(tmp);
        }
    }
}
void calculate_Best()
{
    struct Car *tmp;
    for(tmp=car_head; tmp!=NULL; tmp=tmp->next)
    {
        //		printf("%f %f\n",(double)tmp->x/9, (double)tmp->y/9);
    }

}
int main(void)
{
    struct Car *car_tmp;
    int i;
    srand(time(NULL));
    init();
    for(i=1; i<=75+8; i++)
    {
        simulate_car();
        move(); //car move
        check_boundary();// check if car is out of boundary
    }
    return 0;
}

