#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include "myerror.h"

#define MAXLINE 4096
static void  err_doit(int, const char *, va_list) ;
char* pname = NULL ; /* caller can set this from argv[0] */

void err_ret(const char* fmt, ...) {
  va_list ap;
  va_start(ap,fmt) ;
  err_doit(1,fmt,ap) ;
  va_end(ap) ;
  return ;
}

void err_sys(const char* fmt, ...) {
  va_list ap ;

  va_start(ap,fmt) ;
  err_doit(1,fmt,ap) ;
  va_end(ap) ;
  exit(1) ;
}

void err_dump(const char* fmt, ...) {
  va_list ap ;

  va_start(ap,fmt) ;
  err_doit(1,fmt,ap) ;
  va_end(ap) ;
  abort() ;
  exit(1) ;
}

void err_msg(const char* fmt, ...) {
  va_list ap ;

  va_start(ap,fmt) ;
  err_doit(0,fmt,ap) ;
  va_end(ap) ;
  return ;
}

void err_quit(const char* fmt, ...) {
  va_list ap ;

  va_start(ap, fmt) ;
  err_doit(0,fmt,ap) ;
  va_end(ap) ;
  exit(1) ;
}

static void err_doit(int errnoflag, const char* fmt, va_list ap) {
  int errno_save ;
  char buf[MAXLINE] ;

  errno_save = errno ;
  vsprintf(buf,fmt,ap) ;
  if (errnoflag)
    sprintf(buf + strlen(buf), ": %s", strerror(errno_save));
  strcat(buf, "\n");
  fflush(stdout) ;  /* in case stdout and stderr are the same */
  fputs(buf,stderr) ;
  fflush (NULL) ;  /* flushes all stdio output streams */
  return ;
}
