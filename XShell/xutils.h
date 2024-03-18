#ifndef __XUTILS_H__
#define __XUTILS_H__


#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "xarg.h"
#include "xerr.h"


typedef enum {FALSE, TRUE} Boolean;


#define min(m, n) ((m) < (n) ? (m) : (n))
#define max(m, n) ((m) > (n) ? (m) : (n))


#endif