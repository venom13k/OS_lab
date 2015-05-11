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
char SEM1_NAME[]= "copy";
char SEM2_NAME[]= "disk";

//int main(int argc, char **argv)
int main()
{
  FILE *source_file;
  int shm_id;
  int nBytes;
  char* shm_location;
  char* buf = "Good luck!\n";
  key_t key;
  sem_t *data_in_memory;
  sem_t *data_on_disk;

  key = 5678;

  /*  if(argc <= 1) {
    printf("usage: server /path/to/file/filename \n");
    return 1;
    }*/
  
  //create & initialize semaphore #1
  data_in_memory = sem_open(SEM1_NAME, O_CREAT, 0644, 1);
  if(data_in_memory == SEM_FAILED)
    {
      perror("unable to create semaphore");
      sem_unlink(SEM1_NAME);
      exit(-1);
    }
  //create & initialize semaphore #2
  data_on_disk = sem_open(SEM2_NAME, O_CREAT, 0644, 1);
  if(data_on_disk == SEM_FAILED)
    {
      perror("unable to create semaphore");
      sem_unlink(SEM1_NAME);
      exit(-1);
    }
  
  //create the shared memory segment with this key
  shm_id = shmget(key, SHM_SIZE, IPC_CREAT|SHM_R|SHM_W); // get the id of the fresshly allocated segment
  if(shm_id < 0)
    {
      perror("failure in shmget");
      exit(-1);
    }
  printf("shmid = %d",shm_id);

  //attach this segment to virtual memory
  shm_location = shmat(shm_id, NULL, 0); // get address where our segment has been mapped using id
  if (shm_location == (char *)(-1)) // do a cast during the comparison to check for errors
        perror("shmat"); 

  printf("\nIPC SHARED MEMORY address is: %p \n", shm_location);

  //copying your file at the shared location.
  //   memcpy(addr1, &f, SHM_SIZE);
  /*
  source_file = fopen(argv[1], "r");
  if (source_file == NULL) {
    perror("opening file error");
    return 5;
  }
  */

  //start reading file
  //  f = addr1;
  //  while(!feof(fp)) {
  //    sem_wait(mutex);
  //    nBytes = fread(buf, 1, SHM_SIZE, source_file);
  memcpy(shm_location, buf, SHM_SIZE);
  sem_post(data_in_memory);
    //    }
  printf("\nMESSAGE STORED\n");

  //  sem_wait(data_on_disk);

  //the below loop could be replaced by binary semaphore
  while(*shm_location != '*')
    {
      sleep(1);
    }

  sem_close(data_in_memory);
  sem_close(data_on_disk);
  sem_unlink(SEM1_NAME);
  sem_unlink(SEM2_NAME);
  
  shmctl(shm_id, IPC_RMID, 0);

  /*
  fclose(source_file);
  exit(0);
  */



  return 0;
}
