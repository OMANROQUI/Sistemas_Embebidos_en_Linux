// kernel_module/led_driver.c
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>        // register_chrdev, unregister_chrdev
#include <linux/gpio.h>      // gpio_request, gpio_direction_output, gpio_set_value, gpio_free
#include <linux/uaccess.h>   // copy_to_user, copy_from_user
#include <linux/string.h>    // strcmp, strlen

#define DEVICE_NAME "gpio_led"
#define LED_GPIO     17      // BCM 17 (pin físico 11)
static int  majorNumber;
static bool led_state = false;

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Persona A & B – Equipo");
MODULE_DESCRIPTION("GPIO LED driver – init/exit, open/release, read/write ON/OFF");

static int     led_open(struct inode *, struct file *);
static int     led_release(struct inode *, struct file *);
static ssize_t led_read(struct file *, char __user *, size_t, loff_t *);
static ssize_t led_write(struct file *, const char __user *, size_t, loff_t *);

static struct file_operations fops = {
    .owner   = THIS_MODULE,
    .open    = led_open,
    .read    = led_read,
    .write   = led_write,
    .release = led_release,
};

static int __init led_init(void)
{
    int ret;

    printk(KERN_INFO "gpio_led: Init module\n");

    majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
    if (majorNumber < 0) {
        printk(KERN_ERR "gpio_led: Failed to register major number\n");
        return majorNumber;
    }
    printk(KERN_INFO "gpio_led: Registered with major %d\n", majorNumber);

    ret = gpio_request(LED_GPIO, DEVICE_NAME);
    if (ret && ret != -EPROBE_DEFER) {
        printk(KERN_ERR "gpio_led: gpio_request failed (%d)\n", ret);
        unregister_chrdev(majorNumber, DEVICE_NAME);
        return ret;
    }
    if (ret == 0)
        gpio_direction_output(LED_GPIO, 0);  // LED apagado

    return 0;
}

static void __exit led_exit(void)
{
    gpio_free(LED_GPIO);
    unregister_chrdev(majorNumber, DEVICE_NAME);
    printk(KERN_INFO "gpio_led: Module unloaded, freed major %d\n", majorNumber);
}

static int led_open(struct inode *inodep, struct file *filep)
{
    printk(KERN_INFO "gpio_led: Device opened\n");
    return 0;
}

static int led_release(struct inode *inodep, struct file *filep)
{
    printk(KERN_INFO "gpio_led: Device closed\n");
    return 0;
}

static ssize_t led_read(struct file *filep, char __user *buffer, size_t len, loff_t *offset)
{
    const char *state_str = led_state ? "on\n" : "off\n";
    size_t      state_len = strlen(state_str);

    if (*offset >= state_len)
        return 0;
    if (len > state_len - *offset)
        len = state_len - *offset;
    if (copy_to_user(buffer, state_str + *offset, len))
        return -EFAULT;

    *offset += len;
    return len;
}

static ssize_t led_write(struct file *filep, const char __user *buffer, size_t len, loff_t *offset)
{
    char cmd[16];
    size_t cmd_len = len;

    if (cmd_len >= sizeof(cmd))
        return -EINVAL;

    if (copy_from_user(cmd, buffer, cmd_len))
        return -EFAULT;
    cmd[cmd_len] = '\0';

    // Elimina \n y \r al final
    while (cmd_len > 0 && (cmd[cmd_len-1] == '\n' || cmd[cmd_len-1] == '\r')) {
        cmd[--cmd_len] = '\0';
    }

    if (strcmp(cmd, "on") == 0) {
        gpio_set_value(LED_GPIO, 1);
        led_state = true;
        printk(KERN_INFO "gpio_led: Turned ON\n");
    } else if (strcmp(cmd, "off") == 0) {
        gpio_set_value(LED_GPIO, 0);
        led_state = false;
        printk(KERN_INFO "gpio_led: Turned OFF\n");
    } else {
        return -EINVAL;
    }

    return len;
}

module_init(led_init);
module_exit(led_exit);
