#define _POSIX_C_SOURCE 199309L //required for clock
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

struct timespec current_time;
int n, m, k; //n cabs, m riders, k payment servers
int no_of_rider_left;

pthread_t *Rider_Threads;
pthread_t *Server_Threads;

typedef struct passenger
{
    int intime, maxwaittime, ridetime, outtime;
    int p_cabtype; // 0 for pool and 1 for premier
    int cabId;
    int status; //0 for not booked and 1 for booked
} passenger;

typedef struct driver
{
    int d_cabtype; // 0 for waitstate, 1 for onRidePremier, 2 for onridePoolOne, 3 for onridePoolfull
} driver;

passenger riders[100005];
driver drivers[100005];
int PaymentServerStatus[100005]; //id

pthread_mutex_t riders_mutex[100005];
pthread_mutex_t drivers_mutex[100005];
pthread_mutex_t paymentserverstatus_mutex[100005];

void acceptride(int p) //rider id
{
    char *cabtype = "Premier";
    if (riders[p].p_cabtype == 0)
    {
        cabtype = "Pool";
    }
    for (int i = 0; i < n; i++)
    {
        clock_gettime(CLOCK_MONOTONIC, &current_time);
        pthread_mutex_lock(&drivers_mutex[i]);
        int d_cabtype = drivers[i].d_cabtype;
        if ((d_cabtype == 0 || (d_cabtype == 2 && riders[p].p_cabtype == 0)) && riders[p].status == 0)
        {
            riders[p].status = 1;
            riders[p].cabId = i;
            // printf("Status of cab %d is %d\n", i, d_cabtype);
            printf("Rider %d has booked a cab number %d with type %s.\n", p, i, cabtype);
            if (riders[p].p_cabtype == 1)
            {
                drivers[i].d_cabtype = 1;
                int outtime = riders[p].ridetime + current_time.tv_sec;
                riders[p].outtime = outtime;
            }
            else if (riders[p].p_cabtype == 0)
            {
                if (d_cabtype == 0)
                {
                    drivers[i].d_cabtype = 2;
                    int outtime = riders[p].ridetime + current_time.tv_sec;
                    riders[p].outtime = outtime;
                }
                else if (d_cabtype == 2)
                {
                    drivers[i].d_cabtype = 3;
                    int outtime = riders[p].ridetime + current_time.tv_sec;
                    riders[p].outtime = outtime;
                }
            }
        }
        pthread_mutex_unlock(&drivers_mutex[i]);
    }
    return;
}

void initiatePayment(int p) //rider id
{
    int paymentDone = 0;
    printf("Rider %d is looking for a payment server.\n", p);
    while (paymentDone != 1)
    {
        for (int i = 0; i < k; i++)
        {
            pthread_mutex_lock(&paymentserverstatus_mutex[i]);
            if (PaymentServerStatus[i] == -1)
            {
                printf("Rider %d has initiated Payment on Server %d.\n", p, i);
                PaymentServerStatus[i] = p;
                paymentDone = 1;
                pthread_mutex_unlock(&paymentserverstatus_mutex[i]);
                return;
            }
            pthread_mutex_unlock(&paymentserverstatus_mutex[i]);
        }
    }
}

void endride(int p) //rider id
{
    sleep(riders[p].ridetime);
    printf("Rider %d has reached his destination.\n", p);
    initiatePayment(p);
    return;
}

void bookcab(int i) //rider id
{
    clock_gettime(CLOCK_MONOTONIC, &current_time);
    char *cabtype = "Premier";
    if (riders[i].p_cabtype == 0)
    {
        cabtype = "Pool";
    }
    printf("Rider %d is trying to book a cab with type %s.\n", i, cabtype);
    while ((current_time.tv_sec <= riders[i].intime + riders[i].maxwaittime) && (riders[i].status == 0))
    {
        acceptride(i);
        clock_gettime(CLOCK_MONOTONIC, &current_time);
    }
    if (riders[i].status == 0)
    {
        printf("Rider %d Timedout\n", i);
        no_of_rider_left -= 1;
    }
    else if (riders[i].status == 1)
    {
        endride(i); //rider id
    }
    return;
}

void *riders_thread(void *riderid)
{
    int id = *(int *)riderid;
    clock_gettime(CLOCK_MONOTONIC, &current_time);
    passenger rider;
    rider.intime = current_time.tv_sec;
    rider.maxwaittime = (rand() % 10) + 3;
    rider.ridetime = (rand() % 5) + 3;
    rider.p_cabtype = rand() % 2;
    rider.status = 0;
    rider.cabId = -1;
    rider.outtime = -1;
    riders[id] = rider;
    bookcab(id);
}

void *payments_thread(void *paymentserverid)
{
    int i = *(int *)paymentserverid;
    while (no_of_rider_left > 0)
    {
        pthread_mutex_lock(&paymentserverstatus_mutex[i]);
        if (PaymentServerStatus[i] != -1)
        {
            sleep(2);
            printf("Rider %d has completed his payment.\n", PaymentServerStatus[i]);
            no_of_rider_left -= 1;
            PaymentServerStatus[i] = -1;
        }
        pthread_mutex_unlock(&paymentserverstatus_mutex[i]);
    }
}

int main()
{
    printf("Enter no of cabs, no of riders and no of Payment Servers:\n");
    scanf("%d %d %d", &n, &m, &k);

    no_of_rider_left = m;

    pthread_t ridersThread[m];
    pthread_t paymentsThread[k];

    for (int i = 0; i < n; i++)
    {
        pthread_mutex_init(&drivers_mutex[i], NULL);
    }

    for (int i = 0; i < m; i++)
    {
        int *val = malloc(sizeof(*val));
        *val = i;
        pthread_create(&ridersThread[i], NULL, riders_thread, (void *)(val));
        pthread_mutex_init(&riders_mutex[i], NULL);
    }
    for (int i = 0; i < k; i++)
    {
        int *val = malloc(sizeof(*val));
        *val = i;
        PaymentServerStatus[i] = -1;
        pthread_create(&paymentsThread[i], NULL, payments_thread, (void *)(val));
        pthread_mutex_init(&paymentserverstatus_mutex[i], NULL);
    }

    for (int i = 0; i < m; i++)
        pthread_join(ridersThread[i], NULL);
    for (int i = 0; i < k; i++)
        pthread_join(paymentsThread[i], NULL);
    return 0;
}