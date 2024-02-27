

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <list.h>


//不需要处理结果
typedef int (*observer_func)(char *buf);


struct observer_ops_node {
    struct list_head list;  //内核链表标准结构
    observer_func *handler;  //handler的回调函数，没有优先级
};


//全局的观察者链
struct list_head observer_global_list;


//具体的处理函数
int observer_handler1(char *buf)
{
    //do something
    return 0;
}


int observer_handler2(char *buf)
{
    //do something
    return 0;
}


//封装成节点
struct observer_ops_node node1 = {
    .handler = observer_handler1,
};


struct observer_ops_node node2 = {
    .handler = observer_handler2,
};


int observer_register(struct observer_ops_node *node)
{
    //lock observer_global_list
    //add node into observer_global_list
    //unlock observer_global_list
    return 0;
}


int observer_unregister(struct observer_ops_node *node)
{
    //lock observer_global_list
    //delete node into observer_global_list
    //unlock observer_global_list
    return 0;
}


int main(void)
{
    struct list_head *node;
    struct observer_ops_node *node_func;
    char buf[16];

    observer_register(&node1);
    observer_register(&node1);

    //something happend, should trigger responsibility observer
    //fill buf with event

    list_for_each(node, &observer_global_list) {
        node_func = (struct observer_ops_node *)node;
        node_func.handler(buf);
    }

    return 0;
}