    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/wait.h>
    #include <sys/ipc.h>
    #include <sys/shm.h>
    #include <stdbool.h>
    #include <time.h>
    #include <math.h>
    
    // #define N 4
    #define MAXDIGITS 4
    #define BILLION  1000000000L;
 
 
    int main(int argc,char *argv[])
    {
        double tq=1;
        if(argc!=7){
            printf("Invalid number of arguments\n");
            return 0;
        }
        int i=atoi(argv[1]);
        int j=atoi(argv[2]);
        int k=atoi(argv[3]);
        
        char arg1[MAXDIGITS + 1];
        char arg2[MAXDIGITS + 1];
        char arg3[MAXDIGITS + 1];
        snprintf(arg1, MAXDIGITS + 1, "%d", i);
        snprintf(arg2, MAXDIGITS + 1, "%d", j);
        snprintf(arg3, MAXDIGITS + 1, "%d", k);
        // main = getpid();
        
        pid_t pid1 = fork();
        // printf("%s %s %s\n",arg1,arg2,arg3);
        if(pid1 == 0)
        {
            execlp("./first.out","./first.out",arg1,arg2,arg3,argv[4],argv[5],NULL);
        }
        else
        {
            int pp=kill(pid1, SIGSTOP);
            pid_t pid2 = fork();
            
            if(pid2 == 0){
                execlp("./second.out","./second.out",argv[4],argv[6],NULL);
                
            } 
            else {
                int zz=kill(pid2, SIGSTOP);
                printf("%d %d\n",pp,zz);
                int flag = 1;
                int status1,status2;
                int cc=0;
                struct timespec start, stop;
                double accum;
                if( clock_gettime( CLOCK_REALTIME, &start) == -1 ) {
                    perror( "clock gettime" );
                    return EXIT_FAILURE;
                }
                int f1,f2;
                int num1=0,num2=0;
 
                double timex=0;
                struct timespec startx, stopx;
 
 
 
                while((f1=(waitpid(pid1, &status1, WNOHANG)) )== 0 || (f2=(waitpid(pid2, &status2, WNOHANG)) )== 0){
                    // printf("%d\n",cc);
                     if(flag == 1){
                        if(f1 == 0){
 
                            if( clock_gettime( CLOCK_REALTIME, &startx) == -1 ) {
                                perror( "clock gettime" );
                                return EXIT_FAILURE;
                            }
                            kill(pid1, SIGCONT);
                            usleep(tq*100);
                            kill(pid1, SIGSTOP);
                            // sleep(0.01);
 
                            if( clock_gettime( CLOCK_REALTIME, &stopx) == -1 ) {
                                perror( "clock gettime" );
                                return EXIT_FAILURE;
                            }  
                            
                            num1++;
 
                            timex += ( stopx.tv_sec - startx.tv_sec )
                                + (double)( stopx.tv_nsec - startx.tv_nsec )
                                / (double)BILLION;
 
 
                        }
                         flag = 0;
                    }else {
                        if(f2==0){
                            kill(pid2, SIGCONT);
                            usleep(tq*100);
                            kill(pid2, SIGSTOP);
                            num2++;
                        }
                        flag = 1;
                    }
                }  
                if( clock_gettime( CLOCK_REALTIME, &stop) == -1 ) {
                    perror( "clock gettime" );
                    return EXIT_FAILURE;
                }  
                accum = ( stop.tv_sec - start.tv_sec )
                        + (double)( stop.tv_nsec - start.tv_nsec )
                        / (double)BILLION;
                printf( "%0.9lf\n", accum );
                printf("Total Turnaround Time for Process 1 = %lf\n", (num1+num1-1)*tq);
                printf("Waiting Time for Process 1 = %lf\n", (num1-1)*tq);
                printf("Total Turnaround Time for Process 2 = %lf\n", (num2+num1)*tq);
                printf("Waiting Time for Process 2 = %lf\n", num1*tq);
 
                printf("Timex = %lf\n", timex*10000);
                // printf("Num1 = %d \n", num1);
 
                printf("Context Switch Time = %lf\n",(timex*10000-num1*tq)/num1);// (timex - num1*tq)/num1);
                FILE *fp;
                fp = fopen("data3.txt","a");
                fprintf(fp,"%lf ",log10(i*j+j*k));
                fprintf(fp,"%lf ",(num1+num1-1)*tq);
                fprintf(fp,"%lf ",(num1-1)*tq);
                fprintf(fp,"%lf ",(num2+num1)*tq);
                fprintf(fp,"%lf ",num1*tq);
                fprintf(fp,"%lf\n",(timex*10000-num1*tq)/num1);
                fclose(fp);
            }
        }
        return 0;
    }