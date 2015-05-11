#include<sys/types.h>
#include<string.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<semaphore.h>

#define SHM_SIZE 1024
char SEM_NAME[]= "vik";



int main(int argc, char **argv)
{
  FILE* destination_file;
  int shm_id;
  char* shm_location;
  char* f1;
  key_t key;
  sem_t *mutex;

  key = 5678;

 if(argc <= 1) {
    printf("usage: client /path/to/file/filename \n");
    return 1;
  }

  //create & initialize existing semaphore
  mutex = sem_open(SEM_NAME,0,0644,0);
  if(mutex == SEM_FAILED) {
    perror("reader:unable to execute semaphore");
    sem_close(mutex);
    exit(-1);
  }

  shm_id = shmget(key, SHM_SIZE, SHM_R|SHM_W); // get the id of the fresshly allocated segment
  if(shm_id < 0) {
    perror("reader:failure in shmget");
    exit(-1);
  }
  printf("shmid = %d",shm_id);

  shm_location = shmat(shm_id, NULL, 0); // get address where our segment has been mapped using id.
  if (shm_location == (char *)(-1)) // do a cast during the comparison to check for errors
    perror("shmat"); 
  
  printf("\nIPC SHARED MEMORY address is: %s", shm_location);


  //  memcpy(f1, addr1, SHM_SIZE);
  destination_file = fopen(argv[1], "w");
  //  while(!feof(fp)) {
  //  f1 = addr1;
  sem_wait(mutex);
  fwrite(shm_location, 1, SHM_SIZE, destination_file);    
  sem_post(mutex);
  // printf("%s\n", s);
    //  }


  // printf("\nThe message is: %s",);

  fclose(destination_file);

  return 0;

}
