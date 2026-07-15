#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define FIFO_FILE "/tmp/my_fifo"

int main() {
    int fd;
    char buffer[1024];

    printf("Reader: Waiting for a writer to join...\n");

    // 1. Open the FIFO for reading only (this blocks until a writer opens it)
    fd = open(FIFO_FILE, O_RDONLY | O_NONBLOCK);
    if (fd == -1) {
        perror("open failed");
        return 1;
    }

    printf("Reader: Writer connected. Reading data...\n");

    // 2. Read data from the FIFO
    read(fd, buffer, sizeof(buffer));
    printf("Reader: Received message: \"%s\"\n", buffer);

    // 3. Clean up
    close(fd);
    
    // Optional: Remove the FIFO file from the system when done
    unlink(FIFO_FILE); 

    return 0;
}