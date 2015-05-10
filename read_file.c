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
    return 0;
  }

  fp = fopen(argv[1], "r");
  if (fp==NULL)
    perror("opening file error");
/*
  i = 0;

  while ((c = fgetc(fp)) != NULL  )  {                
    f[++i] = (char) c;     
    printf("%c", f[i]);
  } 
*/
  while(!feof(fp)) {
    nBytes = fread(f, 1, SHM_SIZE, fp);
    printf("%s\n", f);
  }


  fw = fopen("love.txt", "w");
  fwrite(f, 1, nBytes, fw);  
  fclose(fp);
  fclose(fw);


  return 0;
}
