#ifndef _DSV_H_
#define _DSV_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


extern int resize(void **,int,int,int);
extern int split(char *,char ***,int *);
extern int readline(FILE *,char **,int *);


#endif
