#ifndef _SIDTAB_H
#define _SIDTAB_H
#define SIDTAB_SIZE 128
#include <linux/slab.h> 

typedef struct user_context {
    int uid;
    int department;
    int project;
    int level;
}UContext;

typedef struct user_node {
    UContext *data;
    struct user_node *next;
}UNode;

typedef struct user_linkedList {
    UNode *head;
    UNode *tail;
}ULinkedList;

void initializeUList(ULinkedList* list);              //初始化链表
ULinkedList* createUList(void);
int addUHead(ULinkedList* list, UNode* position, UContext* data);              //给链表的头节点添加数据
UNode* getUNode(ULinkedList* list, int uid);
int updateUList(ULinkedList* list, UContext* data);
void addUList(ULinkedList* list, UContext* data);
void deleteUList(ULinkedList* list, UContext* data);
#endif