/*
 * csaw.c
 * CSAW CTF Challenge Kernel Module
 * Jon Oberheide <jon@oberheide.org>
 *
 * This module implements the /proc/csaw interface which can be read
 * and written like a normal file. For example:
 *
 * $ cat /proc/csaw
 * Welcome to the CSAW CTF challenge. Best of luck!
 * $ echo "Hello World" > /proc/csaw
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/string.h>
#include <asm/uaccess.h>

#define MAX_LENGTH 64

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jon Oberheide");
MODULE_DESCRIPTION("CSAW CTF Challenge Kernel Module");

static struct proc_dir_entry *csaw_proc;

int
csaw_write(struct file *file, const char __user *ubuf, unsigned long count, void *data)
{
    char buf[MAX_LENGTH];

    unsigned int x = (unsigned int) &buf[0];
    memset(&buf[0], 0, sizeof(buf));

    x -= 20;

    printk(KERN_ERR "csaw: called csaw_write 2010\n");

    // print_hex_dump(KERN_INFO, "raw data: ", DUMP_PREFIX_ADDRESS,
    //           16, 4, (void *) x, 120 * sizeof(int), 1);

    /*
     * We should be safe to perform this copy from userspace since our
     * kernel is compiled with CC_STACKPROTECTOR, which includes a canary
     * on the kernel stack to protect against smashing the stack.
     *
     * While the user could easily DoS the kernel, I don't think they
     * should be able to escalate privileges without discovering the
     * secret stack canary value.
     */
    if (copy_from_user(&buf, ubuf, count)) {
        printk(KERN_INFO "csaw: error copying data from userspace\n");
        return -EFAULT;
    }

//   print_hex_dump(KERN_INFO, "raw data: ", DUMP_PREFIX_ADDRESS,
//              16, 4, (void *) x, 120 * sizeof(int), 1);

    return count;
}

int
csaw_read(char *page, char **start, off_t off, int count, int *eof, void *data)
{
    char buf[MAX_LENGTH];

    printk(KERN_INFO "csaw: called csaw_read 2010\n");

    *eof = 1;
    memset(buf, 0, sizeof(buf));
    strcpy(buf, "Welcome to the CSAW CTF challenge. Best of luck!\n");
    memcpy(page, buf + off, MAX_LENGTH);

    return MAX_LENGTH;
}

static int __init
csaw_init(void)
{
    printk(KERN_INFO "csaw: loading module\n");

    csaw_proc = create_proc_entry("csaw2010", 0666, NULL);
    csaw_proc->read_proc = csaw_read;
    csaw_proc->write_proc = csaw_write;

    printk(KERN_INFO "csaw: created /proc/csaw2010 entry\n");

    return 0;
}

static void __exit
csaw_exit(void)
{
    if (csaw_proc) {
        remove_proc_entry("csaw2010", csaw_proc);
    }

    printk(KERN_INFO "csaw: unloading module\n");
}

module_init(csaw_init);
module_exit(csaw_exit);

