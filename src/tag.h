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
#include "debug.h"


int lstat(const char *, struct stat *); 
char *strdup(const char *);

struct tagrow{
	char *name;
	char **tags;
	int len;
};

extern int make_tagrow(char **,int,struct tagrow *);
extern int free_tagrow(struct tagrow *);
extern int get_path_and_name(const char *,char **,char **);
extern int modify_tagrow(struct tagrow *,char *,int);
extern int query_tagfile(FILE *,const char *,const char *);
extern int tag_tagfile(FILE *stream,char *,char *,char **,int);
extern int match_tagrow(struct tagrow *,char **,int);
extern int search_tagfile(char *path,char *,char **,int);
extern void search(char *,char **,int);
extern int find_tagfile(char *,char **,int);


#endif
