#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct nodes
{
    int left, right;
} nodes;

int arr[100005];
int partition(int left, int right)
{
    int pivot = arr[right];
    int i = left - 1;
    for (int j = left; j < right; j++)
    {
        // printf("left::%d, i::%d, j::%d, right::%d\n",left, i, j, right);
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

void *quicksort(void *arg)
{
    nodes *p = (nodes *)arg;
    int left = p->left;
    int right = p->right;

    if (left >= right)
        return NULL;
    else
    {
        int P_index = partition(left, right);
        pthread_t tid1, tid2;
        int ret;
        nodes left_child, right_child;
        left_child.left = left;
        left_child.right = P_index - 1;
        right_child.left = P_index + 1;
        right_child.right = right;
        ret = pthread_create(&tid1, NULL, quicksort, &left_child);
        ret = pthread_create(&tid2, NULL, quicksort, &right_child);
        pthread_join(tid2, NULL);
        pthread_join(tid1, NULL);
        pthread_exit(NULL);
        return NULL;
    }
}

int main()
{
    int i, n;
    scanf("%d", &n);

    for (i = 0; i < n; i++)
        scanf("%d", &arr[i]);

    struct timespec startTime, endTime;
    double timeSpent;
    clock_gettime(CLOCK_MONOTONIC, &startTime);

    pthread_t tid;
    nodes p;
    p.left = 0;
    p.right = n - 1;
    int ret = pthread_create(&tid, NULL, quicksort, &p);
    pthread_join(tid, NULL);
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
