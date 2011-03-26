#ifndef _DSV_C_
#define _DSV_C_

#include "dsv.h"


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



int split(char *line,char delim,char ***array,int *count){
	int alen=0,asize=1;
	(*array)=malloc(asize*sizeof(char *));

	int slen=0,ssize=32;
	(*array)[alen]=calloc(ssize,sizeof(char));

	int mode=0;
	int wb=0;
	char c='\0';
	char *ptr=line;

	while((c=*(ptr++))!='\0'){
		wb=0;

		if(mode==0){//normal character mode
			if(c=='\\'){
				mode=1;
			}else if(c==delim){
				alen++;
				if(alen>=asize){
					resize((void**)&(*array),sizeof(char*),asize,asize*2);
					asize*=2;
				}
				slen=0;
				ssize=1;
				(*array)[alen]=calloc(ssize,sizeof(char));
			}else{
				wb=1;
			}

		}else if(mode==1){//escape character mode
			if(c==delim){
				wb=1;
			}else if(c=='\\'){
				wb=1;
			}
			mode=0;
		}

		if(wb==1){//writeback a character
			wb=0;
			if(slen>=ssize-1){
				resize((void**)&(*array)[alen],sizeof(char),ssize,ssize*2);
				ssize*=2;
			}
			(*array)[alen][slen]=c;
			slen++;
		}
	}
	*count=alen+1;

	return 0;
}


int readline(FILE *fp,char **str,int *len){//read a single line fron an input stream
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
