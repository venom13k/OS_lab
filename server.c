#include<sys/types.h>
#include<string.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<semaphore.h>

#define SHM_SIZE 128
char SEM_NAME[]= "vik";

int main(int argc, char **argv)
{
  FILE *fp;
  int shmid;
  int nBytes;
  char* addr1;
  char* f;
  key_t key;
  sem_t *mutex;

  key = 5678;

  if(argc <= 1) {
    printf("usage: server /path/to/file/filename \n");
    return 1;
  }

  //create & initialize semaphore
  mutex = sem_open(SEM_NAME, O_CREAT, 0644, 1);
  if(mutex == SEM_FAILED)
    {
      perror("unable to create semaphore");
      sem_unlink(SEM_NAME);
      exit(-1);
    }
  
  //create the shared memory segment with this key
  shmid = shmget(key, SHM_SIZE, IPC_CREAT|SHM_R|SHM_W); // get the id of the fresshly allocated segment
  if(shmid<0)
    {
      perror("failure in shmget");
      exit(-1);
    }
  printf("shmid = %d",shmid);
  //attach this segment to virtual memory
  addr1 = shmat(shmid, NULL, 0); // get address where our segment has been mapped using id
  if (addr1 == (char *)(-1)) // do a cast during the comparison to check for errors
        perror("shmat"); 

  printf("\nIPC SHARED MEMORY address is: %s \n",addr1);

  //copying your file at the shared location.
  //   memcpy(addr1, &f, SHM_SIZE);

  fp = fopen(argv[1], "r");
  if (fp==NULL) {
    perror("opening file error");
    return 5;
  }


  //start reading file
  f = addr1;
  //  while(!feof(fp)) {
    sem_wait(mutex);
    nBytes = fread(f, 1, SHM_SIZE, fp);
    //    *f = *f + SHM_SIZE;
    sem_post(mutex);
    //    }
 

  fclose(fp);
  sem_close(mutex);
  sem_unlink(SEM_NAME);
  shmctl(shmid, IPC_RMID, 0);
  exit(0);

  printf("\nMESSAGE STORED\n");


  return 0;
}
