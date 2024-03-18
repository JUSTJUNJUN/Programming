#include <stdarg.h>
#include "xerr.h"
#include "xutils.h"

#ifdef __GNUC__
__attribute__((__noreturn__))
#endif


static void
terminate(Boolean useExit3)
{
    char *s;

    s = getenv("EF_DUMPCORE");

    if (s != NULL && *s != '\0') {
        abort();
    } else if (useExit3) {
        exit(EXIT_FAILURE);
    } else {
        _exit(EXIT_FAILURE);
    }

    return;
}


static void
outputError(Boolean useErr, int err, Boolean flushStdout,
            const char *format, va_list ap)
{
#define BUF_SIZE    500

    char buf[BUF_SIZE], userMsg[BUF_SIZE], errText[BUF_SIZE];

    vsnprintf(userMsg, BUF_SIZE, format, ap);

    if (useErr) {
        snprintf(errText, BUF_SIZE, "[%s %s]",
                    (err > 0 && err <= MAX_ENAME)?
                    ename[err]: "?UNKNOWN?", strerror(err));
    } else {
        snprintf(errText, BUF_SIZE, ":");
    }

    snprintf(buf, BUF_SIZE, "ERROR%s %s\n", errText, userMsg);

    if (flushStdout) {
        fflush(stdout);
    }

    fputs(buf, stderr);
    fflush(stderr);

    return;
}