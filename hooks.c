#include <linux/lsm_hooks.h>
#include <linux/sysctl.h>
#include <linux/sched.h>
#include <linux/proc_fs.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/sysfs.h>
#include <linux/slab.h>
#include "include/objsec.h"
#include "ztlinuxfs.h"
#include "avc.h"

extern ULinkedList ulinkedList;
extern PLinkedList plinkedList;

static struct kmem_cache *sel_inode_cache;
static struct kmem_cache *file_security_cache;


static int inode_alloc_security(struct inode *inode)
{
	struct inode_security_struct *isec = kmem_cache_zalloc(sel_inode_cache, GFP_NOFS);
    int uid = current->real_cred->uid.val;

    UNode *uNode = getUNode(&ulinkedList, uid);
	if (uNode)
	{
		isec->uid = uid;
		isec->department = uNode->data->department;
		isec->project = uNode->data->project;
		isec->level = uNode->data->level;
	} else {
		isec->uid = uid;
		isec->department = 1;
		isec->project = 1;
		isec->level = 1;
	}
	inode->i_security = isec;
	if (!isec)
		return -ENOMEM;
	return 0;
}

static void inode_free_security(struct inode *inode)
{
	struct inode_security_struct *isec = inode->i_security;
	inode->i_security = NULL;
	kmem_cache_free(sel_inode_cache, isec);
}

static struct inode_security_struct *inode_security(struct inode *inode)
{
	return inode->i_security;
}

static int file_alloc_security(struct file *file)
{
	struct file_security_struct *fsec;
	int uid = current->real_cred->uid.val;

	fsec = kmem_cache_zalloc(file_security_cache, GFP_KERNEL);
	UNode *uNode = getUNode(&ulinkedList, uid);
	if (uNode)
	{
		fsec->uid = uid;
		fsec->department = uNode->data->department;
		fsec->project = uNode->data->project;
		fsec->level = uNode->data->level;
	} else {
		fsec->uid = uid;
		fsec->department = 1;
		fsec->project = 1;
		fsec->level = 1;
	}
	file->f_security = fsec;
	printk("uid = %d, dep = %d, pro = %d, levle = %d\n", fsec->uid, fsec->department, fsec->project, fsec->level);
	if (!fsec)
		return -ENOMEM;
	return 0;
}

static void file_free_security(struct file *file)
{
	struct file_security_struct *fsec = file->f_security;
	file->f_security = NULL;
	kmem_cache_free(file_security_cache, fsec);
}

static int may_create(struct inode *dir, struct dentry *dentry, u16 tclass)
{
	return 0;
}

static int may_link(struct inode *dir, struct dentry *dentry, int kind)
{
	return 0;
}

static int may_rename(struct inode *old_dir, struct dentry *old_dentry, struct inode *new_dir, struct dentry *new_dentry)
{
	struct inode_security_struct *isec = old_dentry->d_inode->i_security;
	int uid = current->real_cred->uid.val;

	int rc = check_inode_request(uid, isec);
	return rc;
}

static int ztlinux_inode_alloc_security(struct inode *inode)
{
	return inode_alloc_security(inode);
}

static void ztlinux_inode_free_security(struct inode *inode)
{
	inode_free_security(inode);
}

static int ztlinux_inode_create(struct inode *dir, struct dentry *dentry, umode_t mode)
{
	return 0;
}

static int ztlinux_inode_link(struct dentry *old_dentry, struct inode *dir, struct dentry *new_dentry)
{
	return 0;
}

static int ztlinux_inode_unlink(struct inode *dir, struct dentry *dentry)
{
	return 0;
}

static int ztlinux_inode_mkdir(struct inode *dir, struct dentry *dentry, umode_t mask)
{
	return 0;
}

static int ztlinux_inode_rmdir(struct inode *dir, struct dentry *dentry)
{
	return 0;
}

static int ztlinux_inode_rename(struct inode *old_inode, struct dentry *old_dentry, struct inode *new_inode, struct dentry *new_dentry)
{	
	return may_rename(old_inode, old_dentry, new_inode, new_dentry);
}

static int ztlinux_inode_permission(struct inode *inode, int mask)
{
	return 0;
}

static int ztlinux_file_permission(struct file *file, int mask)
{
	return 0;
}

static int ztlinux_file_alloc_security(struct file *file)
{
	return file_alloc_security(file);
}

static void ztlinux_file_free_security(struct file *file)
{
	file_free_security(file);
}

static int ztlinux_file_open(struct file *file, const struct cred *cred)
{
	struct file_security_struct *fsec;
	struct inode_security_struct *isec;

	int uid = current->real_cred->uid.val;

	int rc = check_file_request(uid, file);
	return rc;
}

static int ztlinux_task_alloc(struct task_struct *task, unsigned long clone_flags)
{
	return 0;
}

static int ztlinux_task_kill(struct task_struct *p, struct siginfo *info, int sig, const struct cred *cred)
{
	return 0;
}


static struct security_hook_list ztlinux_hooks[] = {
	LSM_HOOK_INIT(inode_alloc_security, ztlinux_inode_alloc_security),
	LSM_HOOK_INIT(inode_free_security, ztlinux_inode_free_security),
	// LSM_HOOK_INIT(inode_create, ztlinux_inode_create),
	// LSM_HOOK_INIT(inode_link, ztlinux_inode_link),
	// LSM_HOOK_INIT(inode_unlink, ztlinux_inode_unlink),
	// LSM_HOOK_INIT(inode_mkdir, ztlinux_inode_mkdir),
	// LSM_HOOK_INIT(inode_rmdir, ztlinux_inode_rmdir),
	LSM_HOOK_INIT(inode_rename, ztlinux_inode_rename),
	// LSM_HOOK_INIT(inode_permission, ztlinux_inode_permission),
	// LSM_HOOK_INIT(file_permission, ztlinux_file_permission),
	// LSM_HOOK_INIT(file_alloc_security, ztlinux_file_alloc_security),
	// LSM_HOOK_INIT(file_free_security, ztlinux_file_free_security),
	// LSM_HOOK_INIT(file_open, ztlinux_file_open),
	// LSM_HOOK_INIT(task_alloc, ztlinux_task_alloc),
	// LSM_HOOK_INIT(task_kill, ztlinux_task_kill),
	
};
void __init ztlinux_add_hooks(void)
{
	pr_info("Demo: becoming mindful.\n");							// print relevant mesg, cat by dmesg | grep huhu
	security_add_hooks(ztlinux_hooks, ARRAY_SIZE(ztlinux_hooks), "huhu"); // add security model function
}

static __init int ztlinux_init(void)
{
	sel_inode_cache = kmem_cache_create("ztlinux_inode_security",
					    sizeof(struct inode_security_struct),
					    0, SLAB_PANIC, NULL);
	file_security_cache = kmem_cache_create("ztlinux_file_security",
					    sizeof(struct file_security_struct),
					    0, SLAB_PANIC, NULL);
	ztlinux_add_hooks();

	return 0;
}
security_initcall(ztlinux_init); // 4. register this hook function