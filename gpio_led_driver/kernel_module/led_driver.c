#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>        // register_chrdev, unregister_chrdev

#define DEVICE_NAME "gpio_led"
static int majorNumber;       // Major dinámico

MODULE_LICENSE("GPL");
MODULE_AUTHOR("OscarDavidGuerrero");
MODULE_DESCRIPTION("GPIO LED driver – gestión de major/minor y open/release");

static int led_open(struct inode *inodep, struct file *filep);
static int led_release(struct inode *inodep, struct file *filep);

static struct file_operations fops = {
    .owner   = THIS_MODULE,
    .open    = led_open,
    .release = led_release,
};

static int __init led_init(void) {
    printk(KERN_INFO "gpio_led: Init module\n");
    majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
    if (majorNumber < 0) {
        printk(KERN_ERR "gpio_led: Failed to register a major number\n");
        return majorNumber;
    }
    printk(KERN_INFO "gpio_led: Registered correctly with major number %d\n", majorNumber);
    return 0;
}

static void __exit led_exit(void) {
    unregister_chrdev(majorNumber, DEVICE_NAME);
    printk(KERN_INFO "gpio_led: Module unloaded, major %d freed\n", majorNumber);
}

static int led_open(struct inode *inodep, struct file *filep) {
    printk(KERN_INFO "gpio_led: Device opened\n");
    return 0;
}

static int led_release(struct inode *inodep, struct file *filep) {
    printk(KERN_INFO "gpio_led: Device closed\n");
    return 0;
}

module_init(led_init);
module_exit(led_exit);
