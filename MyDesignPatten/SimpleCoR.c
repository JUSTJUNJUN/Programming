

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <list.h>


//两类处理结果，子类可以扩展
#define CHAIN_PASS 0
#define CHAIN_STOP 1

typedef int (*chain_func)(char *buf);

struct chain_ops_node {
    struct list_head list;  
    chain_func *handler;  //handler的回调函数
    int priority; //优先级
};


//全局的责任链
struct list_head chain_global_list;


//具体的处理函数
int chain_handler1(char *buf)
{
    //do something
    if (/*some conditions*/) {
        return CHAIN_PASS;
    }

    return CHAIN_STOP;
}


int chain_handler2(char *buf)
{
    //do something
    if (/*some conditions*/) {
        return CHAIN_PASS;
    }

    return CHAIN_STOP;
}


//封装成节点
struct chain_ops_node node1 =
{
    .handler = chain_handler1,
    .priority = 0
};

struct chain_ops_node node2 =
{
    .handler = chain_handler2,
    .priority = 1
};


int chain_register(struct chain_ops_node *node)
{   
    //lock chain_global_list
    //add node into chain_global_list according to priority
    //unlock chain_global_list

    return 0;
}


int chain_unregister(struct chain_ops_node *node)
{
    //lock chain_global_list
    //delete node into chain_global_list
    //unlock chain_global_list

    return 0;
}


int main(void)
{
    struct list_head *node;
    struct chain_ops_node *node_func;
    char buf[16];

    chain_register(&node1);
    chain_register(&node2);
    //something happend, should trigger responsibility chain
    //fill buf with event
    list_for_each(node, &chain_global_list) {
        node_func = (struct chain_ops_node *)node;
        if(node_func.handler(buf) == CHAIN_STOP)
        {break;}
    }

    return 0;
}