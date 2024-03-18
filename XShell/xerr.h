#ifndef __XERR_H__
#define __XERR_H__


void errmsg(const char *format, ...);


#ifdef __GNUC__
#define NORETURN __attribute__((__noreturn__))
#else
#define NORETURN
#endif


void errExit(const char *format, ...) NORETURN;
void err_exit(const char *format, ...) NORETURN;
void errExitEN(int errnum, const char *format, ...) NORETURN;
void fatal(const char *format, ...) NORETURN;
void usageErr(const char *format, ...) NORETURN;
void cmdLineErr(const char *format, ...) NORETURN;


static char *ename[] = {
    /* 0 */ "",
    /* 1 */ "EPERM", "ENOENT", "ESRCH", "EINTR", "EIO", "ENXIO", "E2BIG",
    /* 8 */ "ENOEXEC", "EBADF", "ECHILD", "EAGAIN/EWOULDBLOCK", "ENOMEM", 
    /* 13 */ "EACCES", "EFAULT", "ENOTBLK", "EBUSY", "EEXIST", "EXDEV", 
    /* 19 */ "ENODEV", "ENOTDIR", "EISDIR", "EINVAL", "ENFILE", "EMFILE", 
    /* 25 */ "ENOTTY", "ETXTBSY", "EFBIG", "ENOSPC", "ESPIPE", "EROFS", 
    /* 31 */
    /* 36 */
    /* 42 */
    /* 48 */
    /* 54 */
    /* 61 */
    /* 67 */
    /* 73 */
    /* 78 */
    /* 83 */
    /* 88 */
    /* 92 */
    /* 95 */
    /* 98 */
    /* 102 */
    /* 107 */
    /* 111 */
    /* 115 */
    /* 120 */
    /* 125 */
    /* 129 */
};


#define MAX_ENAME   132


#endif