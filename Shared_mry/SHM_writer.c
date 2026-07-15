#include "Shared_mry.h"

int main(){

    int shm_fd = shm_open(SHM_NAME, O_RDWR | O_CREAT, 0666);

    if(shm_fd == -1){
        perror("SHM open failed");
        return EXIT_FAILURE;
    }
    
    if(ftruncate(shm_fd, sizeof(struct Shared_mry_structure)) == -1){
        perror("ftruncate is failed");
        return EXIT_FAILURE;
    }

    struct Shared_mry_structure *shm_ptr = (struct Shared_mry_structure *) mmap(NULL, sizeof(struct Shared_mry_structure), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    if(shm_ptr == MAP_FAILED){
        perror("MApping failed");
        return EXIT_FAILURE;
    }

    shm_ptr->data_ready = 0;

    printf("[WRITER] Shared Memory initialized at physical RAM layer.\n");
    printf("[WRITER] Waiting 5 seconds before writing to simulate background work...\n");
    sleep(10);

    if(sem_init(&(shm_ptr->mutex), 1, 1) == -1){
        perror("Semaphore initialization failed");
        return EXIT_FAILURE;
    }

    sem_wait(&(shm_ptr->mutex));

    strncpy(shm_ptr->shared_mry_arr, "Hii this is my own Shared mry msg!.. oh! oh!\n", SHM_DATA_SIZE);
    shm_ptr->data_ready = 1;

    sem_post(&(shm_ptr->mutex));
    printf("Timepass\n");
    sleep(10);
    munmap(shm_ptr, sizeof(struct Shared_mry_structure));
    close(shm_fd);

    return EXIT_SUCCESS;
}