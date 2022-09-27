/**
 * Description:
 *  Print name of current/working directory.
 * 
 * Function:
 *  Starts in current direntory and recursively
 *  climbs up to root of filesystem, prints top part
 *  then prints current part.
 * 
 * Function Process:
 *  1.uses readdir() to get info about each thing.
 * 
 * TODO:
 *  prints an empty string if run from "/".
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

ino_t get_file_inode(char *);
void print_path_to_ino(ino_t);
void ino_to_name(ino_t , char *, int);

/**
 * Function: Returns current file's restrict path.
 */
int main(int argc, char **argv)
{
    print_path_to_ino(get_file_inode("."));
    putchar('\n');

    return 0;
}

/**
 * Function: Prints path leading down to an object with this inode, using recursive way.
 */
void print_path_to_ino(ino_t inode)
{
    ino_t my_inode;
    char its_name[BUFSIZ] = {0};

    if (get_file_inode("..") != inode) {
        chdir("..");
        ino_to_name(inode, its_name, BUFSIZ);
        my_inode = get_file_inode(".");
        print_path_to_ino(my_inode);
        printf("/%s", its_name);
    }

    return;
}

/**
 * Function: Looks through current directory for a file with this inode
 * number and copies its name into namebuf.
 */
void ino_to_name(ino_t inode_to_find, char *namebuf, int buflen)
{
    DIR *dir_ptr;
    struct dirent *dirent_ptr;

    if ((dir_ptr = opendir(".")) == NULL) {
        fprintf(stderr, "Failed to get (%llu)current directory's form\n", 
            (unsigned long long)inode_to_find);
        perror(".:");
        exit(1);
    }

    while ((dirent_ptr = readdir(dir_ptr)) != NULL) {
        if (dirent_ptr->d_ino == inode_to_find) {
            strncpy(namebuf, dirent_ptr->d_name, buflen);
            namebuf[buflen - 1] = '\0';
            closedir(dir_ptr);
            return;
        }
    }
    
    closedir(dir_ptr);
    fprintf(stderr, "Failed to find the ino(%llu)'s filename in current directory\n",
        (unsigned long long)inode_to_find);
    exit(1);

    return;
}

/**
 * Function: Returns inode number of the file.
 */
ino_t get_file_inode(char *file_name)
{
    struct stat st_info;
    
    if (stat(file_name, &st_info) == -1) {
        fprintf(stderr, "Failed to get file(%s)'s stat info\n", file_name);
        perror(file_name);
        exit(1);
    }
    
    return st_info.st_ino;
}
