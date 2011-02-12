#ifndef _DBG_H_
#define _DBG_H_
#include <stdarg.h>
#include <stdio.h>




#if DEBUG
#define DEBUG_PREFIX "\e[31;1m"
#define DEBUG_SUFFIX "\e[0m"


void _debugf(FILE *stream,const char* format,...){
	va_list args;
	va_start(args,format);
	fprintf(stream,DEBUG_PREFIX);
	vfprintf(stream,format,args);
	fprintf(stream,DEBUG_SUFFIX);
	fflush(stream);
	va_end(args);
}
	#define debugf(format, args...) _debugf(stderr, format , ##args)
	#define fdebugf(stream, format, args...) _debugf(stream, format , ##args)
//	#define ldebugf(level, format, args...) _debugf(stderr, level, format , ##args)
//	#define lfdebugf(stream, level, format, args...) _debugf(stream, level, format , ##args)
#else
	#define debugf(format, args...)
	#define fdebugf(file, format, args...)
//	#define ldebugf(level, format, args...)
//	#define lfdebugf(stream, level, format, args...)
#endif

#endif
