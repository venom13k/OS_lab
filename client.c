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
  FILE* fw;
  int shmid;
  char* addr1;
  char* f1;
  key_t key;
  sem_t *mutex;

  key = 5678;

 if(argc <= 1) {
    printf("usage: server /path/to/file/filename \n");
    return 1;
  }

  //create & initialize existing semaphore
  mutex = sem_open(SEM_NAME,0,0644,0);
  if(mutex == SEM_FAILED)
    {
      perror("reader:unable to execute semaphore");
      sem_close(mutex);
      exit(-1);
    }

  shmid = shmget(key, SHM_SIZE, SHM_R|SHM_W); // get the id of the fresshly allocated segment
  if(shmid<0)
    {
      perror("reader:failure in shmget");
      exit(-1);
    }
  printf("shmid = %d",shmid);

  addr1 = shmat(shmid, NULL, 0); // get address where our segment has been mapped using id.
  if (addr1 == (char *)(-1)) // do a cast during the comparison to check for errors
        perror("shmat"); 

  printf("\nIPC SHARED MEMORY address is: %s",addr1);


  //  memcpy(f1, addr1, SHM_SIZE);
  fw = fopen("love.txt", "w");
  //  while(!feof(fp)) {
  f1 = addr1;
  sem_wait(mutex);
  fwrite(f1, 1, SHM_SIZE, fw);    
  sem_post(mutex);
  printf("%s\n", f1);
    //  }


  printf("\nThe message is: %s", f1);

  fclose(fw);

  return 0;

}
