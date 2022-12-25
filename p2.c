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
#include <string.h>
 
#define N 3
#define BILLION  1000000000L;
 
 
int *matrix1;
int *matrix2;
int *product;
char *file1;
char *file3;
int fl , cl , rem;
long long *p ;
 
int i;
int j;
int k;
 
void print_matrix1(int matrix1[])
{
    for (int r = 0; r < i * j; r++)
    {
        printf("%d ", matrix1[r]);
        if ((r + 1) % j == 0)
        {
            printf("\n");
        }
    }
}
 
void print_matrix2(int matrix1[])
{
    for (int r = 0; r < j * k; r++)
    {
        printf("%d ", matrix1[r]);
        if ((r + 1) % k == 0)
        {
            printf("\n");
        }
    }
}
 
void *mult(void *arg)
{
    int *data = (int * )arg;
    int upper = (data[0] == 1 ? cl : fl); //2
    int start = data[1]; //0
    if(start>=i){
        pthread_exit(NULL);
    }
    for (int row = start; row < upper + start; row++) {
        for (int column = 0; column < k; column++) {
            long long temp = 0;
            for (int jj = 0; jj < j; jj++) {
                int val1 = matrix1[row * j + jj] ; 
                int val2 = matrix2[jj * k + column]; 
                temp += (matrix1[row * j + jj] * matrix2[jj * k + column]);
                // if((matrix1[row * j + jj] * matrix2[jj * k + column])==0){
                //     printf("Here %d %d %d %d %d %d\n",i,jj,row * j + jj,jj * k + column,val1,val2);
                // }
                // if(row == 0 && column == 0) {
                //     printf("Va1l is: %d\n" , val1);
                //     printf("Val2 is: %d\n" , val2);
                // }
            }
          p[k*row + column] = temp;  
        //   printf("Row: %d Col: %d Result: %d\n" , row , column , temp);
        //   printf("%d \n",k*row + column);
        //   printf("%d \n",p[k*row + column]);
        }
    }
    // Used to terminate a thread and the return value is passed as a pointer
    // pthread_exit(&p);
    pthread_exit(NULL);
}
 
int main(int argc,char *argv[])
{
    if(argc!=3){
        printf("Please enter 2 files\n");
        exit(1);
    }
    file1=argv[1];
    file3=argv[2];
    key_t unique_key_1 = ftok(file1, 10);
    key_t unique_key_2 = ftok(file1, 11);
    key_t unique_key_3 = ftok(file1, 12);
    key_t unique_key_4 = ftok(file1, 13);
    key_t unique_key_5 = ftok(file1, 14);
    key_t unique_key_6 = ftok(file1, 15);
    // shmget returns an identifier in shmid
    int unique_id_3 = shmget(unique_key_3, sizeof(int), 0666 );
    int unique_id_4 = shmget(unique_key_4, sizeof(int), 0666 );
    int unique_id_5 = shmget(unique_key_5, sizeof(int), 0666 );
    int unique_id_6 = shmget(unique_key_6,sizeof(bool),0666|IPC_CREAT);
    // // shmat to attach to shared memory
    int *p1 = ((int *)shmat(unique_id_3, (void *)0, 0));
    int *p2 = ((int *)shmat(unique_id_4, (void *)0, 0));
    int *p3 = ((int *)shmat(unique_id_5, (void *)0, 0));
    bool *b1 = ((bool*) shmat(unique_id_6,(void*)0,0));
 
    printf("%d %d %d %d\n", *p1, *p2, *p3,*b1);
    i = *p1;
    j = *p2;
    k = *p3;
    
    while(*b1==false){
        printf("Waiting for input\n");
    }
    int unique_id_1 = shmget(unique_key_1, sizeof(int) * i * j, 0666 );
    int unique_id_2 = shmget(unique_key_2, sizeof(int) * j * k, 0666 );
 
    matrix1 = (int *)shmat(unique_id_1, (void *)0, 0);
    matrix2 = (int *)shmat(unique_id_2, (void *)0, 0);
    // print_matrix1(matrix1);
    // print_matrix2(matrix2);
 
    int max = i * k;
 
    // declaring array of threads of size r1*c2
    pthread_t *threads;
    threads = (pthread_t *)malloc(N * sizeof(pthread_t));
 
    int count = 0;
    int *data = NULL;
    cl = (i / N) + ((i % N) != 0);
    fl = i / N;
    rem = i % N;
    // printf("%d %d %d\n" , fl , cl , rem);
    p=(long long *)malloc(sizeof(long long)*i*k);
    struct timespec start, stop;
    double accum;
    if( clock_gettime( CLOCK_REALTIME, &start) == -1 ) {
      perror( "clock gettime" );
      return EXIT_FAILURE;
    }
    for (int i = 0; i < N; i++) {
    //   data = (int *)malloc((j + j + 3) * sizeof(int));
      data = (int *)malloc(2 * sizeof(int));
      if(i < rem) {
        data[0] = 1;
        data[1] = i * cl;
      } else {
        data[0] = 0;
        data[1] = (rem * cl) + (i - rem) * fl;
      }
    //   data[2] = j;
      pthread_create(&threads[i] , NULL , mult , (void *)data);
    }
    if( clock_gettime( CLOCK_REALTIME, &stop) == -1 ) {
      perror( "clock gettime" );
      return EXIT_FAILURE;
    }
    // for (int r = 0; r < max; r++)
    // {
    //     void *tt;
 
    //     // Joining all threads and collecting return value
    //     pthread_join(threads[r], &tt);
 
    //     int *p = (int *)tt;
    //     printf("%d ", *p);
    //     if ((r + 1) % k == 0)
    //         printf("\n");
    // }
    // product = (int *)malloc((i * k) * sizeof(int));
    accum = ( stop.tv_sec - start.tv_sec )
             + (double)( stop.tv_nsec - start.tv_nsec )
               / (double)BILLION;
    printf( "%d %0.9lf\n",N, accum );
    for (int th = 0; th < N; th++) {
        int upper = (th < rem ? cl : fl);
        void *res;
        // *res = (int *)malloc((upper * k) * sizeof(int));
        pthread_join(threads[th],NULL);
        // for (int x = 0; x < upper * k; x++) {
        //     printf("%d %d %d %d " ,th,x, upper*k ,p[x]);
            // if((x + 1) % k == 0) {
                // printf("\n");
            // }
        // }
    }
    FILE *out;
    printf("%s\n",file3);
    out=fopen(file3,"w");
    for (int x = 0; x < i* k; x++) {
            fprintf(out,"%lld ",p[x]);
            if((x + 1) % k == 0) {
                fprintf(out,"\n");
            }
    }
    fclose(out);
    printf("RESULTANT MATRIX IS IN THE FILE:- %s\n", file3);
 
    // shmdt(matrix1);
    // shmdt(matrix2);
    // // destroy the shared memory
    shmctl(unique_id_1,IPC_RMID,NULL);
    shmctl(unique_id_2,IPC_RMID,NULL);
    shmctl(unique_id_3,IPC_RMID,NULL);
    shmctl(unique_id_4,IPC_RMID,NULL);
    shmctl(unique_id_5,IPC_RMID,NULL);
    shmctl(unique_id_6,IPC_RMID,NULL);
 
    return 0;
}