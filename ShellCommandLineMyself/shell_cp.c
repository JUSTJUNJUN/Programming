/**
 * Description:
 *  Copy files and directories.
 * 
 * Function Process:
 *  1.  Open sourcefile for reading;
 *  2.  Open copyfile for writing;
 *  3.  Read from sourcefile to buffer;
 *  4.  Write buffer to copyfile;
 *  5.  Close sourcefile & copyfile.
 * 
 */
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

void file_err_deal(char *s1, char *s2);

int main(int argc, char **argv)
{
    int src_fd, dst_fd;
    int num_of_read = 0;
    char buf[BUFSIZ];

    if (argc != 3) {
        fprintf(stderr, "usage: argument lack\n");
        exit(1);
    }

    if ((src_fd = open(argv[1], O_RDONLY)) == -1) {
        file_err_deal("Cannot open src file", argv[1]);
    }

    if ((dst_fd = creat(argv[2], 0644)) == -1) {
        file_err_deal("Cannot open dst file", argv[2]);
    }

    while ((num_of_read = read(src_fd, buf, BUFSIZ)) > 0) {
        if (write(dst_fd, buf, num_of_read) != num_of_read) {
            file_err_deal("Cannot write to dst file", argv[2]);
        }
    }

    if (num_of_read == -1) {
        file_err_deal("Cannot read from src file", argv[1]);
    }

    if (close(src_fd) == -1 || close(dst_fd) == -1) {
        file_err_deal("Cannot close src&dst file", "");
    }
    
    return 0;
}

void file_err_deal(char *s1, char *s2)
{
    fprintf(stderr, "Error: %s", s1);/* error note */
    perror(s2);/* print syscall error */
    exit(1);/* error reason has been printed, exit normally */
}
