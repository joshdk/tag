#ifndef _DSV_C_
#define _DSV_C_

#include "dsv.h"


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
			}else if(c==delim){//found a field seperator
				alen++;
				if(alen>=asize){
					char **temp=NULL;
					if((temp=realloc(*array,sizeof(char *)*asize*2))!=NULL){
						*array=temp;
						temp=NULL;
					}
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
				char *temp=NULL;
				if((temp=realloc((*array)[alen],sizeof(char)*ssize*2))!=NULL){
					(*array)[alen]=temp;
					temp=NULL;
				}
				ssize*=2;
			}
			(*array)[alen][slen]=c;
			slen++;
		}
	}
	*count=alen+1;

	return 0;
}


#endif
