#ifndef _POLICY_H_
#define _POLICY_H_
#define POLICY_SIZE 128
#include <linux/slab.h> 

typedef struct policy_context {
    int uid;
    int department;
    int project;
    int level;
}PContext;

typedef struct policy_node {
    PContext *data;
    struct policy_node *next;
}PNode;

typedef struct policy_linkedList {
    PNode *head;
    PNode *tail;
}PLinkedList;

void initializePList(PLinkedList* list);              //初始化链表
PLinkedList* createPList(void);
int addPHead(PLinkedList* list, PNode* position, PContext* data);              //给链表的头节点添加数据
PNode* getPNode(PLinkedList* list, int uid);
int updatePList(PLinkedList* list, PContext* data);
void addPList(PLinkedList* list, PContext* data);
void deletePList(PLinkedList* list, PContext* data);
#endif