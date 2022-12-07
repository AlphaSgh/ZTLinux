#ifndef _AVC_H_
#define _AVC_H_
#include "ztlinuxfs.h"
#include "include/objsec.h"

int check_inode_request(int uid, struct inode_security_struct *isec);
int check_file_request(int uid, struct file *file);
int check_task_request(int uid, struct task_security_struct *tsec);

#endif