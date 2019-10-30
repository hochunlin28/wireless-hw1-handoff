#include "simulate.h"
#include <time.h>
struct BS BS_array[4];
struct Entry entry_array[12];
struct Car *car_head = NULL;
double spawn_prob;
float r_turn; //random turn probability
float r_spawn; //random spawn car probability
int car_num = 0;
int handoff[4] = {0,0,0,0};

void init()
{
    spawn_prob = (1/30.0)*exp(-1/30.0);
    //printf("%f\n",spawn_prob);
    BS_array[0].x = BLOCK;
    BS_array[0].y = BLOCK;
    BS_array[1].x = BLOCK*3;
    BS_array[1].y = BLOCK;
    BS_array[2].x = BLOCK;
    BS_array[2].y = BLOCK*3;
    BS_array[3].x = BLOCK*3;
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
            car_tmp->BS_current[0] = entry_BS[i];
            car_tmp->BS_current[1] = entry_BS[i];
            car_tmp->BS_current[2] = entry_BS[i];
            car_tmp->BS_current[3] = entry_BS[i];
            car_tmp->totalpower = Px;
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
            //car_num--;
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

double calculate_power(double x)
{
    if(x<=1)
    {
        return Px;
    }
    else
    {
        return Px-20*log10(x);
    }
}

double findmax_power(double a[],int k)
{
    int max = k,i;
    for(i=0; i<=3; i++)
    {
        if(a[max]<a[i])
        {
            max = i;
        }
    }
    return max;
}

void Best_policy(struct Car *tmp,double power[]) //first policy
{
    int maxpower;
    maxpower = findmax_power(power,tmp->BS_current[0]);
    if(maxpower != tmp->BS_current[0])
    {
        /*printf("range: %f\n",sqrt(pow((tmp->x - BS_array[maxpower].x),2) + pow((tmp->y - BS_array[maxpower].y),2)));
        printf("location: %f %f\n",tmp->x,tmp->y);
        printf("dir: %d %d\n",tmp->direction_x,tmp->direction_y);
            printf("power: %f %f\n",power[maxpower],power[tmp->BS_current[0]]);
        printf("BS: %d %d\n",maxpower,tmp->BS_current[0]);*/
        tmp->BS_current[0] = maxpower;
        handoff[0]++;
    }

}
void Threshold_policy(struct Car *tmp, double power[])
{
    int maxpower;
    maxpower = findmax_power(power,tmp->BS_current[1]);
    if(maxpower != tmp->BS_current[1] && power[tmp->BS_current[1]] < T)
    {
        tmp->BS_current[1] = maxpower;
        handoff[1]++;
    }

}

void Entrophy_policy(struct Car *tmp, double power[])
{
    int maxpower;
    maxpower = findmax_power(power,tmp->BS_current[2]);
    if(maxpower != tmp->BS_current[2] && power[maxpower] > (power[tmp->BS_current[2]] + E))
    {
        tmp->BS_current[2] = maxpower;
        handoff[2]++;
    }
}

void Principle()
{
    struct Car *tmp;
    double power[4] = {0,0,0,0}; //4 BS station current power to car
    int maxpower[4] = {0,0,0,0},i; //4 principle
    for(tmp=car_head; tmp!=NULL; tmp=tmp->next)
    {
        power[0] = calculate_power(sqrt(pow((tmp->x - BS_array[0].x),2) + pow((tmp->y - BS_array[0].y),2)));
        power[1] = calculate_power(sqrt(pow((tmp->x - BS_array[1].x),2) + pow((tmp->y - BS_array[1].y),2)));
        power[2] = calculate_power(sqrt(pow((tmp->x - BS_array[2].x),2) + pow((tmp->y - BS_array[2].y),2)));
        power[3] = calculate_power(sqrt(pow((tmp->x - BS_array[3].x),2) + pow((tmp->y - BS_array[3].y),2)));
        Best_policy(tmp,power);
        Threshold_policy(tmp,power);
        Entrophy_policy(tmp,power);
    }
}



int main(void)
{
    struct Car *car_tmp;
    int i;
    srand(time(NULL));
    init();
    for(i=1; i<=86400; i++)
    {
        simulate_car();
        move(); //car move
        Principle();
        check_boundary();// check if car is out of boundary
    }
    printf("%d, %d %d %d\n",car_num,handoff[0],handoff[1],handoff[2]);
    return 0;
}

