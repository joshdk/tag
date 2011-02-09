#ifndef _TAG_H_
#define _TAG_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <libgen.h>
#include <dirent.h>
#include "dsv.h"

//#include <errno.h>

int lstat(const char *, struct stat *); 
char *strdup(const char *);


//int search_tagfile(char *,char *,char **,int);
//int match_tagrow(struct tagrow *,char **,int);


struct tagrow{
	char *name;
	char **tags;
	int len;
};


int make_tagrow(char **array,int alen,struct tagrow *row){
	if(alen<1)return 0;
	row->name=calloc(sizeof(char),strlen(array[0])+1);
	strcpy(row->name,array[0]);
	if(alen<2)return 0;
	row->tags=calloc(sizeof(char *),alen-1);
	row->len=0;
	for(int n=1;n<alen;++n){
		if(!strlen(array[n]))continue;//tag has zero lelgth
		row->tags[row->len]=calloc(sizeof(char),strlen(array[n])+1);
		strcpy(row->tags[row->len],array[n]);
		row->len++;
	}
	return 1;
}


int free_tagrow(struct tagrow *row){
	if(row==NULL)return 0;
	free(row->name);
	row->name=NULL;
	for(int n=0;n<row->len;++n){
		free(row->tags[n]);
		row->tags[n]=NULL;
	}
	free(row->tags);
	row->tags=NULL;
	return 1;
}


//int readline(FILE *,char **);




int get_path_and_name(const char *target,char **path,char **name){

	struct stat info;//={0};
	if(lstat(target,&info)!=0){
		return 0;
	}
//	printf("inode:[%d]\n",(int)info.st_ino);

	*path=NULL;
	*name=NULL;


//	printf("target:[%s]\n",target);


	if(S_ISDIR(info.st_mode)){
//		puts("dir");
		*path=calloc(1,strlen(target)+7);
		strcpy(*path,target);
		strcat(*path,"/.tags");
		*name=calloc(sizeof(char),2);
		strcat(*name,".");
		//			printf("[%s]\n",*path);
		//			free(*path);
	}else if(S_ISREG(info.st_mode) || S_ISCHR(info.st_mode) || S_ISBLK(info.st_mode) || S_ISLNK(info.st_mode)){
//		puts("file");
		char *temp=strdup(target);
		char *temp2=dirname(temp);
		*path=calloc(sizeof(char),strlen(temp2)+7);
		strcpy(*path,temp2);
		strcat(*path,"/.tags");
		free(temp);

		temp=strdup(target);
		temp2=basename(temp);
		*name=calloc(sizeof(char),strlen(temp2)+1);
		strcpy(*name,temp2);
		free(temp);
		//			printf("path:[%s/.tags] base:[%s]\n",path,base);
	}else{
//		puts("unknown");
		return 0;
	}

	return 1;
}














int full_negative(void *val,int size,int piece,void *tag){
	printf("full_negative(): comparing [%s] and [%s]\n",(char *)val,(char *)tag);
	if(!strcmp(val,tag)){
		return 1;
	}
	return 0;
}

int partial_positive(void *val,int size,int piece,void *tag){
	printf("partial_positive(): comparing [%s] and [%s]\n",(char *)val,(char *)tag);
	if(strstr(val,tag)){
		return 1;
	}
	return 0;
}

int partial_negative(void *val,int size,int piece,void *tag){
	printf("partial_negative(): comparing [%s] and [%s]\n",(char *)val,(char *)tag);
	if(!strstr(val,tag)){
		return 1;
	}
	return 0;
}


int filter_blank_and_null(void *val,int size,int piece,void *un1){
	char *str=(char *)val;
	printf("filter_blank_and_null(%s)\n",str);
	if(str==NULL || !strlen(str)){
		return 1;
	}
	return 0;
}



int filter_array(void ***array,int size,int *count,void *val,int (*match)(void *,int,int,void *)){
	int rlen=0,rsize=*count;
	void **result=calloc(size,rsize);
	for(int n=0;n<rsize;++n){
		if(!(*match)(   *((*array)+n),rsize,n,val    )    ){
			result[rlen++]=*((*array)+n);
		}else{
			free(*((*array)+n));
		}
	}
	
//	for(int n=0;n<rlen;++n){
//		printf("new array: [%s]\n",*(result+n));
//	}

	free(*array);
	*array=result;
	*count=rlen;

	return 0;
}




int query_tagfile(FILE *stream,const char *path,const char *name){
	FILE *tagf=NULL;
	if((tagf=fopen(path,"r"))==NULL){
		puts("no such tagfile...");
		return 1;
	}

	int len=0;
	char *line=NULL;

	while(readline(tagf,&line,&len)){
		if(len){
			//			printf("[%s](%d)\n",line,len);
			int count;
			char **array=NULL;
			split(line,&array,&count);
			if(count>=2 && !strcmp(name,array[0])){
//				filter_array((void ***)&array,sizeof(char *),&count,&filter_blank_and_null);
				struct tagrow row;
				make_tagrow(array,count,&row);
				for(int n=0;n<row.len;++n){
					if(strlen(row.tags[n])){
//						puts(array[n]);
						fprintf(stream,"%s\n",row.tags[n]);
					}
				}
				free_tagrow(&row);
			}

			for(int n=0;n<count;++n){
				free(array[n]);
			}
			free(array);
		}
		free(line);
	}

	return 0;
}





