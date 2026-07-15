#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
pthread_mutex_t lock ;
int shared_data ;
// Often shared data is more complex than just an int.
void *thread_function (void *arg )
{
int i ;
for ( i = 0 ; i < 1024 * 1024; ++i ) {
// Access the shared data here.
pthread_mutex_lock(&lock);
shared_data++;
pthread_mutex_unlock(&lock);
}
return NULL;
}
int main (void)
{
pthread_t thread_ID ;
void *exitstatus;
int i ;
// Initialize the mutex before trying to use it.
pthread_mutex_init(&lock , NULL) ;
pthread_create(&thread_ID , NULL, thread_function, NULL) ;
// Try to use the shared data.
for ( i = 0 ; i < 10 ; ++i ) {
sleep(1);
pthread_mutex_lock(&lock ) ;
printf( "\rShared integer 's value = %d\n" , shared_data ) ;
pthread_mutex_unlock(&lock);
}
printf("\n");
pthread_join(thread_ID, &exitstatus);
// Clean up the mutex when we are finished with it.
pthread_mutex_destroy(&lock);
return 0 ;
}