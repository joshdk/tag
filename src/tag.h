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
//#define DEBUG 1
#include "debug.h"


int lstat(const char *, struct stat *); 
char *strdup(const char *);

struct tagrow{
	char *name;
	char **tags;
	int len;
};

int make_tagrow(char **,int,struct tagrow *);
int free_tagrow(struct tagrow *);
int get_path_and_name(const char *,char **,char **);
int modify_tagrow(struct tagrow *,char *,int);
int query_tagfile(FILE *,const char *,const char *);
int tag_tagfile(FILE *stream,char *,char *,char **,int);
int match_tagrow(struct tagrow *,char **,int);
//int search_tagfile(char *path,char *,char **,int);
//int search(char *,char **,int);
int find_tagfile(char *,char **,int);


#endif
