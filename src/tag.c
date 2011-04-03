#ifndef _TAG_C_
#define _TAG_C_

#include "dsv.h"
#include "tag.h"


int make_tagrow(char **array,int alen,struct tagrow *row){
	if(alen<1)return 0;
//	row->name=NULL;
	row->name=calloc(sizeof(char),strlen(array[0])+1);
	strcpy(row->name,array[0]);
	if(alen<2)return 0;
	row->tags=calloc(sizeof(char *),alen-1);
	row->len=0;
	for(int n=1;n<alen;++n){
		if(array[n]!=NULL && !strlen(array[n]))continue;//tag has zero lelgth
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



int get_path_and_name(const char *target,char **path,char **name){

	struct stat info;//={0};
	if(lstat(target,&info)!=0){
		return 0;
	}

	*path=NULL;
	*name=NULL;

	if(S_ISDIR(info.st_mode)){
		*path=calloc(1,strlen(target)+7);
		strcpy(*path,target);
//		strcat(*path,"/");
		*name=calloc(sizeof(char),2);
		strcat(*name,".");
	}else if(S_ISREG(info.st_mode) || S_ISCHR(info.st_mode) || S_ISBLK(info.st_mode) || S_ISLNK(info.st_mode)){
		char *temp=strdup(target);
		char *temp2=dirname(temp);
		*path=calloc(sizeof(char),strlen(temp2)+7);
		strcpy(*path,temp2);
		strcat(*path,"/");
		free(temp);

		temp=strdup(target);
		temp2=basename(temp);
		*name=calloc(sizeof(char),strlen(temp2)+1);
		strcpy(*name,temp2);
		free(temp);
	}else{
//		puts("unknown");
		return 0;
	}

	return 1;
}



int modify_tagrow(struct tagrow *row,char *tag,int action){

	debugf("running against [%s] with [%d]\n",tag,action);
	debugf("tag count [%d]\n",row->len);

	if(action==1){
		int trip=0;
		for(int n=0;n<row->len;++n){
			if(!strcmp(row->tags[n],tag)){
				debugf("duplicate tag +[%s]\n",row->tags[n]);
				trip=1;
				break;
			}
		}
		if(trip)return 0;//duplicate tag. no point in adding it again...
//		resize((void**)&(row->tags),sizeof(char**),row->len,row->len+1);
		char **temp=NULL;
		if((temp=realloc(row->tags,sizeof(char **)*(row->len+1)))!=NULL){
			row->tags=temp;
			temp=NULL;
		}
		row->len++;
		row->tags[row->len-1]=calloc(sizeof(char),strlen(tag)+1);
		strcpy(row->tags[row->len-1],tag);
		debugf("adding tag [%s]\n",tag);
	}else if(action==2 || action==3 || action==4){
		for(int n=0;n<row->len;++n){
			int remove=0;
			switch(action){
				case 2:
					if(!strcmp(row->tags[n],tag)){
						debugf("removing tag -[%s]\n",row->tags[n]);
						remove=1;
					}
					break;
				case 3:
					if(strstr(row->tags[n],tag)){
						debugf("removing tag :[%s]\n",row->tags[n]);
						remove=1;
					}
					break;
				case 4:
					if(!strstr(row->tags[n],tag)){
						debugf("removing tag .[%s]\n",row->tags[n]);
						remove=1;
					}
					break;
			}
			if(!remove)continue;
			free(row->tags[n]);
			for(int m=n+1;m<row->len;++m){
				row->tags[m-1]=row->tags[m];
			}
			row->len--;
			n--;
		}
	}
	return 0;
}




int query_tagfile(FILE *stream,const char *path,const char *name){
	FILE *tagf=NULL;
	if((tagf=fopen(path,"r"))==NULL){
		debugf("no such tagfile...");
		return 1;
	}

	int len=0;
	char *line=NULL;

	while(readline(tagf,&line,&len)){
		if(len){
			//			printf("[%s](%d)\n",line,len);
			int count;
			char **array=NULL;
			split(line,':',&array,&count);
			if(count>=2 && !strcmp(name,array[0])){
				struct tagrow row;
				make_tagrow(array,count,&row);
				for(int n=0;n<row.len;++n){
					if(strlen(row.tags[n])){
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

	int llen=0,lsize=1;
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
				split(line,':',&array,&count);
				if(count>=2 && strlen(array[0])){//if there is data in the line, add to the list
					if(llen>=lsize){
//						resize((void **)&lines,sizeof(struct tagrow),lsize,lsize*2);
						struct tagrow *temp=NULL;
						if((temp=realloc(lines,sizeof(struct tagrow)*(lsize*2)))!=NULL){
							lines=temp;
							temp=NULL;
						}
						lsize*=2;
					}
					make_tagrow(array,count,&lines[llen]);
					if(!strcmp(lines[llen].name,name)){//found the row we're looking for
						offset=llen;
						found=1;
					}
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
		debugf("couldn\'t open .tags for reading...\n");
	}


	if(!found){//out file isn't currently in the file list...
		debugf("the file wasn't listed in the .tags file\n");
		if(llen>=lsize){
//			resize((void **)&lines,sizeof(struct tagrow),lsize,lsize+1);
			struct tagrow *temp=NULL;
			if((temp=realloc(lines,sizeof(struct tagrow)*(lsize+1)))!=NULL){
				lines=temp;
				temp=NULL;
			}
			lsize+=1;
		}
		lines[llen].name=calloc(sizeof(char),strlen(name)+1);
		strcpy(lines[llen].name,name);
		lines[llen].len=0;
		lines[llen].tags=NULL;
		offset=llen;
		llen++;
	}


	int trip=0;

	for(int n=0;n<tagc;++n){
		trip=0;
		if(tags[n]==NULL || !strlen(tags[n])){continue;}
		switch(tags[n][0]){
			case '-':
				modify_tagrow(&lines[offset],tags[n]+1,2);
				break;

			case ':':
				modify_tagrow(&lines[offset],tags[n]+1,3);
				break;

			case '.':
				modify_tagrow(&lines[offset],tags[n]+1,4);
				break;

			case '+':
				modify_tagrow(&lines[offset],tags[n]+1,1);
				break;

			default:
				modify_tagrow(&lines[offset],tags[n],1);
				break;
		}
	}



	if((tagf=fopen(path,"w"))!=NULL){
		debugf("writing to .tags\n");
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
		debugf("couldn\'t open .tags for writing...\n");
	}


//	for(int n=0;n<llen;++n){
//		printf("[%s]:\n",lines[n].name);
//		for(int m=0;m<lines[n].len;++m){
//			fprintf(stream,"\t%s\n",lines[n].tags[m]);
//		}
//	}

	for(int m=0;m<lines[offset].len;++m){
		fprintf(stream,"%s\n",lines[offset].tags[m]);
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









int search_tagfile(char *path,char **tags,int tagc){

//TODO: extract directory name from path

char *dir=NULL,*name=NULL;
if(!get_path_and_name(path,&dir,&name)){
	return 0;
}

//printf("search: [%s] [%s]\n",dir,name);

//	int llen=0,lsize=16;
//	struct tagrow *lines=calloc(sizeof(struct tagrow),lsize);
//	int offset=0;
//	int found=0;

	FILE *tagf=NULL;
	if((tagf=fopen(path,"r"))!=NULL){
//		printf("opening %s for reading...\n",path);
		
		int len=0;
		char *line=NULL;

		while(readline(tagf,&line,&len)){
			if(len){
				int count;
				char **array=NULL;
				split(line,':',&array,&count);
				if(count>=2 && strlen(array[0])){//if there is data in the line, add to th list
					struct tagrow row={NULL,NULL,0};
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
	free(dir);dir=NULL;
	free(name);name=NULL;

	return 0;
}







int search(char **path,int len,int *size,char **tags,int tagc){
//	printf("inside: [%s]\n",*path);

	struct dirent *dp;
	struct stat s;
	DIR *dir;
	int count=0;

	if((dir=opendir(*path))==NULL){return 0;}

	while((dp=readdir(dir))!=NULL){

		if(!strcmp(dp->d_name,".") || !strcmp(dp->d_name,".."))continue;

		if((*size)<=(int)(len+strlen(dp->d_name))){
//			resize((void **)&(*path),sizeof(char),*size,(len)+strlen(dp->d_name)+4);
			char *temp=NULL;
			if((temp=realloc(*path,sizeof(char)*(len+strlen(dp->d_name)+4)))!=NULL){
				*path=temp;
				temp=NULL;
			}
			(*size)=len+strlen(dp->d_name)+4;
		}
		strcat(*path,dp->d_name);
		(*path)[len+strlen(dp->d_name)]='\0';
		lstat(*path,&s);
		if(S_ISREG(s.st_mode)){//if it's regular file
			if(!strcmp(dp->d_name,".tags")){
				search_tagfile(*path,tags,tagc);
				count++;
			}
		}else if(S_ISDIR(s.st_mode) && !S_ISLNK(s.st_mode)){//if it's a directory
			strcat(*path,"/");
			(*path)[len+strlen(dp->d_name)+1]='\0';
			count+=search(path,len+strlen(dp->d_name)+1,size,tags,tagc);
		}

		(*path)[len]='\0';

	}

	closedir(dir);
	return count;
}






int find_tagfile(char *dir,char **tags,int tagc){
	struct stat info;
	if(lstat(dir,&info)!=0)return 0;
	if(!S_ISDIR(info.st_mode))return 0;

	int len=strlen(dir),size=len+1024;
	char *path=calloc(sizeof(char),size);
	strcpy(path,dir);
	if(path[len-1]!='/'){
		strcat(path,"/");
		len++;
	}
	search(&path,len,&size,tags,tagc);
	free(path);

	return 1;
}





#endif
