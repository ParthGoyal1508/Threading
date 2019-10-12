#define _POSIX_C_SOURCE 199309L //required for clock
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <pthread.h>

struct timespec current_time;
int m, n, k; //robots, tables, students

typedef struct robot
{
    int w, r, p, mode; //w second, r vessel, p students;mode: 0 can work and 1 for cannot work
} robot;

typedef struct serving_table
{
    int mode, portions, max_no_of_student, no_of_student_serving; // 0 for empty and 1 for serving; portions: no of students that can be served; no_of_st: seating capacity
} serving_table;

typedef struct student
{
    int arrived, tableno, iscomplete; //intime for entering the mess, tableno for biryani served
} student;

robot robots_arr[100005];
serving_table serving_tables_arr[100005];
student students_arr[100005];

pthread_mutex_t robots_mutex[100005];
pthread_mutex_t tables_mutex[100005];
pthread_mutex_t students_mutex[100005];

int num_student_left;
int total_num_students_left;
int alive = 1;
int total_portions;

void ready_to_serve_table(int id) //table id
{
    printf("Serving Table %d is ready to serve with %d slot.\n", id, serving_tables_arr[id].max_no_of_student);
    while (serving_tables_arr[id].portions > 0 && total_num_students_left > 0)
    {
        for (int i = 0; i < k; i++)
        {
            pthread_mutex_lock(&students_mutex[i]);
            if ((students_arr[i].arrived == 1) && (students_arr[i].iscomplete == 0) && (serving_tables_arr[id].portions > 0) && (serving_tables_arr[id].no_of_student_serving < serving_tables_arr[id].max_no_of_student))
            {
                serving_tables_arr[id].no_of_student_serving += 1;
                sleep(1);
                printf("Student %d on serving table %d has been served.\n", i, id);
                students_arr[i].iscomplete = 1;
                total_num_students_left -= 1;
                serving_tables_arr[id].portions -= 1;
                if (serving_tables_arr[id].portions == 0)
                {
                    serving_tables_arr[id].mode = 0;
                }
                pthread_mutex_unlock(&students_mutex[i]);
                if (total_num_students_left == 0)
                {
                    alive = 0;
                    printf("Simulation Over.\n");
                    return;
                }
            }
            pthread_mutex_unlock(&students_mutex[i]);
        }
    }
    printf("Serving Container of Table %d is empty, waiting for refill.\n", id);
    return;
}

void biryani_ready(int i) //robot id
{
    while ((alive == 1) && robots_arr[i].r > 0)
    {
        for (int j = 0; j < n; j++)
        {
            pthread_mutex_lock(&tables_mutex[j]);
            if ((robots_arr[i].r > 0) && (serving_tables_arr[j].mode == 0))
            {
                printf("Robot Chef %d is refilling Serving Container of Serving Table %d.\n", i, j);
                serving_tables_arr[j].mode = 1;
                serving_tables_arr[j].portions = robots_arr[i].p;
                robots_arr[i].r -= 1;
                pthread_mutex_unlock(&tables_mutex[j]);
                ready_to_serve_table(j);
                if (alive == 0)
                {
                    return;
                }
            }
            pthread_mutex_unlock(&tables_mutex[j]);
        }
    }
    robots_arr[i].mode = 0;
    printf("All the vessels prepared by Robot Chef %d are emptied. Resuming cooking now.\n", i);
    return;
}

void *robot_thread(void *robotId)
{
    int id = *(int *)robotId;
    while (alive == 1)
    {
        if (robots_arr[id].mode == 0)
        {
            pthread_mutex_lock(&robots_mutex[id]);
            robot temp;
            temp.w = (rand() % 4) + 2;
            temp.r = (rand() % 10) + 1;
            temp.p = (rand() % 26) + 25;
            total_portions += temp.p;
            printf("Robot Chef %d is preparing %d vessels of Biryani.\n", id, temp.r);
            sleep(temp.w);
            printf("Robot Chef %d has prepared %d vessels of Biryani. Waiting for all the vessels to be emptied to resume cooking.\n", id, temp.r);
            temp.mode = 1;
            robots_arr[id] = temp;
            biryani_ready(id); //mutex lock
            pthread_mutex_unlock(&robots_mutex[id]);
        }
    }
}

void *table_thread(void *tableId)
{
    int id = *(int *)tableId;
    serving_table table;
    table.mode = 0;
    table.portions = 0;
    table.max_no_of_student = (rand() % 10) + 1;
    table.no_of_student_serving = 0;
    serving_tables_arr[id] = table;
}

void wait_for_slot(int id)
{
    for (int i = 0; i < n; i++)
    {
        if (serving_tables_arr[i].no_of_student_serving < serving_tables_arr[i].max_no_of_student && serving_tables_arr[i].portions > 0)
        {
            students_arr[id].tableno = i;
            printf("Student %d assigned a slot on the serving table %d and waiting to be served.\n", id, i);
            return;
        }
    }
}

void *student_thread(void *studentId)
{
    int id = *(int *)studentId;
    student temp;
    pthread_mutex_lock(&students_mutex[id]);
    sleep(rand() % 15 + 1);
    printf("Student %d has arrived.\n", id);
    printf("Student %d is waiting to be allocated a slot on the serving table.\n", id);
    wait_for_slot(id);
    clock_gettime(CLOCK_MONOTONIC, &current_time);
    temp.arrived = 1;
    temp.tableno = -1;
    temp.iscomplete = 0;
    students_arr[id] = temp;
    pthread_mutex_unlock(&students_mutex[id]);
}

int main()
{
    scanf("%d %d %d", &m, &n, &k);

    total_num_students_left = k;

    pthread_t RobotThreads[m];
    pthread_t TableThreads[n];
    pthread_t StudentThreads[k];

    for (int i = 0; i < m; i++)
    {
        int *val = malloc(sizeof(*val));
        *val = i;
        pthread_mutex_init(&robots_mutex[i], NULL);
        pthread_create(&RobotThreads[i], NULL, robot_thread, (void *)(val));
    }

    for (int i = 0; i < n; i++)
    {
        int *val = malloc(sizeof(*val));
        *val = i;
        pthread_mutex_init(&tables_mutex[i], NULL);
        pthread_create(&TableThreads[i], NULL, table_thread, (void *)(val));
    }

    for (int i = 0; i < k; i++)
    {
        int *val = malloc(sizeof(*val));
        *val = i;
        pthread_mutex_init(&students_mutex[i], NULL);
        pthread_create(&StudentThreads[i], NULL, student_thread, (void *)(val));
    }

    for (int i = 0; i < m; i++)
        pthread_join(RobotThreads[i], NULL);
    for (int i = 0; i < n; i++)
        pthread_join(TableThreads[i], NULL);
    for (int i = 0; i < k; i++)
        pthread_join(StudentThreads[i], NULL);

    return 0;
}