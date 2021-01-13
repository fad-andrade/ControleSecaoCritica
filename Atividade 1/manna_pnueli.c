#include<sys/time.h>
#include<pthread.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<time.h>

#define TRUE 1
#define FALSE 0

#define MAX_THREADS 4

#define TARGET 50000

int SOMA = 0;
long request = -1, respond = -1;

void* server(void *args){
    while(TRUE){
        while(request == -1){}
        respond = request;
        while(respond != -1){}
        request = -1;
    }
    pthread_exit(0);
}

void* client(void *th){
    long thd;
    thd = (long) th;

    while(TRUE){
        while(respond != thd){
            request = thd;
        }

        printf("%ld ENTER\n", thd);
        
        if(SOMA >= TARGET){
            respond = -1;
            printf("%ld exit\n", thd);
            pthread_exit(0);
        }
        int local = SOMA;
        usleep((random() % 2) * 1000);
        SOMA = local + 1;

        printf("%ld exit\n", thd);

        respond = -1;
    }
}

void* client_wrong(void *th){
    long thd;
    thd = (long) th;

    while(TRUE){
        printf("%ld ENTER\n", thd);
        if(SOMA >= TARGET){
            printf("%ld exit\n", thd);
            pthread_exit(0);
        }
        int local = SOMA;
        usleep((random() % 2) * 1000);
        SOMA = local + 1;
        printf("%ld exit\n", thd);
    }
}

void with_mutual_exclusion(){
    pthread_t server_t;
    pthread_t clients_t[MAX_THREADS];

    pthread_create(&server_t, NULL, server, NULL);

    for(long th = 0; th < MAX_THREADS; th++){
        pthread_create(&clients_t[th], NULL, client, (void*) th);
    }
    for(long th = 0; th < MAX_THREADS; th++){
        pthread_join(clients_t[th], NULL);
    }

    printf("SOMA = %d\n", SOMA);
}

void without_mutual_exclusion(){
    pthread_t clients_t[MAX_THREADS];

    for(long th = 0; th < MAX_THREADS; th++){
        pthread_create(&clients_t[th], NULL, client_wrong, (void*) th);
    }
    for(long th = 0; th < MAX_THREADS; th++){
        pthread_join(clients_t[th], NULL);
    }

    printf("SOMA = %d\n", SOMA);
}

int main(){

    // Clock initialize
    struct timeval start, end;
    gettimeofday(&start, NULL);

    srand (time(NULL));

    //with_mutual_exclusion();
    without_mutual_exclusion();

    // Clock finalize
    gettimeofday(&end, NULL);

    printf("%ldms to run the main function\n", (1000 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000));

    return 0;
}