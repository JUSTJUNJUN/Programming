/**
 * Description:
 *  List directory contents.
 * 
 * Function Process:
 *  1.  Open dir;
 *  2.  Read entry in the dir;
 *  3.  Show each entry's info;
 *  4.  Close dir.
 * 
 * Option: 
 *  -l, -a, -lu, -s, -t, -F
 * 
 * ToDo:
 *  1.  Show the total num of records;
 *  2.  Sort by file name;
 *  3.  Consider the parameter is file not dir;
 *  4.  Show specified dir;
 */

#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <grp.h>
#include <pwd.h>

void do_ls(char *dir_name);
void show_ls(char *name);
void stat_mode_to_string(unsigned short mode, char mode_str[], int mode_str_len);
void stat_time_to_str(time_t time, char time_str[], int time_str_len);
char *uid_to_name(__uid_t uid);
char *gid_to_name(__gid_t gid);

int main(int argc, char **argv)
{
    if (argc == 1) {
        do_ls(".");
    } else {
        while (--argc) {
            printf("%s\n", *++argv);
            do_ls(*argv);
        }   
    }

    return 0;
}

void do_ls(char *dir_name)
{
    DIR *dir_pt = NULL;
    struct dirent *dir_entry_pt = NULL;

    if ((dir_pt = opendir(dir_name)) == NULL) {
        fprintf(stderr, "Cannot open directory: %s\n", dir_name);
    } else {
        while ((dir_entry_pt = readdir(dir_pt)) != NULL) {
            printf("%s\n", dir_entry_pt->d_name);
            show_ls(dir_entry_pt->d_name);
        }
        closedir(dir_pt);
    }

    return;
}

void show_ls(char *name)
{
    struct stat info;
    char mode_buf[11] = {0};
    char time_buf[128] = {0};

    if (stat(name, &info) == 0) {
        stat_mode_to_string(info.st_mode, mode_buf, sizeof(mode_buf));
        stat_time_to_str(info.st_mtime, time_buf, sizeof(time_buf));

        printf("mode:%s, link:%lu, user:%s, grp:%d,\
                size:%ld, modtime:%s, name:%s\n",\
                mode_buf, info.st_nlink, uid_to_name(info.st_uid), info.st_gid,\
                info.st_size, time_buf, name);
    } else {
        perror(name);
    }

    return;
}

void stat_mode_to_string(unsigned short mode, char mode_str[], int mode_str_len)
{
    if (mode_str_len < 11) {
        printf("Mode str buffer is too short, Cannot tranform the mode\n");
        return;
    }

    strncpy(mode_str, "----------", mode_str_len);

    if (S_ISDIR(mode)) mode_str[0] = 'd';
    if (S_ISCHR(mode)) mode_str[0] = 'c';
    if (S_ISBLK(mode)) mode_str[0] = 'b';

    if (S_IRUSR & mode) mode_str[1] = 'r';
    if (S_IWUSR & mode) mode_str[2] = 'w';
    if (S_IXUSR & mode) mode_str[3] = 'x';

    if (S_IRGRP & mode) mode_str[4] = 'r';
    if (S_IWGRP & mode) mode_str[5] = 'w';
    if (S_IXGRP & mode) mode_str[6] = 'x';

    if (S_IROTH & mode) mode_str[7] = 'r';
    if (S_IWOTH & mode) mode_str[8] = 'w';
    if (S_IXOTH & mode) mode_str[9] = 'x';

    mode_str[10] = '\0';
    
    return;
}

void stat_time_to_str(time_t time, char time_str[], int time_str_len)
{
    char *time_ptr = NULL;

    time_ptr = ctime(&time);
    if (strlen(time_ptr) > time_str_len) {
        printf("Time str buffer is too short, Cannot tranform the time\n");
        return;
    }
    strncpy(time_str, time_ptr, time_str_len);

    return;
}

char *uid_to_name(__uid_t uid)
{
    struct passwd *pd = NULL;
    static char numstr[10] = {0};

    if ((pd = getpwuid(uid)) == NULL) {
        sprintf(numstr, "%d", uid);
        return numstr;
    } else {
        return pd->pw_name;
    }
}

char *gid_to_name(__gid_t gid)
{
    struct group *gp = NULL;
    static char numstr[10] = {0};

    if ((gp = getgrgid(gid)) == NULL) {
        sprintf(numstr, "%d", gid);
        return numstr;
    } else {
        return gp->gr_name;
    }
}
