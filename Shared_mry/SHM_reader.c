#include "Shared_mry.h"

int main(){
    /*Creating the shared mry in virtual mry*/
    int shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);

    if(shm_fd == -1){
        perror("SHM open failed");
        return EXIT_FAILURE;
    }
    
    if(ftruncate(shm_fd, sizeof(struct Shared_mry_structure)) == -1){
        perror("ftruncate is failed");
        return EXIT_FAILURE;
    }

    struct Shared_mry_structure *shm_ptr = (struct Shared_mry_structure *) mmap(NULL, sizeof(struct Shared_mry_structure), PROT_READ | PROT_WRITE,MAP_SHARED, shm_fd, 0);

    if(shm_ptr == MAP_FAILED){
        perror("MApping failed");
        return EXIT_FAILURE;
    }

    if(sem_init(&(shm_ptr->mutex), 1, 1) == -1){
        perror("Semaphore initialization failed");
        return EXIT_FAILURE;
    }

    while(1){
        sem_wait(&(shm_ptr->mutex));
        if(shm_ptr->data_ready == 1){
            
            printf("%s", shm_ptr->shared_mry_arr);
            sem_post(&(shm_ptr->mutex));
            break;
        }
         sem_post(&(shm_ptr->mutex));
         //usleep(100000); // Sleep for 100ms to avoid burning CPU cycles
    }
    // 5. Complete System Cleanup
    // Destroy the semaphore
    sem_destroy(&(shm_ptr->mutex));
    munmap(shm_ptr, sizeof(struct Shared_mry_structure));
    close(shm_fd);

    shm_unlink(SHM_NAME);
    printf("[READER] Shared memory unlinked from kernel space cleanly.\n");

    return EXIT_SUCCESS;
}