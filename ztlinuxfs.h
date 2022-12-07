#ifndef _ZTLINUXFS_H_
#define _ZTLINUXFS_H_
#include <linux/kernel.h> 
#include <linux/proc_fs.h>
#include <linux/module.h>  				//任何模块都必须包含，定义了可动态加载到内核的模块所需要的必要信息
#include <linux/init.h>    				//必须包含，包含了宏__init(指定初始化函数)和__exit(指定清除函数)
#include <linux/slab.h> 
#include "sidtab.h"
#include "policy.h"

#define BUFFER_SIZE 1024
static struct proc_dir_entry *userdbfile;
static struct proc_dir_entry *policydbfile;

static ssize_t userdb_read(struct file *file, char __user *buf, size_t lbuf, loff_t *ppos);
static ssize_t userdb_write(struct file *file, const char __user *buf, size_t lbuf, loff_t *ppos);

static ssize_t policydb_read(struct file *file, char __user *buf, size_t lbuf, loff_t *ppos);
static ssize_t policydb_write(struct file *file, const char __user *buf, size_t lbuf, loff_t *ppos);
#endif 