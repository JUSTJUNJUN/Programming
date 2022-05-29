/**
 * use utmp cache
 */
#include <stdio.h>
#include <stdlib.h>
#include <utmp.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define UTMP_FILE_POS   "/var/run/utmp"
#define NRECS   16
#define NULLUT  ((struct utmp *)NULL)
#define UTSIZE  (sizeof(struct utmp))

static char utmp_buf[NRECS*UTSIZE] = {0};
static int cur_rec = 0;
static int num_of_recs = 0;
static int fd_utmp = -1;

void show_info(struct utmp *);
void show_time(struct utmp *);
int utmp_open(char *filename);
struct utmp *utmp_next(int utfd);
int utmp_reload(int fd);
void utmp_close(int fd);

int main(void)
{
    struct utmp *user_record = NULL;
    int utmp_fd = 0;
    int read_len = sizeof(struct utmp);
    
    if ((utmp_fd = utmp_open(UTMP_FILE_POS)) == -1) {
        perror(UTMP_FILE_POS);
        exit(1);
    }

    while ((user_record = utmp_next(utmp_fd)) != NULLUT) {
        show_info(user_record);
    }
    utmp_close(utmp_fd);

    return 0;
}

void show_info(struct utmp *record)
{
    if (record->ut_type != USER_PROCESS) {
        return;
    }

    printf("%-8.8s", record->ut_user);
    printf(" ");
    printf("%-8.8s", record->ut_line);
    printf(" ");
    show_time(record);
    printf("%s", record->ut_host);
    printf("\n");

    return;
}

void show_time(struct utmp *record)
{
    char *time = NULL;
    time_t t = 0;

    memcpy(&t, &(record->ut_tv), sizeof(time_t));
    time = ctime(&t);
    //printf("%ld", t);
    printf("%12.12s", time + 4);
    printf(" ");

    return;
}

int utmp_open(char *filename)
{
    fd_utmp = open(filename, O_RDONLY);
    cur_rec = num_of_recs = 0;

    return fd_utmp;
}

struct utmp *utmp_next(int utfd)
{
    struct utmp *record = NULL;

    if (utfd == -1) {
        return NULLUT;
    }

    if (cur_rec == num_of_recs && utmp_reload(utfd) == 0) {/* cache and utmpfile are both empty */
        return NULLUT;
    }

    record = (struct utmp *)&utmp_buf[cur_rec*UTSIZE];
    cur_rec++;

    return record;
}

int utmp_reload(int fd)
{
    int num_of_read = 0;
    
    num_of_read = read(fd, utmp_buf, NRECS * UTSIZE);
    num_of_recs = num_of_read / UTSIZE;
    cur_rec = 0;
    
    return num_of_recs;
}

void utmp_close(int fd)
{
    if (fd != -1) {
        close(fd);
    }    

    return;
}