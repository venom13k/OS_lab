#include<stdio.h>

#define SHM_SIZE 1024
#define BLOCK_SIZE 128

int main(int argc, char **argv) {

  FILE *fp;
  FILE *fw;
  int i;
  int nBytes;
  char f[SHM_SIZE];
  if(argc <= 1) {
    printf("usage: server /path/to/file/filename \n");
    return 1;
  }

  fp = fopen(argv[1], "r");
  if (fp==NULL) {
    perror("opening file error");
    return 5;
  }

  fw = fopen("love.txt", "w");
  while(!feof(fp)) {
    nBytes = fread(f, 1, BLOCK_SIZE, fp);
    fwrite(f, 1, nBytes, fw);    
    printf("%s\n", f);
  }


  
  
  fclose(fp);
  fclose(fw);


  return 0;
}
