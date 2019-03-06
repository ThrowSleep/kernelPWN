#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
void (*my_funptr)(void);
int bug1_write(struct file *file,const char *buf,unsigned long len)
{
        my_funptr();
        return len;
}
static int __init null_dereference_init(void)
{
        printk(KERN_ALERT "null_dereference driver init!\n");
        create_proc_entry("bug1",0666,0)->write_proc = bug1_write;
       return 0;
}
static void __exit null_dereference_exit(void)
{
        printk(KERN_ALERT "null_dereference driver exit\n");
}
module_init(null_dereference_init);
module_exit(null_dereference_exit);

