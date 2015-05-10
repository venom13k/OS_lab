#include<sys/types.h>
#include<string.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<stdio.h>

#define SHM_SIZE 1024


int main(void)
{
  int shmid;
  char* addr1;
  char* f = "Good luck!/n";
  key_t key;

  key = 5678;

  shmid = shmget(key, SHM_SIZE, IPC_CREAT|SHM_R|SHM_W); // get the id of the fresshly allocated segment
  printf("shmid = %d",shmid);
  addr1 = shmat(shmid, NULL, 0); // get address where our segment has been mapped using id.

  if (addr1 == (char *)(-1)) // do a cast during the comparison to check for errors
        perror("shmat"); 

  printf("\nIPC SHARED MEMORY address is: %s",addr1);
  //copying your file at the shared location.
  memcpy(addr1, &f, SHM_SIZE);
  printf("\nMESSAGE STORED\n");
}
