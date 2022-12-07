#include <linux/module.h>  				//任何模块都必须包含，定义了可动态加载到内核的模块所需要的必要信息
#include <linux/init.h>    				//必须包含，包含了宏__init(指定初始化函数)和__exit(指定清除函数)
#include <linux/kernel.h> 
#include <linux/slab.h> 
#include <linux/moduleparam.h>
#include <linux/proc_fs.h>
#include "ztlinuxfs.h"

ULinkedList ulinkedList;
EXPORT_SYMBOL(ulinkedList);

PLinkedList plinkedList;
EXPORT_SYMBOL(plinkedList);

static ssize_t userdb_read(struct file *file, char __user *buf, size_t lbuf, loff_t *ppos)
{
    char info[BUFFER_SIZE * 8];
    memset(info, BUFFER_SIZE * 8, 0);
    int nbytes = 0;
    UNode *cur = ulinkedList.head;
    while (cur != NULL)
    {
        nbytes += sprintf(info + nbytes, "uid == %d dep = %d pro = %d level = %d\n", cur->data->uid, cur->data->department, cur->data->project, cur->data->level);
        cur = cur->next;
    }
    return simple_read_from_buffer(buf, lbuf, ppos, info, nbytes);
}
static ssize_t userdb_write(struct file *file, const char __user *buf, size_t lbuf, loff_t *ppos)
{
    ssize_t rc;
    char tmp[BUFFER_SIZE];
    memset(tmp, BUFFER_SIZE, 0);
    rc = simple_write_to_buffer(tmp, lbuf, ppos, buf, lbuf);
    UContext *uContext = kmalloc(sizeof(UContext), GFP_ATOMIC);
    int delete = 0;
    sscanf(tmp, "%d%d%d%d%d", &uContext->uid, &uContext->department, &uContext->project, &uContext->level, &delete);
    if (delete == 1) {
        deleteUList(&ulinkedList, uContext);
    } else {
        addUList(&ulinkedList, uContext);
    }    
    return rc;
}

static ssize_t policydb_read(struct file *file, char __user *buf, size_t lbuf, loff_t *ppos)
{
    char info[BUFFER_SIZE * 8];
    memset(info, BUFFER_SIZE * 8, 0);
    int nbytes = 0;
    PNode *cur = plinkedList.head;
    while (cur != NULL)
    {
        nbytes += sprintf(info + nbytes, "uid == %d dep = %d pro = %d level = %d\n", cur->data->uid, cur->data->department, cur->data->project, cur->data->level);
        cur = cur->next;
    }
    return simple_read_from_buffer(buf, lbuf, ppos, info, nbytes);
}
static ssize_t policydb_write(struct file *file, const char __user *buf, size_t lbuf, loff_t *ppos)
{
    ssize_t rc;
    char tmp[BUFFER_SIZE];
    memset(tmp, BUFFER_SIZE, 0);
    rc = simple_write_to_buffer(tmp, lbuf, ppos, buf, lbuf);
    PContext *pContext = kmalloc(sizeof(PContext), GFP_ATOMIC);
    int delete = 0;
    sscanf(tmp, "%d%d%d%d%d", &pContext->uid, &pContext->department, &pContext->project, &pContext->level, &delete);
    if (delete == 1) {
        deletePList(&plinkedList, pContext);
    } else {
        addPList(&plinkedList, pContext);
    }    
    return rc;
}

static const struct file_operations huhu_userdb_fops = {
	.owner = THIS_MODULE,
	.read = userdb_read,
	.write = userdb_write,
};

static const struct file_operations huhu_policydb_fops = {
	.owner = THIS_MODULE,
	.read = policydb_read,
	.write = policydb_write,
};

static int userdb_file_init(void)
{
	userdbfile =proc_create("userdb",0660,NULL,&huhu_userdb_fops);
	printk(KERN_ALERT "create userdb file successfully!\n");
	return 0;
}

static int policydb_file_init(void)
{
	policydbfile =proc_create("policydb",0660,NULL,&huhu_policydb_fops);
	printk(KERN_ALERT "create policydb file successfully!\n");
	return 0;
}
static int __init file_init(void)    		//__init将函数hds_init()标记为初始化函数，在模块被装载到内核时调用hds_init()
{
    initializeUList(&ulinkedList);
	if(!(&ulinkedList))
		printk(KERN_CRIT "[test+] userdb_init init success!\n");	
	else
		printk(KERN_CRIT "[test+] userdb_init init faild!\n"); 

    initializePList(&plinkedList);
	if(!(&plinkedList))
		printk(KERN_CRIT "[test+] policydb_init init success!\n");	
	else
		printk(KERN_CRIT "[test+] policydb_init init faild!\n"); 


    printk(KERN_ALERT "[test+] ready to create userdb file!\n");
	userdb_file_init();
    policydb_file_init();
    
	return 0;
} 

static void __exit file_exit(void)			
{
	printk(KERN_ALERT "[test+] Goodbye kernel\n");  
}	
__initcall(file_init);