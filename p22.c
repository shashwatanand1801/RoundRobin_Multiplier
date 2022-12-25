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
     
    // #define N 3
    int N;
    #define BILLION  1000000000L;
     
     
    int *matrix1;
    int *matrix2;
    int *product;
    int fl , cl , rem;
    int *p ;
    int *p1,*p2,*p3;
    bool *b1;
    int unique_id_1,unique_id_2,unique_id_3,unique_id_4,unique_id_5,unique_id_6;
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
        int *data = (int *)arg;
        int upper = (data[0] == 1 ? cl : fl);
        int start = data[1]; 
        if(start>=i){
            pthread_exit(NULL);
        }
        for (int row = start; row < upper + start; row++) {
            for (int column = 0; column < k; column++) {
                int temp = 0;
                for (int jj = 0; jj < j; jj++) {
                    // int val1 = matrix1[row * j + jj] ; 
                    // int val2 = matrix2[jj * k + column]; 
                    temp += (matrix1[row * j + jj] * matrix2[jj * k + column]);
                }
              p[k*row + column] = temp;  
            }
        }
        pthread_exit(NULL);
    }
     
 
    int solve(int noOfThreads) {
        N = noOfThreads;
        
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
        p=(int *)malloc(sizeof(int)*i*k);
        struct timespec start, stop;
        double accum;
        if( clock_gettime( CLOCK_REALTIME, &start) == -1 ) {
          perror( "clock gettime" );
          return EXIT_FAILURE;
        }
        for (int i = 0; i < N; i++) {
          data = (int *)malloc(2 * sizeof(int));
          if(i < rem) {
            data[0] = 1;
            data[1] = i * cl;
          } else {
            data[0] = 0;
            data[1] = (rem * cl) + (i - rem) * fl;
          }
          pthread_create(&threads[i] , NULL , mult , (void *)data);
        }
        if( clock_gettime( CLOCK_REALTIME, &stop) == -1 ) {
          perror( "clock gettime" );
          return EXIT_FAILURE;
        }
        accum = ( stop.tv_sec - start.tv_sec )
                 + (double)( stop.tv_nsec - start.tv_nsec )
                   / (double)BILLION;
        printf( "%d %0.9lf\n",N, accum );
        for (int th = 0; th < N; th++) {
            int upper = (th < rem ? cl : fl);
            void *res;
            pthread_join(threads[th],NULL);
        }
        FILE *out;
        out=fopen("out.txt","w");
        for (int x = 0; x < i* k; x++) {
                fprintf(out,"%d ",p[x]);
                if((x + 1) % k == 0) {
                    fprintf(out,"\n");
                }
        }
        fclose(out);
        // printf("RESULTANT MATRIX IS IN THE FILE:- \n");
     
        // shmdt(matrix1);
        // shmdt(matrix2);
        // // destroy the shared memory
        shmctl(unique_id_1,IPC_RMID,NULL);
        shmctl(unique_id_2,IPC_RMID,NULL);
        shmctl(unique_id_3,IPC_RMID,NULL);
        shmctl(unique_id_4,IPC_RMID,NULL);
        shmctl(unique_id_5,IPC_RMID,NULL);
        shmctl(unique_id_6,IPC_RMID,NULL);
        printf("%d %0.9lf\n", N, accum);
        FILE *fp;
        fp = fopen("data2.txt","a+");
        fprintf(fp,"%d %0.9lf\n", N, accum);
        return 0;
        // printf("%d %0.9lf\n" , N , accum);
        return 0;
    }
 
    void attach(){
        key_t unique_key_1 = ftok("in1.txt", 10);
        key_t unique_key_2 = ftok("in1.txt", 11);
        key_t unique_key_3 = ftok("in1.txt", 12);
        key_t unique_key_4 = ftok("in1.txt", 13);
        key_t unique_key_5 = ftok("in1.txt", 14);
        key_t unique_key_6 = ftok("in1.txt",15);
        // shmget returns an identifier in shmid
        unique_id_3 = shmget(unique_key_3, sizeof(int), 0666 );
        unique_id_4 = shmget(unique_key_4, sizeof(int), 0666 );
        unique_id_5 = shmget(unique_key_5, sizeof(int), 0666 );
        unique_id_6 = shmget(unique_key_6,sizeof(bool),0666|IPC_CREAT);
        // shmat to attach to shared memory
        p1 = ((int *)shmat(unique_id_3, (void *)0, 0));
        p2 = ((int *)shmat(unique_id_4, (void *)0, 0));
        p3 = ((int *)shmat(unique_id_5, (void *)0, 0));
        b1 = ((bool*) shmat(unique_id_6,(void*)0,0));
     
        printf("%d %d %d %d\n", *p1, *p2, *p3,*b1);
        i = *p1;
        j = *p2;
        k = *p3;
        
        while(*b1==false){
            printf("Waiting for input\n");
        }
        unique_id_1 = shmget(unique_key_1, sizeof(int) * i * j, 0666 );
        unique_id_2 = shmget(unique_key_2, sizeof(int) * j * k, 0666 );
     
        matrix1 = (int *)shmat(unique_id_1, (void *)0, 0);
        matrix2 = (int *)shmat(unique_id_2, (void *)0, 0);
    }
    int main()
    {
      attach();
      FILE *fp;
      fp = fopen("data2.txt","w");
      fclose(fp);
      for (int i = 1; i <= 60; i++) {
        int dum=solve(i);
      } 
      return 0;
    }
