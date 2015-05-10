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
  char* f1;
  key_t key;

  key = 5678;

  shmid = shmget(key, SHM_SIZE, SHM_R|SHM_W); // get the id of the fresshly allocated segment
  printf("shmid = %d",shmid);
  addr1 = shmat(shmid, NULL, 0); // get address where our segment has been mapped using id.
  printf("\nIPC SHARED MEMORY address is: %s",addr1);
  if (shmid == -1)
    printf("\nShared memory error");

  memcpy(f1, addr1, SHM_SIZE);
  printf("\nThe message is: %s", f1);

}
