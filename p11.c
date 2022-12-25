    #include <pthread.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <unistd.h>
    #include <sys/wait.h>
    #include <time.h>
    #include <stdbool.h>
    #include <sys/time.h>
    #include <sys/ipc.h>
    #include <sys/shm.h>
     
    // #define N 1
    int N;
    #define BILLION 1000000000L;
     
    pthread_t *t1;
    pthread_t *t2;
    int i, j, k;
    int to_read_per_thread_1 = 0;
    int to_read_per_thread_2 = 0;
    int *matrix1;
    int *matrix2;
    int *p1,*p2,*p3;
    bool *b1;
     
    typedef struct
    {
        int id;
    } tosend;
    tosend *obj;
     
    void print_matrix(int matrix1[])
    {
        for (int k = 0; k < i * j; k++)
        {
            printf("%d ", matrix1[k]);
            if ((k + 1) % j == 0)
            {
                printf("\n");
            }
        }
    }
    pthread_mutex_t lock;
     
    void *read_file_1(void *arg)
    {
        tosend obj = *((tosend *)arg);
        int id = obj.id;
        FILE *fp;
        fp = fopen("in1.txt", "r");
        if (fp == NULL)
        {
            printf("Error!");
            exit(1);
        }
        int start = id * to_read_per_thread_1;
        int end = (id + 1) * to_read_per_thread_1;
        if (id == N - 1)
        {
            end = i * j;
        }
        for (int r = 0; r < start; r++)
        {
            fscanf(fp, "%*s");
        }
        // printf("Matrix 1 : Thread %d is reading from position %d to position %d\n",id,start,end-1);
        for (int r = start; r < end; r++)
        {
            // printf("%d ",r);
            fscanf(fp, "%d ", &matrix1[r]);
        }
        pthread_exit(NULL);
    }
     
    void *read_file_2(void *arg)
    {
        tosend obj = *((tosend *)arg);
        int id = obj.id;
        FILE *fp;
        fp = fopen("in2.txt", "r");
        if (fp == NULL)
        {
            printf("Error!");
            exit(1);
        }
        int start = id * to_read_per_thread_2;
        int end = (id + 1) * to_read_per_thread_2;
        if (id == N - 1)
        {
            end = j * k;
        }
        for (int r = 0; r < start; r++)
        {
            fscanf(fp, "%*s");
        }
        // printf("Matrix 2 : Thread %d is reading from position %d to position %d\n",id,start,end-1);
        for (int r = start; r < end; r++)
        {
            fscanf(fp, "%d ", &matrix2[r]);
        }
        pthread_exit(NULL);
    }
     
    int solve(int ii, int jj, int kk, int noOfThreads)
    {
        i = ii;
        j = jj;
        k = kk;
        N = noOfThreads;
        t1 = (pthread_t *)malloc(N * sizeof(pthread_t));
        t2 = (pthread_t *)malloc(N * sizeof(pthread_t));
        obj = (tosend *)malloc(N * sizeof(tosend));
        pthread_mutex_init(&lock, NULL);
        struct timespec start, stop;
        double accum;
        to_read_per_thread_1 = i * j / N;
        to_read_per_thread_2 = j * k / N;
     
        // creating shared memory
        // printf("%d %d %d\n",*p1,*p2,*p3);
        printf("%d %d %d\n", i, j, k);
        if (clock_gettime(CLOCK_REALTIME, &start) == -1)
        {
            perror("clock gettime");
            return EXIT_FAILURE;
        }
        for (int r = 0; r < N; r++)
        {
            obj[r].id = r;
            pthread_create(&t1[r], NULL, read_file_1, (void *)&obj[r]);
            pthread_create(&t2[r], NULL, read_file_2, (void *)&obj[r]);
        }
        for (int r = 0; r < N; r++)
        {
            pthread_join(t1[r], NULL);
            pthread_join(t2[r], NULL);
        }
        if (clock_gettime(CLOCK_REALTIME, &stop) == -1)
        {
            perror("clock gettime");
            return EXIT_FAILURE;
        }
        *b1 = true;
        accum = (stop.tv_sec - start.tv_sec) + (double)(stop.tv_nsec - start.tv_nsec) / (double)BILLION;
        printf("%d %0.9lf\n", N, accum);
        printf("%d \n", matrix1[0]);
        // shmdt(matrix1);
        // shmdt(matrix2);
        // shmdt(p1);
        // shmdt(p2);
        // shmdt(p3);
        // // destroy the shared memory
        // shmctl(unique_id_1, IPC_RMID, NULL);    
        // shmctl(unique_id_2, IPC_RMID, NULL);
        // shmctl(unique_id_3, IPC_RMID, NULL);
        // shmctl(unique_id_4, IPC_RMID, NULL);
        // shmctl(unique_id_5, IPC_RMID, NULL);
        // shmctl(unique_id_6, IPC_RMID, NULL);
        // free(t1);
        // free(t2);
        // free(obj);
        printf("%d %0.9lf\n", N, accum);
        FILE *fp;
        fp = fopen("data1.txt","a+");
        fprintf(fp,"%d %0.9lf\n", N, accum);
        return 0;
    }
     
    void attach(){
        key_t unique_key_1 = ftok("in1.txt", 10);
        key_t unique_key_2 = ftok("in1.txt", 11);
        key_t unique_key_3 = ftok("in1.txt", 12);
        key_t unique_key_4 = ftok("in1.txt", 13);
        key_t unique_key_5 = ftok("in1.txt", 14);
        key_t unique_key_6 = ftok("in1.txt", 15);
        // shmget returns an identifier in shmid
        int unique_id_1 = shmget(unique_key_1, sizeof(int) * i * j, 0666 | IPC_CREAT);
        int unique_id_2 = shmget(unique_key_2, sizeof(int) * j * k, 0666 | IPC_CREAT);
        int unique_id_3 = shmget(unique_key_3, sizeof(int), 0666 | IPC_CREAT);
        int unique_id_4 = shmget(unique_key_4, sizeof(int), 0666 | IPC_CREAT);
        int unique_id_5 = shmget(unique_key_5, sizeof(int), 0666 | IPC_CREAT);
        int unique_id_6 = shmget(unique_key_6, sizeof(bool), 0666 | IPC_CREAT);
        // shmat to attach to shared memory
        matrix1 = (int *)shmat(unique_id_1, (void *)0, 0);
        matrix2 = (int *)shmat(unique_id_2, (void *)0, 0);
        p1 = ((int *)shmat(unique_id_3, (void *)0, 0));
        p2 = ((int *)shmat(unique_id_4, (void *)0, 0));
        p3 = ((int *)shmat(unique_id_5, (void *)0, 0));
        b1 = ((bool *)shmat(unique_id_6, (void *)0, 0));
        *b1 = false;
        *p1 = i;
        *p2 = j;
        *p3 = k;
    }
    int main(int argc, char *argv[])
    {
        i=atoi(argv[1]);
        j=atoi(argv[2]);
        k=atoi(argv[3]);
        attach();
        FILE *fp;
        fp = fopen("data1.txt","w");
        fclose(fp);
        for (int i = 1; i < 61; i++)
        {
            int vin=solve(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), i);
        }
        return 0;
    }
