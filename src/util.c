#ifndef _UTIL_C_
#define _UTIL_C_

#include "util.h"


int resize(void **ptr,int size,int osize,int nsize){
	char *temp=calloc(size,nsize);
	if(temp==NULL)return 0;
	if(*ptr!=NULL){
		memcpy(temp,*ptr,size*osize);
		free(*ptr);
	}
	(*ptr)=temp;
	return 1;
}


int readline(FILE *fp,char **str,int *len){//read a single line from an input stream
	int size=256;//,length=0;
	char *line=calloc(size,sizeof(char));
	*len=0;
	for(int n=0;;++n,++(*len)){
		char c=fgetc(fp);
		if(n==0 && c==EOF){
			free(line);
			return 0;
		}
		if(c==EOF || c=='\n')break;
		if(n>=size-2){//time to reallocate
			resize((void**)&line,sizeof(char),size,size*2);
			size*=2;
		}
		line[n]=c;
	}
	line[*len]='\0';
	(*str)=line;
	return 1;
}


#endif
