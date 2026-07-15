#include <stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>
#include <string.h>

int main(){
    int pfds[2]= {0};
    char buff[10]= {'\0'};

    pipe(pfds);

    if(!fork()){
        printf("Child: writing to the pipe\n");
        write(pfds[1], "test", 5);
        printf("Child: Execuiting\n");
        exit(0);
    }else{
        printf("Parent: Reading from the pipe\n");
        read(pfds[0], buff, 5);
        printf("Parent: Read %s\n",buff);
        wait(NULL);
    }

    printf("Final out of fork\n");

    return 0;
}