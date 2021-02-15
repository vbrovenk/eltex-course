#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <stdlib.h>
#include "ename.c.inc"

#define TRUE 1
#define FALSE 0

#define MAX_ERROR_LEN 256
#define BUF_SIZE 500

static char buf[MAX_ERROR_LEN];

// char	*strerror(int err) {
// 	if (err < 0 || err >= _sys_nerr || _sys_errlist[err] == NULL) {
// 		snprintf(buf, MAX_ERROR_LEN, "Unknown error %d", err);
// 	} else {
// 		strncpy(buf, _sys_errlist[err], MAX_ERROR_LEN - 1);
// 		buf[MAX_ERROR_LEN - 1] = '\0';
// 	}

// 	return (buf);
// }

static void terminate(int useExit3) {
	char *s;  

	/* Сохраняет дамп ядра, если переменная среды EF_DUMPCORE определена 
	и содержит непустую строку; в противном случае вызывает exit(3) 
	или _exit(2), в зависимости от значения 'useExit3'. */  
	s = getenv("EF_DUMPCORE");
	if (s != NULL && *s != '\0')    
		abort();
	else if (useExit3)
		exit(EXIT_FAILURE);
	else
		exit(EXIT_FAILURE);
}

static void outputError(int useErr, int err, int flushStdout,  const char *format, va_list ap) {
	char buf[BUF_SIZE], userMsg[BUF_SIZE], errText[BUF_SIZE];

	vsnprintf(userMsg, BUF_SIZE, format, ap);  
	if (useErr)    
		snprintf(errText, BUF_SIZE, " [%s %s]", (err > 0 && err <= MAX_ENAME) ? ename[err] : "?UNKNOWN?", strerror(err));  
	else
		snprintf(errText, BUF_SIZE, ":");  
	snprintf(buf, BUF_SIZE, "ERROR%s %s\n", errText, userMsg);  
	if (flushStdout)
		fflush(stdout);   /* Сброс всего ожидающего стандартного вывода */  
	fputs(buf, stderr);  
	fflush(stderr);    /* При отсутствии построчной буферизации в stderr */}

void	errExitEN(int errnum, const char *format, ...) {
	va_list argList;
	va_start(argList, format);
	outputError(TRUE, errnum, TRUE, format, argList);
	va_end(argList);  
	terminate(TRUE);
}