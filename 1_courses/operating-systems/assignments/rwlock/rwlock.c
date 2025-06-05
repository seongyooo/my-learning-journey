#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <time.h>
#include <string.h>
#include <unistd.h> // usleep() 사용하기 위함
#include <pthread.h>

#define rw_count 100
#define BUFFER_SIZE 100

// void create_sequence_file();
// void rwlock_init(rwlock_t *rw);
// void rwlock_acquire_readlock(rwlock_t *rw);
// void rwlock_release_readlock(rwlock_t *rw);
// void rwlock_acquire_writelock(rwlock_t *rw);
// void rwlock_release_writelock(rwlock_t *rw);
void *reader(void *arg);
void *writer(void *arg);

// typedef struct _rwlock_t{
//     sem_t lock;
//     sem_t write_lock;
//     int readers;
// } rwlock_t;

typedef struct{
    int id;
    int processing_time;

} thread_args;



int main(int argc, char *argv[]){

    if(argc !=2){
        printf("Usage: ./rwlock <sequence file>\n");
        return 1;
    }

    FILE *fp = fopen(argv[1], "r");
    if(fp == NULL){
        perror("Error");
        return 1;
    }

    char thread[rw_count];
    char rw_type[10];
    int processing_time=0;
    int read_count=0, write_count=0, total_count=-1;

    pthread_t threads[rw_count];

    while(fgets(thread, BUFFER_SIZE, fp) != NULL){
        thread_args *ta = malloc(sizeof(thread_args));
        if(ta == NULL){
            perror("Error malloc NULL");
            continue;
        }

        if(sscanf(thread, "%s %d", rw_type, &processing_time) !=2){
            perror("Error not two condition");
            continue;
        };
        
        total_count++;

        if(!strcmp(rw_type, "R")){ 
            ta->id = ++read_count;
            ta->processing_time = processing_time;

            if(pthread_create(&thread[total_count], NULL, reader, (void*)ta) != 0){
                perror("Error: failed create read thread");
                free(ta);
                total_count--;
            }
        }
        else if(!strcmp(rw_type, "W")){
            ta->id = ++write_count;
            ta->processing_time = processing_time;

            if(pthread_create(&thread[total_count], NULL, writer, (void*)ta) !=0){
                perror("Error: failed create write thread");
                free(ta);
                total_count--;
            }
        }
        
        usleep(100*1000); // 100ms
    }

    for(int i=0; i<=total_count; i++){
        pthread_join(thread[i], NULL);
    }

    fclose(fp);

    printf("End of sequence\n");
    

   
    return 0;
}


// // 100개의 read/write 작업 + 100~500의 processing time을 갖는 작업을 생성
// void create_sequence_file() {
//     FILE *file = fopen("sequence.txt", "w");
//     if (file == NULL) {
//         printf("Error opening file\n");
//         return;
//     }

//     char rw[2] = {'R', 'W'};
//     int random_number = 0;

//     for (int i = 0; i < rw_count; i++) {
//         random_number = (rand() % 5 + 1) * 100;
//         int rw_random = rand() % 2;

//         fprintf(file, "%c %d\n", rw[rw_random], random_number);
//     }

//     fclose(file);
// }

// void read_sequence_file(rwlock_t *rw){
//     FILE *file = fopen("sequence.txt", "r");
//     if (file == NULL) {
//         printf("Error opening file\n");
//         return;
//     }
// }


// void rwlock_init(rwlock_t *rw){
//     rw->readers = 0;
//     sem_init(&rw->lock, 0, 1);
//     sem_init(&rw->write_lock, 0, 1);
// }

// void rwlock_acquire_readlock(rwlock_t *rw){
//     sem_wait(&rw->lock);
//     rw->readers++;
//     if(rw->readers == 1){
//         sem_wait(&rw->write_lock);
//     }
//     sem_post(&rw->lock);
// }

// void rwlock_release_readlock(rwlock_t *rw){
//     sem_wait(&rw->lock);
//     rw->readers--;
//     if(rw->readers == 0){
//         sem_post(&rw->write_lock);
//     }
//     sem_post(&rw->lock);
// }

// void rwlock_acquire_writelock(rwlock_t *rw){
//     sem_wait(&rw->write_lock);
// }

// void rwlock_release_writelock(rwlock_t *rw){
//     sem_post(&rw->write_lock);
// }

void *reader(void *arg){
    thread_args *ta = (thread_args*)arg;

    printf("[] Reader#%d: Created!\n", ta->id); 
    printf("[] Reader#%d: Read started! (reading %d ms)\n", ta->id, ta->processing_time);

    usleep(ta->processing_time * 1000);
    printf("[] Reader#%d: Terminated\n", ta->id); 
    free(ta); 
    return NULL;

}

void *writer(void *arg){
    thread_args *ta = (thread_args*)arg;

    printf("[] Writer#%d: Created!\n", ta->id); 
    printf("[] Writer#%d: Write started! (writing %d ms)\n", ta->id, ta->processing_time);

    usleep(ta->processing_time * 1000);
    printf("[] Writer#%d: Terminated\n", ta->id); 
    free(ta); 
    return NULL;
 

}


