#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#define FIFO_FILE "/tmp/my_fifo"

char buffer[1024];

int main() {
    int fd;
    unlink(FIFO_FILE);

    // 1. Create the FIFO if it doesn't already exist
    // 0666 gives read/write permissions to user, group, and others
    if (mkfifo(FIFO_FILE, 0666) == -1) {
        perror("mkfifo failed (it might already exist)");
        exit(1);
    }
    int c = fork();
    if (c == 0) {
        printf("Reader: Waiting for a writer to join...\n");

        // 1. Open the FIFO for reading only (this blocks until a writer opens it)
        fd = open(FIFO_FILE, O_RDONLY);
        if (fd == -1) {
            perror("open failed");
            return 1;
        }

        printf("Reader: Writer connected. Reading data...\n");

        // // 2. Read data from the FIFO
        // read(fd, buffer, sizeof(buffer));
        // printf("Reader: Received message: \"%s\"\n", buffer);
        // Read data from the FIFO and capture how many bytes actually came in
        ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0'; // Explicitly null-terminate
            printf("Reader: Received message: \"%s\"\n", buffer);
        } else {
            printf("Reader: No data received or read error.\n");
        }

        // 3. Clean up
        close(fd);
        exit(0);
          
    } else {
 
        printf("Writer: Waiting for a reader to join...\n");

        // 2. Open the FIFO for writing only (this blocks until a reader opens it)
        fd = open(FIFO_FILE, O_WRONLY);
        if (fd == -1) {
            perror("open failed");
            return 1;
        }

        printf("Writer: Reader connected. Sending data...\n");

        // 3. Write data to the FIFO
        char *message = "Hello from the Writer process!";
        write(fd, message, strlen(message) + 1);
        
        // 4. Clean up
        close(fd);
        printf("Writer: Data sent and connection closed.\n");

        wait(NULL);
        
        unlink(FIFO_FILE);
    }
    return 0;
}