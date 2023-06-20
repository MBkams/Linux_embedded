#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h> 

#define TAILLE 1024  // Define the size of the message buffer

static struct proc_dir_entry *proc_entry;
static char message[TAILLE];
static size_t size_of_message;

ssize_t fops_read(struct file *file, char __user *buffer, size_t count, loff_t *ppos) {
    int errno = 0;
    int copy;

    if (*ppos > 0 || count < size_of_message)
        return 0;

    if (copy_to_user(buffer, message, size_of_message))
        return -EFAULT;

    *ppos = size_of_message;
    return size_of_message;
}

ssize_t fops_write(struct file *file, const char __user *buffer, size_t count, loff_t *ppos) {
    int len = count;

    if (len > TAILLE)
        len = TAILLE;

    if (copy_from_user(message, buffer, len))
        return -EFAULT;

    message[len] = '\0';
    size_of_message = len;

    printk("Received : %s\n", message);

    return len;
}

static const struct file_operations proc_fops = {
    .owner = THIS_MODULE,
    .read = fops_read,
    .write = fops_write,
};

static int __init le_module_init(void) {
    proc_entry = proc_create("my_proc_file", 0666, NULL, &proc_fops);
    if (!proc_entry) {
        printk(KERN_ALERT "Failed to create /proc/my_proc_file\n");
        return -ENOMEM;
    }

    printk(KERN_INFO "Module starting!\n");
    return 0;
}

static void __exit le_module_exit(void) {
    remove_proc_entry("my_proc_file", NULL);
    printk(KERN_ALERT "Bye bye Bilal...\n");
}

module_init(le_module_init);
module_exit(le_module_exit);


MODULE_DESCRIPTION("Un fichier dans /proc/");
