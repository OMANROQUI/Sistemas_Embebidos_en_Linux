#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <linux/cdev.h>

#define DEVICE_NAME "led_driver"
#define GPIO_LED 21

static dev_t dev_number;
static struct cdev led_cdev;

static int led_open(struct inode *inode, struct file *file) {
    printk(KERN_INFO "led_driver: Dispositivo abierto\n");
    return 0;
}

static int led_release(struct inode *inode, struct file *file) {
    printk(KERN_INFO "led_driver: Dispositivo cerrado\n");
    return 0;
}

static ssize_t led_read(struct file *file, char __user *buf, size_t len, loff_t *offset) {
    char value;

    if (*offset > 0)
        return 0;

    value = gpio_get_value(GPIO_LED) ? '1' : '0';

    if (copy_to_user(buf, &value, 1))
        return -EFAULT;

    *offset = 1;
    return 1;
}

static ssize_t led_write(struct file *file, const char __user *buf, size_t len, loff_t *offset) {
    char kbuf;

    if (copy_from_user(&kbuf, buf, 1))
        return -EFAULT;

    printk(KERN_INFO "led_driver: valor recibido = %c\n", kbuf);

    if (kbuf == '1') {
        gpio_set_value(GPIO_LED, 1);
        printk(KERN_INFO "led_driver: LED ENCENDIDO\n");
    } else if (kbuf == '0') {
        gpio_set_value(GPIO_LED, 0);
        printk(KERN_INFO "led_driver: LED APAGADO\n");
    } else {
        printk(KERN_WARNING "led_driver: Valor no válido recibido: %c\n", kbuf);
        return -EINVAL;
    }

    return 1;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = led_open,
    .release = led_release,
    .read = led_read,
    .write = led_write,
};

static int __init led_init(void) {
    int ret;

    ret = alloc_chrdev_region(&dev_number, 0, 1, DEVICE_NAME);
    if (ret < 0) return ret;

    cdev_init(&led_cdev, &fops);
    ret = cdev_add(&led_cdev, dev_number, 1);
    if (ret < 0) return ret;

    if (!gpio_is_valid(GPIO_LED)) return -ENODEV;

    gpio_request(GPIO_LED, "LED");
    gpio_direction_output(GPIO_LED, 0);

    printk(KERN_INFO "led_driver: Módulo cargado. Major=%d Minor=%d\n", MAJOR(dev_number), MINOR(dev_number));
    return 0;
}

static void __exit led_exit(void) {
    gpio_set_value(GPIO_LED, 0);
    gpio_free(GPIO_LED);
    cdev_del(&led_cdev);
    unregister_chrdev_region(dev_number, 1);
    printk(KERN_INFO "led_driver: Módulo descargado\n");
}

module_init(led_init);
module_exit(led_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Omar");
MODULE_DESCRIPTION("Driver de caracter para controlar LED por GPIO");

