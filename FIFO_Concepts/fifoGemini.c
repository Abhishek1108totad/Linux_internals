#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#define FIFO_FILE "/tmp/my_fifo"

int main() {
    int fd;
    char buffer[1024];
    
    // Clean up any old leftover instance of the FIFO
    unlink(FIFO_FILE);

    // 1. Create the FIFO
    if (mkfifo(FIFO_FILE, 0666) == -1) {
        perror("mkfifo failed");
        exit(1);
    }
    
    int c = fork();
    if (c < 0) {
        perror("fork failed");
        return 1;
    }

    if (c == 0) {
        // ==========================================
        // CHILD PROCESS: READER ONLY
        // ==========================================
        printf("Reader: Waiting for a writer to join...\n");

        // REMOVED O_NONBLOCK. Standard O_RDONLY blocks until the writer opens it.
        fd = open(FIFO_FILE, O_RDONLY);
        if (fd == -1) {
            perror("Reader open failed");
            exit(1);
        }

        printf("Reader: Writer connected. Reading data...\n");

        // Clear the buffer to be safe
        memset(buffer, 0, sizeof(buffer));

        // Read data from the FIFO and capture how many bytes actually came in
        ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0'; // Explicitly null-terminate
            printf("Reader: Received message: \"%s\"\n", buffer);
        } else {
            printf("Reader: No data received or read error.\n");
        }

        close(fd);
        exit(0); // CRITICAL: Stop the child here so it doesn't become a writer!

    } else {
        // ==========================================
        // PARENT PROCESS: WRITER ONLY
        // ==========================================
        printf("Writer: Waiting for a reader to join...\n");

        // This blocks until the reader opens the FIFO
        fd = open(FIFO_FILE, O_WRONLY);
        if (fd == -1) {
            perror("Writer open failed");
            return 1;
        }

        printf("Writer: Reader connected. Sending data...\n");

        char *message = "Hello from the Writer process!";
        write(fd, message, strlen(message) + 1);

        close(fd);
        printf("Writer: Data sent and connection closed.\n");

        // Wait for the child process to fully finish reading
        wait(NULL);
        
        // Clean up the pipe file now that everyone is done
        unlink(FIFO_FILE); 
    }

    return 0;
}