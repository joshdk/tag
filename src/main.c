#ifndef _TAG_C_
#define _TAG_C_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#define DEBUG 1
//#include "debug.h"
#include "tag.h"
#include "res.h"
#include "dsv.h"


int main(int argc,char **argv){

	if(argc<2){
		fprintf(stderr,"tag: mising operand\n");
		fprintf(stderr,"Try `tag --help' for more information.\n");
		return 1;
	}

	if(!strcmp(argv[1],"--help")){
		printf(help_text);
		return 0;
	}

	if(!strcmp(argv[1],"--version")){
		printf(version_text);
		return 0;
	}
	
	/*missing argument*/
	if(argc<3){
		fprintf(stderr,"tag: mising operand\n");
		fprintf(stderr,"Try `tag --help' for more information.\n");
		return 1;
	}

	/*file tagging mode*/
	if(!strcmp(argv[1],"-t")){
		char *path=NULL;
		char *name=NULL;
		if(get_path_and_name(argv[2],&path,&name)){
			char *temp=NULL;
			if((temp=realloc(path,sizeof(char)*strlen(path)+8))!=NULL){//reallocate, making room for the ".tags" concat
				path=temp;
				temp=NULL;
			}
			strcat(path,".tags");

			tag_tagfile(stdout,path,name,argv+3,argc-3);
			free(path);
			free(name);
		}else{
			fprintf(stderr,"tag: `%s\' no such file or directory\n",argv[argc-1]);
			free(path);
			free(name);
			return 1;
		}

	/*file searching mode*/
	}else if(!strcmp(argv[1],"-f")){
		if(!find_tagfile(argv[2],argv+3,argc-3)){
			fprintf(stderr,"tag: `%s\' no such directory\n",argv[2]);
			return 1;
		}

	/*file querying mode*/
	}else if(!strcmp(argv[1],"-q")){
		char *path=NULL;
		char *name=NULL;
		if(get_path_and_name(argv[2],&path,&name)){
			char *temp=NULL;
			if((temp=realloc(path,sizeof(char)*strlen(path)+8))!=NULL){//reallocate, making room for the ".tags" concat
				path=temp;
				temp=NULL;
			}
			strcat(path,".tags");

			query_tagfile(stdout,path,name);
			free(path);
			free(name);
		}else{
			fprintf(stderr,"tag: `%s\' no such file or directory\n",argv[2]);
			free(path);
			free(name);
			return 1;
		}

	/*bad arguments*/
	}else{
		fprintf(stderr,"tag: invalid operand\n");
		fprintf(stderr,"Try `tag --help' for more information.\n");
		return 1;
	}


	return 0;
}




#endif
