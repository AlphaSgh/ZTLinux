#include <linux/module.h>  				//任何模块都必须包含，定义了可动态加载到内核的模块所需要的必要信息
#include <linux/init.h>    				//必须包含，包含了宏__init(指定初始化函数)和__exit(指定清除函数)
#include <linux/kernel.h> 
#include <linux/slab.h> 
#include <linux/moduleparam.h>
#include <linux/proc_fs.h>
#include "sidtab.h"

EXPORT_SYMBOL(initializeUList);
void initializeUList(ULinkedList *list) {
    list->head = NULL;
    list->tail = NULL;
}
EXPORT_SYMBOL(createUList);
ULinkedList* createUList(void) {
    ULinkedList* ulist = kmalloc(sizeof(ULinkedList), GFP_ATOMIC);
    if (ulist != NULL)
    {
        initializeUList(ulist);
    }
    return ulist;
}
EXPORT_SYMBOL(addUHead);
int addUHead(ULinkedList *list, UNode* position, UContext* data) {
    UNode* unode = kmalloc(sizeof(UNode), GFP_ATOMIC);
    if (unode == NULL) {
        return -1;
    }
    unode->data = data;
    if (position == NULL) {
        unode->next = list->head;
        list->head = unode;
    } else {
        unode->next = position->next;
        position->next = unode;
    }
    if (list->tail == position) {
        list->tail = unode;
    }
    return 0;    
}
EXPORT_SYMBOL(getUNode);
UNode *getUNode(ULinkedList *list, int uid) {
    UNode* position;
    for (position = list->head; position != NULL; position = position->next) {
        if (position->data->uid == uid) {
            return position;
        }
    }
    return NULL;
}
EXPORT_SYMBOL(updateUList);
int updateUList(ULinkedList* list, UContext* data) {
    UNode* position;
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
EXPORT_SYMBOL(deleteUList);
void deleteUList(ULinkedList* list, UContext* data) {
    UNode* prev, *p;
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
EXPORT_SYMBOL(addUList);
void addUList(ULinkedList *list, UContext *data) {
    UNode *uNode = getUNode(list, data->uid);
    if (uNode) {
        updateUList(list, data);
    } else {
        addUHead(list, NULL, data);
    }
}