int tag_tagfile(FILE *stream,char *path,char *name,char **tags,int tagc){

	int llen=0,lsize=16;
	struct tagrow *lines=calloc(sizeof(struct tagrow),lsize);
	int offset=0;
	int found=0;

	FILE *tagf=NULL;
	if((tagf=fopen(path,"r"))!=NULL){
		
		int len=0;
		char *line=NULL;

		while(readline(tagf,&line,&len)){
			if(len){
				int count;
				char **array=NULL;
				split(line,&array,&count);
				if(count>=2 && strlen(array[0])){//if there is data in the line, add to th list
					make_tagrow(array,count,&lines[llen]);
					if(!strcmp(lines[llen].name,name)){//found the row we're looking for
						offset=llen;
						found=1;
					}
					if(llen>=lsize){
						resize((void **)&lines,sizeof(struct tagrow),lsize,lsize*2);
					}
					lsize*=2;
					llen++;
				}
				for(int n=0;n<count;++n){
					free(array[n]);
				}
				free(array);
				
			}
			free(line);
		}
		fclose(tagf);
	}else{
		printf("couldn\'t open .tags for reading...\n");
	}


	if(!found){//out file isn't currently in the file list...
		printf("we didn't find the file...");
		if(llen>=lsize){
			resize((void **)&lines,sizeof(struct tagrow),lsize,lsize+1);
		}
		lsize+=1;
		lines[llen].name=calloc(sizeof(char),strlen(name)+1);
		strcpy(lines[llen].name,name);
		lines[llen].len=0;
		offset=llen;
		llen++;
	}


//	puts("here");


	int trip=0;

	for(int n=0;n<tagc;++n){
		trip=0;
		if(tags[n]==NULL || !strlen(tags[n])){continue;}
		switch(tags[n][0]){
			case '-':
				printf("removing tag [%s] from row [%s]\n",tags[n],lines[offset].name);
				filter_array((void ***)&lines[offset].tags,sizeof(char *),&lines[offset].len,(void *)tags[n]+1,&full_negative);
				break;

			case ':':
				printf("removing tag [%s] from row [%s]\n",tags[n],lines[offset].name);
				filter_array((void ***)&lines[offset].tags,sizeof(char *),&lines[offset].len,(void *)tags[n]+1,&partial_positive);
				break;

			case '.':
				printf("removing tag [%s] from row [%s]\n",tags[n],lines[offset].name);
				filter_array((void ***)&lines[offset].tags,sizeof(char *),&lines[offset].len,(void *)tags[n]+1,&partial_negative);
				break;

			case '+':
				for(int m=0;m<lines[offset].len;++m){
					if(!strcmp(lines[offset].tags[m],tags[n]+1)){
						printf("can't add duplicate +tag...\n");
						trip=1;
						break;
					}
				}
				if(!trip){
					resize((void**)&(lines[offset].tags),sizeof(char**),lines[offset].len,lines[offset].len+1);
					lines[offset].len++;
					lines[offset].tags[lines[offset].len-1]=calloc(sizeof(char),strlen(tags[n]+1)+1);
					strcpy(lines[offset].tags[lines[offset].len-1],tags[n]+1);
				}
				break;
			default:
				for(int m=0;m<lines[offset].len;++m){
					if(!strcmp(lines[offset].tags[m],tags[n])){
						printf("can't add duplicate tag...\n");
						trip=1;
						break;
					}
				}
				if(!trip){
					resize((void**)&(lines[offset].tags),sizeof(char**),lines[offset].len,lines[offset].len+1);
					lines[offset].len++;
					lines[offset].tags[lines[offset].len-1]=calloc(sizeof(char),strlen(tags[n])+1);
					strcpy(lines[offset].tags[lines[offset].len-1],tags[n]);
				}
				break;
		}
	}



	if((tagf=fopen(path,"w"))!=NULL){
		printf("writing to .tags\n");
		for(int n=0;n<llen;++n){
			if(lines[n].len>=1){
				fprintf(tagf,"%s",lines[n].name);
				for(int m=0;m<lines[n].len;++m){
					fprintf(tagf,":");
					for(unsigned int l=0;l<strlen(lines[n].tags[m]);++l){
						switch(lines[n].tags[m][l]){
							case '\\':
							case ':':
								fprintf(tagf,"\\");
							default:
								fprintf(tagf,"%c",lines[n].tags[m][l]);
								break;
						}
					}
				}
				fprintf(tagf,"\n");
			}
		}
		fclose(tagf);
	}else{
		printf("couldn\'t open .tags for writing...\n");
	}


//	printf("data summary:\n");
	for(int n=0;n<llen;++n){
//		printf("[%s]:\n",lines[n].name);
		for(int m=0;m<lines[n].len;++m){
			fprintf(stream,"%s\n",lines[n].tags[m]);
		}
	}

	for(int n=0;n<llen;++n){
		for(int m=0;m<lines[n].len;++m){
			free(lines[n].tags[m]);
		}
		free(lines[n].tags);
		free(lines[n].name);
	}
	free(lines);


	return 0;
}





