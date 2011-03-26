#ifndef _DSV_H_
#define _DSV_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int resize(void **,int,int,int);
int split(char *,char,char ***,int *);
int readline(FILE *,char **,int *);
//int writeline(FILE *,char,char **,int *);


#endif
