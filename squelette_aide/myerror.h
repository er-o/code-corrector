
/*
  err_ret     yes       return
  err_sys     yes       exit(1)
  err_dump    yes       abort
  err_msg     no        return
  err_quit    no        exit(1)
*/


/* Nonfatal error related to a sysem call.  Print a message and
   return. */
void err_dump(const char*, ...);

/* Fatal error related to a system call.  Print a message and
   teminate. */
void err_msg(const char*, ...) ;

/* Fatal error related to a system call.  Print a message, dump core,
   and terminate. */
void err_dump(const char*, ...) ;

/* Nonfatal error unrelated to a system call.  Print a message and
   return. */
void err_sys(const char*, ...) ;

/* Fatal error unrelated to a system call.  Print a message and
   terminate. */
void err_quit(const char*, ...) ;


