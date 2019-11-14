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
double total_average[4] = {0,0,0,0};

void Principle();
void init();
void simulate_car();
void check_boundary();
void turn(struct Car *tmp);
void move();



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
        if(r_spawn <= spawn_prob)  //spawn car
        {
            car_tmp = malloc(sizeof(struct Car));
            car_num++;
            car_tmp->x = entry_array[i].x;
            car_tmp->y = entry_array[i].y;
            car_tmp->BS_current[0] = entry_BS[i];
            car_tmp->BS_current[1] = entry_BS[i];
            car_tmp->BS_current[2] = entry_BS[i];
            car_tmp->BS_current[3] = entry_BS[i];
            car_tmp->totalpower[0] = 0;
            car_tmp->totalpower[1] = 0;
            car_tmp->totalpower[2] = 0;
            car_tmp->totalpower[3] = 0;
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

            turn(car_tmp);
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
        if(cur->x > BLOCK*4 || cur->x < 0 || cur->y > BLOCK*4 ||cur->y < 0 )  //out of boundary
        {
            //car_num--;
            total_average[0] += (cur->totalpower[0]/cur->time);
            total_average[1] += (cur->totalpower[1]/cur->time);
            total_average[2] += (cur->totalpower[2]/cur->time);
            total_average[3] += (cur->totalpower[3]/cur->time);
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

    if(tmp->x==0 && tmp->y==0 && tmp->direction_x == -1 && tmp->direction_y == 0)
    {
        tmp->direction_x = 0;
        tmp->direction_y = 1;
    }
    if(tmp->x==0 && tmp->y==0 && tmp->direction_x == 0 && tmp->direction_y == -1)
    {
        tmp->direction_x = 1;
        tmp->direction_y = 0;
    }
    if(tmp->x==4*BLOCK && tmp->y==0 && tmp->direction_x == 1 && tmp->direction_y == 0)
    {
        tmp->direction_x = 0;
        tmp->direction_y = 1;
    }
    if(tmp->x==4*BLOCK && tmp->y==0 && tmp->direction_x == 0 && tmp->direction_y == -1)
    {
        tmp->direction_x = -1;
        tmp->direction_y = 0;
    }
    if(tmp->x==0 && tmp->y==4*BLOCK && tmp->direction_x == 0 && tmp->direction_y == 1)
    {
        tmp->direction_x = 1;
        tmp->direction_y = 0;
    }
    if(tmp->x==0 && tmp->y==4*BLOCK && tmp->direction_x == -1 && tmp->direction_y == 0)
    {
        tmp->direction_x = 0;
        tmp->direction_y = -1;
    }
    if(tmp->x==4*BLOCK && tmp->y==4*BLOCK && tmp->direction_x == 1 && tmp->direction_y == 0)
    {
        tmp->direction_x = 0;
        tmp->direction_y = -1;
    }
    if(tmp->x==4*BLOCK && tmp->y==4*BLOCK && tmp->direction_x == 0 && tmp->direction_y == 1)
    {
        tmp->direction_x = -1;
        tmp->direction_y = 0;
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
    tmp->totalpower[0] += power[tmp->BS_current[0]];
    int maxpower;
    maxpower = findmax_power(power,tmp->BS_current[0]);
    if(maxpower != tmp->BS_current[0])
    {
        tmp->BS_current[0] = maxpower;
        handoff[0]++;
    }

}
void Threshold_policy(struct Car *tmp, double power[])
{
    tmp->totalpower[1] += power[tmp->BS_current[1]];
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
    tmp->totalpower[2] += power[tmp->BS_current[2]];
    int maxpower;
    maxpower = findmax_power(power,tmp->BS_current[2]);
    if((maxpower != tmp->BS_current[2] && power[maxpower] > (power[tmp->BS_current[2]] + E)))
    {
        tmp->BS_current[2] = maxpower;
        handoff[2]++;
    }
}

void Defined_policy(struct Car *tmp, double power[])
{
    tmp->totalpower[3] += power[tmp->BS_current[3]];
    int maxpower;
    maxpower = findmax_power(power,tmp->BS_current[3]);
    if((tmp->time % 300 == 0 || power[tmp->BS_current[3]] < Pmin) && maxpower != tmp->BS_current[3])
    {
        tmp->BS_current[3] = maxpower;
        handoff[3]++;
    }
}

void Principle()
{
    struct Car *tmp;
    double power[4] = {0,0,0,0}; //4 BS station current power to car
    for(tmp=car_head; tmp!=NULL; tmp=tmp->next)
    {
        power[0] = calculate_power(sqrt(pow((tmp->x - BS_array[0].x),2) + pow((tmp->y - BS_array[0].y),2)));
        power[1] = calculate_power(sqrt(pow((tmp->x - BS_array[1].x),2) + pow((tmp->y - BS_array[1].y),2)));
        power[2] = calculate_power(sqrt(pow((tmp->x - BS_array[2].x),2) + pow((tmp->y - BS_array[2].y),2)));
        power[3] = calculate_power(sqrt(pow((tmp->x - BS_array[3].x),2) + pow((tmp->y - BS_array[3].y),2)));
        Best_policy(tmp,power);
        Threshold_policy(tmp,power);
        Entrophy_policy(tmp,power);
        Defined_policy(tmp,power);
    }
}



int main(void)
{
    struct Car *car_tmp;
    int i;
    struct Car *tmp;
    FILE *fp;
    fp = fopen("handoff.txt","w");
    srand(time(NULL));
    init();
    for(i=1; i<=86400; i++)
    {
        simulate_car();
        move(); //car move
        Principle();
        check_boundary();// check if car is out of boundary
        fprintf(fp,"%d %d %d %d\n",handoff[0],handoff[1],handoff[2],handoff[3]);

    }
    fclose(fp);
    for(tmp=car_head; tmp!=NULL; tmp=tmp->next)
    {
        total_average[0] += (tmp->totalpower[0]/tmp->time);
        total_average[1] += (tmp->totalpower[1]/tmp->time);
        total_average[2] += (tmp->totalpower[2]/tmp->time);
        total_average[3] += (tmp->totalpower[3]/tmp->time);
    }
    printf("Total simulate car number: %d\n\n",car_num);
    printf("Best policy handoff:%d\n",handoff[0]);
    printf("Threshold policy handoff:%d\n",handoff[1]);
    printf("Entrophy policy handoff:%d\n",handoff[2]);
    printf("Defined policy handoff:%d\n\n",handoff[3]);
    printf("Best policy average power: %f\n",total_average[0]/car_num);
    printf("Threshold policy average power: %f\n",total_average[1]/car_num);
    printf("Entrophy policy average power: %f\n",total_average[2]/car_num);
    printf("Defined policy average power: %f\n",total_average[3]/car_num);
    return 0;
}

