#include "avc.h"

extern ULinkedList ulinkedList;
extern PLinkedList plinkedList;

int check_inode_request_default(int uid, struct inode_security_struct *isec, UNode *uNode)
{
    int rc = 0;
	if (uNode->data->level >= isec->level) {
		if (uNode->data->department == isec->department && uNode->data->project == isec->project) {
			printk("rename success\n");
			rc = 0;
		} else {
			printk("department or project can't\n");
			rc = 1;
		}
	} else {
		printk("level can't\n");
		rc = 1;
	}
    return rc;
}

int check_file_request_default(int uid, struct file *file, UNode *uNode)
{
    int rc = 0;

	rc = check_inode_request_default(uid, file->f_inode->i_security, uNode);
    return rc;

}
int check_task_request_default(int uid, struct task_security_struct *tsec)
{
    int rc = 0;
    return rc;
}

int check_inode_request_policy(int uid, struct inode_security_struct *isec, PNode *pNode)
{
    int rc = 0;
	if (pNode->data->level >= isec->level) {
		if (pNode->data->department == isec->department && pNode->data->project == isec->project) {
			printk("rename success\n");
			rc = 0;
		} else {
			printk("department or project can't\n");
			rc = 1;
		}
	} else {
		printk("level can't\n");
		rc = 1;
	}
    return rc;
}

int check_file_request_policy(int uid, struct file *file, PNode *pNode)
{
	int rc = 0;

	rc = check_inode_request_policy(uid, file->f_inode->i_security, pNode);
    return rc;
}

int check_inode_request(int uid, struct inode_security_struct *isec)
{
	int rc = 0;
	PNode *pNode = getPNode(&plinkedList, uid);
	UNode *uNode = getUNode(&ulinkedList, uid);
	if (pNode) {
		rc = check_inode_request_policy(uid, isec, pNode);
	} else if (uNode) {
		rc = check_inode_request_default(uid, isec, uNode);
	}
	return rc;
}

int check_file_request(int uid, struct file *file)
{
	int rc = 0;
	PNode *pNode = getPNode(&plinkedList, uid);
	UNode *uNode = getUNode(&ulinkedList, uid);
	if (pNode) {
		rc = check_file_request_policy(uid, file, pNode);
	} else if (uNode) {
		rc = check_file_request_default(uid, file, uNode);
	}
	return rc;
}
int check_task_request(int uid, struct task_security_struct *tsec)
{
    int rc = 0;
    return rc;
}