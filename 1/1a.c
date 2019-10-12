#define _POSIX_C_SOURCE 199309L //required for clock
#include <sys/types.h>
#include <sys/ipc.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int partition(int *arr, int left, int right)
{
    int pivot = arr[right];
    int i = left - 1;
    for (int j = left; j < right; j++)
    {
        if (arr[j] < pivot)
        {
            i++;
            int t = arr[i];
            arr[i] = arr[j];
            arr[j] = t;
        }
    }
    int t = arr[i + 1];
    arr[i + 1] = arr[right];
    arr[right] = t;
    return i + 1;
}

void quicksort(int *arr, int left, int right)
{
    if (left >= right)
        return;
    else
    {
        pid_t left_child, right_child;
        int P_index = partition(arr, left, right);
        left_child = fork();
        if (left_child == 0) //child process
        {
            quicksort(arr, left, P_index - 1);
            exit(0);
        }
        if (left_child > 0) // parent process
        { 
            right_child = fork();
            if (right_child == 0)//child of parent process
            {
                quicksort(arr, P_index + 1, right);
                exit(0);
            }
        }
        int ls, rs;
        waitpid(right_child, &rs, 0);
        waitpid(left_child, &ls, 0);
        return;
    }
}

int main()
{
    int *arr = NULL;
    int i, n;
    struct timespec startTime, endTime;
    double timeSpent;
    scanf("%d", &n);
    key_t key = IPC_PRIVATE;
    int shmid = shmget(key, 4 * (n + 1), IPC_CREAT | 0666);
    arr = shmat(shmid, 0, 0);
    for (i = 0; i < n; i++)
        scanf("%d", &arr[i]);
    clock_gettime(CLOCK_MONOTONIC, &startTime);
    quicksort(arr, 0, n - 1);
    clock_gettime(CLOCK_MONOTONIC, &endTime);
    timeSpent = (endTime.tv_sec - startTime.tv_sec);
    timeSpent += (endTime.tv_nsec - startTime.tv_nsec) / 1000000000.0;
    printf("Sorted Output: \n");
    for (i = 0; i < n; i++)
        printf("%d ", arr[i]);
    puts("");
    printf("Time Taken:\n%lf\n", timeSpent);
    return 0;
}
