#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <time.h>
#include <string.h>
#include <unistd.h> // usleep() 사용하기 위함

#define rw_count 100

// void create_sequence_file();
// void rwlock_init(rwlock_t *rw);
// void rwlock_acquire_readlock(rwlock_t *rw);
// void rwlock_release_readlock(rwlock_t *rw);
// void rwlock_acquire_writelock(rwlock_t *rw);
// void rwlock_release_writelock(rwlock_t *rw);
// void *reader(void *arg, int processing_time);
// void *writer(void *arg, int processing_time);

// typedef struct _rwlock_t{
//     sem_t lock;
//     sem_t write_lock;
//     int readers;
// } rwlock_t;

// typedef struct{
//     rwlock_t *rw;
//     int id;
//     int processing_time;
//      // struct timeval start_time;
//     char type; // 'R' for reader, 'W' for writer
// } thread_args;



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
    char rw[10];
    int processing_time=0;
    int read_count=0, write_count=0;

    while(fgets(thread, rw_count, fp) != NULL){
        sscanf(thread, "%s %d", rw, &processing_time);
        

        if(!strcmp(rw, "R")) printf("[] Reader#%d: Read started! (reading %d ms)\n" , ++read_count, processing_time);
        else if(!strcmp(rw, "W")) printf("[] Writer#%d: Write started! (writing %d ms)\n" , ++write_count, processing_time);

        usleep(100*1000); // 100ms
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

// void *reader(void *arg, int processing_time){
//     rwlock_t *rw = (rwlock_t *)arg;
//     int id = *((int *)arg);


//     rwlock_acquire_readlock(rw);
//     printf("Reader %d is reading\n", id);
//     sleep(processing_time);
//     rwlock_release_readlock(rw);
// }

// void *writer(void *arg, int processing_time){
//     rwlock_t *rw = (rwlock_t *)arg;
//     int id = *((int *)arg);

//     rwlock_acquire_writelock(rw);
//     printf("Writer %d is writing\n", id);
//     sleep(processing_time);
//     rwlock_release_writelock(rw);

// }


