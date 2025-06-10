#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <time.h>
#include <string.h>
#include <unistd.h> // usleep() 사용하기 위함
#include <pthread.h>
#include <sys/time.h>

#define BUFFER_SIZE 100

void *reader(void *arg);
void *writer(void *arg);


typedef struct{
    int id;
    int processing_time;
    struct timeval program_start_time; //time stamp 하기 위함 -> [0.0000]

} thread_args;



int main(int argc, char *argv[]){

    struct timeval my_time;
    gettimeofday(&my_time, NULL);

    if(argc !=2){
        printf("Usage: ./rwlock <sequence file>\n");
        return 1;
    }

    FILE *fp = fopen(argv[1], "r");
    if(fp == NULL){
        perror("Error");
        return 1;
    }

    char line_buffer[BUFFER_SIZE];
    char rw_type[10];

    int processing_time=0;
    int read_count=0, write_count=0, total_count=-1; // read, write 따로 증가하기에 변수 구별

    pthread_t threads[BUFFER_SIZE];

    while(fgets(line_buffer, BUFFER_SIZE, fp) != NULL){ // fgets로 line을 읽어서 나중에 공백으로 sccanf로 변수들을 구분분
        thread_args *ta = malloc(sizeof(thread_args));
        ta->program_start_time = my_time;

        if(ta == NULL){
            perror("Error malloc NULL");
            continue;
        }

        if(sscanf(line_buffer, "%s %d", rw_type, processing_time) !=2){
            perror("Error not two condition");
            continue;
        };
        
        total_count++;

        if(!strcmp(rw_type, "R")){ // rw_type이 R이면, reader 스레드 실행
            ta->id = ++read_count;
            ta->processing_time = processing_time;

            if(pthread_create(&threads[total_count], NULL, reader, (void*)ta) != 0){
                perror("Error: failed create read thread");
                free(ta);
                total_count--;
            }
        }
        else if(!strcmp(rw_type, "W")){ // rw_type이 W이면, writer 스레드 실행
            ta->id = ++write_count;
            ta->processing_time = processing_time;

            if(pthread_create(&threads[total_count], NULL, writer, (void*)ta) !=0){
                perror("Error: failed create write thread");
                free(ta);
                total_count--;
            }
        }
        
        usleep(100*1000); // 100ms
    }

    for(int i=0; i<=total_count; i++){
        pthread_join(threads[i], NULL);
    }

    fclose(fp);

    printf("End of sequence\n");
    

   
    return 0;
}

typedef struct time_stamp{
    long second;
    long ms;
}time_stamp;

time_stamp time_stamp_cal(struct timeval start_time){
    time_stamp result;

    struct timeval cur_time;
    gettimeofday(&cur_time, NULL);

    result.second = cur_time.tv_sec - start_time.tv_sec;
    result.ms = cur_time.tv_usec - start_time.tv_usec;

    if (result.ms < 0) { 
        result.second--;
        result.ms += 1000000; 
    }

    return result; 
}

void *reader(void *arg){
    thread_args *ta = (thread_args*)arg;
    time_stamp ts;

    ts = time_stamp_cal(ta->program_start_time);
    printf("[%ld.%04ld] Reader#%d: Created!\n", ts.second, ts.ms/100,  ta->id); // usec가 6자리이기에 네자리 고정으로 할려면 100으로 나눠서 4자리로 만들면 된다

    ts = time_stamp_cal(ta->program_start_time);
    printf("[%ld.%04ld] Reader#%d: Read started! (reading %d ms)\n", ts.second, ts.ms/100, ta->id, ta->processing_time);

    usleep(ta->processing_time * 1000);

    ts = time_stamp_cal(ta->program_start_time);
    printf("[%ld.%04ld] Reader#%d: Terminated\n",ts.second, ts.ms/100, ta->id); 


    free(ta); 
    return NULL;

}

void *writer(void *arg){
    thread_args *ta = (thread_args*)arg;
    time_stamp ts;

    ts = time_stamp_cal(ta->program_start_time);
    printf("[%ld.%04ld] Writer#%d: Created!\n", ts.second, ts.ms/100, ta->id); 

    ts = time_stamp_cal(ta->program_start_time);
    printf("[%ld.%04ld] Writer#%d: Write started! (writing %d ms)\n",ts.second, ts.ms/100, ta->id, ta->processing_time);


    usleep(ta->processing_time * 1000);

    ts = time_stamp_cal(ta->program_start_time);
    printf("[%ld.%04ld] Writer#%d: Terminated\n",ts.second, ts.ms/100, ta->id); 
    free(ta); 
    return NULL;
 

}


