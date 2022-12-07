#ifndef _ZTLINUX_OBJSEC_H_
#define _ZTLINUX_OBJSEC_H_

#include <linux/list.h>
#include <linux/sched.h>
#include <linux/fs.h>
#include <linux/binfmts.h>
#include <linux/in.h>
#include <linux/spinlock.h>
#include <net/net_namespace.h>

struct inode_security_struct
{
	int uid;
	int level;
	int project;
	int department;
};

struct file_security_struct 
{
	int uid;
	int department;
	int project;
	int level;	
};

struct task_security_struct
{
	int uid;
	int department;
	int project;
	int level;
};

#endif