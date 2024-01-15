/**
 * Description:
 *  Show who is logged on.
 * 
 * Function Process:
 *  1.  Open /var/run/utmp;
 *  2.  Read records to utmp buf(usr/include/utmp.h);
 *  3.  Use buffer method to decrease consuming.
 *  4.  Show the record;
 *  5.  Close /var/run/utmp;
 * 
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
int user_logout(char line_buf[], int line_size);

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

int user_logout(char line_buf[], int line_size)
{
    int fd = 0;
    char record_buf[UTSIZE] = {0};
    struct utmp *tmp = NULL;
    int retval = -1;

    fd = open(UTMP_FILE_POS, O_RDWR);
    if (fd == -1) {
        printf("Open utmp file failed, exit\n");
        return -1;
    }

    while (read(fd, record_buf, UTSIZE) == UTSIZE) {
        tmp = (struct utmp *)record_buf;
        if (strncmp(tmp->ut_line, line_buf, line_size) == 0) {
            printf("Modify the record to logout\n");
            tmp->ut_type = DEAD_PROCESS;
            if (time(&(tmp->ut_tv)) != -1) {


                if (lseek(fd, -UTSIZE, SEEK_CUR) != -1) {
                    if (write(fd, tmp, UTSIZE) != -1) {
                        retval = 0;
                    }
                }
            }
            break;
        }
    }
    
    if (close(fd) == -1) {
        retval = -1;
    }

    return retval;
}


