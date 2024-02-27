

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <list.h>


#define CMD_1   0
#define CMD_2   1
#define CMD_MAX 2

#define CMD_LEN 256

struct cmd_msg {
    int cmd_code;
    char buf[CMD_LEN];//如果是不同环境的，只能用buffer数组，否则可以用指针   
};


typedef int (*cmd_func)(char *buf);

int cmd1_handler(char *buf)
{
    return 0;
}


int cmd2_handler(char *buf)
{
    return 0;
}


cmd_func cmd_table[] = {
    cmd1_handler,
    cmd2_handler,       
};


int invoker1()
{   
    struct cmd_msg cmd1_case;
    memset(&cmd1_case, 0, sizeof(cmd1_case));
    cmd1_case.cmd_code = CMD_1;
    //send cmd1_case to queue
    return 0;
}


int invoker2()
{   
    struct cmd_msg cmd1_case;

    memset(&cmd1_case, 0, sizeof(cmd1_case));

    cmd1_case.cmd_code = CMD_2;
    //send cmd1_case to queue

    return 0;
}


int cmd_receiver()
{
    struct cmd_msg *cmd_case;

    while (1) {
        //get cmd_case from queue while queue is not empty 
        (*cmd_table[cmd_case->cmd_code])(cmd_case->buf);
    }

    return 0;
}


int main(void)
{
    invoker1();
    invoker2();
    cmd_receiver();
    return 0;
}
