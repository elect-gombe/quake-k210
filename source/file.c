#include "ff.h"
#include <stdio.h>
#include <stdlib.h>

FILE *myfopen(const char *filename,const char *atr){
  FIL *f = malloc(sizeof(FIL));
  if(f==NULL)return 0;

  FRESULT r=f_open(f,filename,FA_READ);
  if(r){
    if(r)printf("fatfs open err%d\n",r);
    free(f);
    return 0;
  }
  
  return f;
}

size_t myfread(void *ptr,size_t s,size_t nm,FILE *f){
  int n;
  FRESULT r;
  r = f_read((FIL*)f,ptr,s*nm,&n);
  if(r)printf("fatfs read err%d\n",r);
  return n/s;
}

int myfclose(FILE *f){
  f_close((FIL*)f);
  free(f);
  return 0;
}

int myfseek(FIL *f,long ofs,int whence){
  FRESULT r;
  if(whence == SEEK_SET){
    r= f_lseek(f,ofs);
  }else if(whence == SEEK_CUR){
    r= f_lseek(f,ofs+f_tell(f));
  }else if(whence == SEEK_END){
    r= f_lseek(f,f_size(f));
  }else{
    printf("error not implemented\n");
  }
  if(r)printf("fatfs seek err%d\n",r);
  return r;
}

long myftell(FIL *f){
  return f_tell(f);
}

int mygetc(FIL *f){
  char c;
  int n;
  FRESULT r;
  r = f_read(f,&c,1,&n);
  if(r==0&&n==1) return c;
  return EOF;
}