int match_tagrow(struct tagrow *row,char **tags,int tagc){

	int pass=0;
	int valid=0;
//	printf("scanning file [%s]\n",row->name);
//	printf("tagc count: [%d]\n",tagc);

	for(int n=0;n<tagc;++n){
		switch(tags[n][0]){//switch on modifier
			case '+':
				pass=0;
				for(int m=0;m<row->len;++m){
					if(!strcmp(tags[n]+1,row->tags[m])){//found one
						pass=1;
						valid=1;
						break;
					}
				}
				if(!pass)return 0;//failed on required(+) tag
				break;

			case '-':
				for(int m=0;m<row->len;++m){
					if(!strcmp(tags[n]+1,row->tags[m])){//found one
						return 0;//failed on banned(-) tag
					}
				}
				break;

			case '~':
				for(int m=0;m<row->len;++m){
					if(!strcmp(tags[n]+1,row->tags[m])){//found one
						valid=1;//this row is marked as valid(+)
						break;
					}
				}
				
				break;

			case ':':
				for(int m=0;m<row->len;++m){
					if(strstr(row->tags[m],tags[n]+1)){//found one
						valid=1;//this row is marked as valid(:)
						break;
					}
				}
				
				break;

			case '.':
				for(int m=0;m<row->len;++m){
					if(strstr(row->tags[m],tags[n]+1)){//found one
						return 0;//failed on banned(.)
					}
				}
				
				break;

			default:
				for(int m=0;m<row->len;++m){
					if(!strcmp(tags[n],row->tags[m])){//found one
						valid=1;
						break;
					}
				}
				
				break;
		}
	}

//	printf("%s/%s\n",dir,row->name);
	return valid;
}









int search_tagfile(char *path,char *dir,char **tags,int tagc){

	int llen=0,lsize=16;
//	struct tagrow *lines=calloc(sizeof(struct tagrow),lsize);
	int offset=0;
	int found=0;

	FILE *tagf=NULL;
	if((tagf=fopen(path,"r"))!=NULL){
//		printf("opening %s for reading...\n",path);
		
		int len=0;
		char *line=NULL;

		while(readline(tagf,&line,&len)){
			if(len){
				int count;
				char **array=NULL;
				split(line,&array,&count);
				if(count>=2 && strlen(array[0])){//if there is data in the line, add to th list
					struct tagrow row;
					make_tagrow(array,count,&row);
					struct stat info;//={0};
					char *full=calloc(sizeof(char),strlen(dir)+strlen(row.name)+1);
					strcat(full,dir);
					strcat(full,row.name);
					if(lstat(full,&info)==0){
						if(S_ISDIR(info.st_mode)){
							if(match_tagrow(&row,tags,tagc)){
								printf("%s\n",dir);
							}

						}else{
							if(match_tagrow(&row,tags,tagc)){
								printf("%s%s\n",dir,row.name);
							}

						}
					}
					free(full);
					free_tagrow(&row);
				}
				for(int n=0;n<count;++n){
					free(array[n]);
				}
				free(array);
				
			}
			free(line);
		}
		fclose(tagf);
	}else{
//		printf("couldn\'t open %s for reading...\n",path);
	}


	return 0;
}







void search(char *path,char **tags,int tagc){

	struct dirent *dp;
	struct stat s;
	DIR *dir;

	if((dir=opendir(path))==NULL){return;}

	while((dp=readdir(dir))!=NULL){

		if(!strcmp(dp->d_name,".") || !strcmp(dp->d_name,".."))continue;

		char *temp=malloc((strlen(path)+strlen(dp->d_name)+4)*sizeof(*temp));
		sprintf(temp,"%s%s",path,dp->d_name);

		lstat(temp,&s);
//		printf("[%s]\n",temp);

		if(S_ISREG(s.st_mode)){//if it's regular file
			if(!strcmp(dp->d_name,".tags")){
//				printf("^-- found one\n");
//				FILE *ftags;
//				if((ftags=fopen(temp,"rb"))!=NULL){search_tagfile(path,tags,tagc,ftags);}
//				fclose(ftags);
				search_tagfile(temp,path,tags,tagc);

			}
		}else if(S_ISDIR(s.st_mode) && !S_ISLNK(s.st_mode)){//if it's a directory
				strcat(temp,"/");
				search(temp,tags,tagc);
		}

		free(temp);


	}
	closedir(dir);


}






int find_tagfile(char *dir,char **tags,int tagc){
	struct stat info;//={0};
	if(lstat(dir,&info)!=0){
		return 0;
	}

	if(!S_ISDIR(info.st_mode)){
		return 0;
	}

	char *path=calloc(sizeof(char),strlen(dir)+8);
	strcpy(path,dir);
	if(path[strlen(path)-1]!='/'){
		strcat(path,"/");
	}
//	printf("starting path: [%s]\n",path);
	search(path,tags,tagc);
	free(path);


	return 1;
}














#endif
