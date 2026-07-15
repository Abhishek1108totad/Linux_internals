#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdint.h>

int main() {
    int pipe1[2]; // Parent writes to pipe1, Child reads from pipe1
    int pipe2[2]; // Child writes to pipe2, Parent reads from pipe2
    
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("Pipe creation failed");
        return 1;
    }

    uint8_t count = 0;
    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        return 1;
    }

    printf("1\n");
    if (pid == 0) {
        printf("2\n");
        // --- CHILD PROCESS ---
        // Close unused ends
        // close(pipe1[1]); // Close write end of pipe1
        // close(pipe2[0]); // Close read end of pipe2

        while (count < 5) {
            printf("3\n");
            // 1. Wait for Parent to send the current count
            read(pipe1[0], &count, sizeof(count));
            printf("4\n");
            // printf("Hii");
            // if (count >= 10) break; // Exit condition Check

            printf("Child Process: %d\n", count);
            printf("5\n");
            count++;

            // 2. Send the updated count back to Parent
            write(pipe2[1], &count, sizeof(count));
            printf("5\n");
        }
        
        // close(pipe1[0]);
        // close(pipe2[1]);
        exit(0);

    } else {
        // --- PARENT PROCESS ---
        // Close unused ends
        // close(pipe1[0]); // Close read end of pipe1
        // close(pipe2[1]); // Close write end of pipe2

        // CRITICAL: Parent kicks off the game by writing the initial 0
        write(pipe1[1], &count, sizeof(count));
        printf("6\n");
        while (count < 5) {
            printf("7\n");
            // 1. Wait for Child to send the updated count
            read(pipe2[0], &count, sizeof(count));
            printf("8\n");
            //if (count >= 10) break;

            printf("Parent Process: %d\n", count);
            printf("9\n");
            count++;

            // 2. Send it back to Child
            write(pipe1[1], &count, sizeof(count));
            printf("10\n");
        }

        // Clean up and wait for child
        // close(pipe1[1]);
        // close(pipe2[0]);
        wait(NULL);
    }

    return 0;
}