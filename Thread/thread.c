#include <stdio.h>
#include <pthread.h>

void *print_msg(void* arg){
    printf("Thread message : %s\n",(char*)arg);
    return NULL;
}

int main(){
    pthread_t thread1, thread2;

    char *msg1 = "I'm thread1, I'm here to write the things\n";
    char *msg2 = "I'm thread2, I'm here to read the things\n";

    int pthrd1 = pthread_create(&thread1, NULL, print_msg, (void*)msg1);
    int pthrd2 = pthread_create(&thread2, NULL, print_msg, (void*)msg2);

    printf("%d\n",pthrd1);
    printf("%d\n",pthrd2);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("Main thread: All threads finished.\n");
    return 0;
}