#include <linux/module.h>  				//任何模块都必须包含，定义了可动态加载到内核的模块所需要的必要信息
#include <linux/init.h>    				//必须包含，包含了宏__init(指定初始化函数)和__exit(指定清除函数)
#include <linux/kernel.h> 
#include <linux/slab.h> 
#include <linux/moduleparam.h>
#include <linux/proc_fs.h>
#include "policy.h"

EXPORT_SYMBOL(initializePList);
void initializePList(PLinkedList *list) {
    list->head = NULL;
    list->tail = NULL;
}
EXPORT_SYMBOL(createPList);
PLinkedList* createPList(void) {
    PLinkedList* plist = kmalloc(sizeof(PLinkedList), GFP_ATOMIC);
    if (plist != NULL)
    {
        initializePList(plist);
    }
    return plist;
}
EXPORT_SYMBOL(addPHead);
int addPHead(PLinkedList* list, PNode* position, PContext* data) {
    PNode* pnode = kmalloc(sizeof(PNode), GFP_ATOMIC);
    if (pnode == NULL) {
        return -1;
    }
    pnode->data = data;
    if (position == NULL) {
        pnode->next = list->head;
        list->head = pnode;
    } else {
        pnode->next = position->next;
        position->next = pnode;
    }
    if (list->tail == position) {
        list->tail = pnode;
    }
    return 0; 
}
EXPORT_SYMBOL(getPNode);
PNode *getPNode(PLinkedList *list, int uid) {
    PNode* position;
    for (position = list->head; position != NULL; position = position->next) {
        if (position->data->uid == uid) {
            return position;
        }
    }
    return NULL;
}
EXPORT_SYMBOL(updatePList);
int updatePList(PLinkedList* list, PContext* data) {
    PNode* position;
    for (position = list->head; position != NULL; position = position->next)
    {
        if (position->data->uid == data->uid)
        {
            position->data->uid = data->uid;
            position->data->department = data->department;
            position->data->project = data->project;
            position->data->level = data->level;
        }
    }
    return 0;
}
EXPORT_SYMBOL(deletePList);
void deletePList(PLinkedList* list, PContext* data) {
    PNode* prev, *p;
    for (prev = NULL, p = list->head; p != NULL; prev = p, p = p->next) {
        if (p->data->uid == data->uid) {
            break;
        }
    }
    if (p == NULL) {
        return;
    }
    if (p == list->head) {
        list->head = p->next;
    }
    if (p == list->tail) {
        list->tail = prev;
    }
    if (prev != NULL) {
        prev->next = p->next;
    }
    kfree(p);
}
EXPORT_SYMBOL(addPList);
void addPList(PLinkedList *list, PContext *data) {
    PNode *pNode = getPNode(list, data->uid);
    if (pNode) {
        updatePList(list, data);
    } else {
        addPHead(list, NULL, data);
    }
}