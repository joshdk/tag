#ifndef _TAG_C_
#define _TAG_C_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define DEBUG 0
#include "debug.h"
#include "tag.h"
#include "res.h"


int main(int argc,char **argv){

	if(argc<2){
		//error, missing parameters
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

	if(argc<3){
		fprintf(stderr,"tag: mising operand\n");
		fprintf(stderr,"Try `tag --help' for more information.\n");
		return 1;
	}


	if(!strcmp(argv[1],"-t")){
		char *path=NULL;
		char *name=NULL;
		if(get_path_and_name(argv[argc-1],&path,&name)){
			tag_tagfile(stdout,path,name,argv+2,argc-3);
			free(path);
			free(name);
		}else{
			fprintf(stderr,"tag: `%s\' no such file or directory\n",argv[argc-1]);
			free(path);
			free(name);
			return 1;
		}

	}else if(!strcmp(argv[1],"-f")){
		if(!find_tagfile(argv[argc-1],argv+2,argc-3)){
			fprintf(stderr,"tag: `%s\' no such directory\n",argv[argc-1]);
			return 1;
		}

	}else if(!strcmp(argv[1],"-q")){
		char *path=NULL;
		char *name=NULL;
		if(get_path_and_name(argv[argc-1],&path,&name)){
			query_tagfile(stdout,path,name);
			free(path);
			free(name);
		}else{
			fprintf(stderr,"tag: `%s\' no such file or directory\n",argv[argc-1]);
			free(path);
			free(name);
			return 1;
		}

	}else{
		fprintf(stderr,"tag: invalid operand\n");
		fprintf(stderr,"Try `tag --help' for more information.\n");
		return 1;
	}


	return 0;
}




#endif
