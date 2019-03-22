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
  
  return (FILE*)f;
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

int myfseek(FILE *f,long ofs,int whence){
  FRESULT r;
  if(whence == SEEK_SET){
    r= f_lseek((FIL*)f,ofs);
  }else if(whence == SEEK_CUR){
    r= f_lseek((FIL*)f,ofs+f_tell((FIL*)f));
  }else if(whence == SEEK_END){
    r= f_lseek((FIL*)f,f_size((FIL*)f));
  }else{
    printf("error not implemented\n");
  }
  if(r)printf("fatfs seek err%d\n",r);
  return r;
}

long myftell(FILE *f){
  return f_tell((FIL*)f);
}

int mygetc(FILE *f){
  char c;
  int n;
  FRESULT r;
  r = f_read((FIL*)f,&c,1,&n);
  if(r==0&&n==1) return c;
  return EOF;
}
