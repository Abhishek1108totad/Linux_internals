#include <stdio.h>
#include <semaphore.h>

#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include<stdlib.h>
#include <string.h>
#include <unistd.h>

#define SHM_NAME        "my_shm"
#define SHM_DATA_SIZE   100

struct Shared_mry_structure
{
    /* data */
    sem_t mutex;
    int data_ready;
    char shared_mry_arr[SHM_DATA_SIZE];
};
