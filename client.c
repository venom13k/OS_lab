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



int main(int argc, char **argv) {

  FILE* destination_file;
  int shm_id;
  char* shm_location;
  char  buf[SHM_SIZE];
  key_t key;
  sem_t *data_in_memory;
  sem_t *data_on_disk;

  key = 5678;
  
  if(argc <= 1) {
    printf("usage: client /path/to/file/filename \n");
    return 1;
    }
 
  //create & initialize existing semaphore #1
  data_in_memory = sem_open(SEM1_NAME,0,0644,0);
  if(data_in_memory == SEM_FAILED) {
    perror("reader1:unable to execute semaphore");
    sem_close(data_in_memory);
    exit(-1);
  }
  //create & initialize existing semaphore #1
  data_on_disk = sem_open(SEM2_NAME,0,0644,0);
  if(data_on_disk == SEM_FAILED) {
    perror("reader2:unable to execute semaphore");
    sem_close(data_on_disk);
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
  
  printf("\nIPC SHARED MEMORY address is: %p", shm_location);


  //  memcpy(f1, addr1, SHM_SIZE);
  destination_file = fopen(argv[1], "w");
  //  while(!feof(fp)) {
  //  f1 = addr1;
  sem_wait(data_in_memory);  
  //  memcpy(buf, shm_location, SHM_SIZE);    
  fwrite(shm_location, 1, SHM_SIZE, destination_file);
  printf("\nThe message is: %s\n", buf);

  *shm_location = '*';

  //  sem_post(data_on_disk);
  //  printf("%s\n", s);
    //  }
  



  //fclose(destination_file);

  return 0;

}
