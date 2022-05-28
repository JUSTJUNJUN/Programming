/**
 * Function description:
 * Show 24 lines from input;
 * Print [more ?] message;
 * Input control key;
 * Enter:advance one line
 * Space:advance one screen
 * q:exit
 * h:help info
 */

#include <stdio.h>
#include <stdlib.h>

#define PAGELEN 24
#define LINELEN 512

void do_more(FILE *);/* show */
int see_more(FILE *);/* deal next ctrl action */

int main (int argc, char **argv)
{
    FILE *fp = NULL;

    if (argc == 1) {/* No other inargs, read from stdin */
        do_more(stdin);
    } else {
        while (--argc) {
            if ((fp = fopen(*(++argv), "r")) != NULL) {
                do_more(fp);
                fclose(fp);
            } else {
                exit(1);
            }
        }   
    }

    return 0;
}

void do_more(FILE *fp)
{
    char line_buf[LINELEN] = {0};
    int num_of_lines = 0;
    int reply = 0;
    FILE *fp_tty = NULL;

    fp_tty = fopen("/dev/tty", "r");
    if (fp_tty == NULL) {
        exit(1);
    }

    while (fgets(line_buf, LINELEN, fp)) {
        if (num_of_lines == PAGELEN) {
            reply = see_more(fp_tty);
            if (reply == 0) {
                break;
            }
            num_of_lines -= reply;
        }

        if (fputs(line_buf, stdout) == EOF) {
            exit(1);
        }

        num_of_lines++;
    }

    return;
}

/* return read again line nums, or stop to read */
int see_more(FILE *cmd)
{
    int ctrl_c = 0;

    printf("\033[7m Show more? \033[m");
    while ((ctrl_c = getc(cmd)) != EOF) {/* stdin may be redirected */
        switch (ctrl_c) {
        case 'q':
            return 0;
        
        case ' ':
            return PAGELEN;
        
        case '\n':
            return 1;

        default:
            printf("\033[7m Useless Command:if you want to quit, please input 'q'. \033[m");
            continue;
        }
    }

    return 0;
}