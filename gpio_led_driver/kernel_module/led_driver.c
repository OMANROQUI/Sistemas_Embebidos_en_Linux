// kernel_module/led_driver.c
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/string.h>
#include <linux/io.h>

#define DEVICE_NAME   "gpio_led"
#define PERIPH_BASE   0x3F000000UL
#define GPIO_BASE     (PERIPH_BASE + 0x200000UL)
#define GPFSEL1       (GPIO_BASE + 0x04)
#define GPSET0        (GPIO_BASE + 0x1C)
#define GPCLR0        (GPIO_BASE + 0x28)

static void __iomem *gpio_virt;
static int majorNumber;
static bool led_state = false;

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Equipo GPIO");
MODULE_DESCRIPTION("LED driver con logging extenso");

// Prototipos
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
    uint32_t val;

    pr_info("gpio_led: Init module\n");

    gpio_virt = ioremap(GPIO_BASE, 0x100);
    if (!gpio_virt) {
        pr_err("gpio_led: ioremap failed\n");
        return -ENOMEM;
    }
    pr_info("gpio_led: MMIO mapped at %p\n", gpio_virt);

    val = readl(gpio_virt + (GPFSEL1 - GPIO_BASE));
    pr_debug("gpio_led: GPFSEL1 before = 0x%08x\n", val);
    val &= ~(7 << 21);
    val |=  (1 << 21);
    writel(val, gpio_virt + (GPFSEL1 - GPIO_BASE));
    pr_info("gpio_led: Configured GPIO17 as output (GPFSEL1 = 0x%08x)\n", val);

    writel((1 << 17), gpio_virt + (GPCLR0 - GPIO_BASE));
    pr_info("gpio_led: LED initially OFF (GPCLR0 write)\n");

    majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
    if (majorNumber < 0) {
        pr_err("gpio_led: register_chrdev failed: %d\n", majorNumber);
        iounmap(gpio_virt);
        return majorNumber;
    }
    pr_info("gpio_led: registered with major %d\n", majorNumber);
    return 0;
}

static void __exit led_exit(void)
{
    writel((1 << 17), gpio_virt + (GPCLR0 - GPIO_BASE));
    pr_info("gpio_led: LED forced OFF on exit\n");

    unregister_chrdev(majorNumber, DEVICE_NAME);
    pr_info("gpio_led: chrdev unregistered\n");

    iounmap(gpio_virt);
    pr_info("gpio_led: MMIO unmapped\nModule unloaded\n");
}

static int led_open(struct inode *inodep, struct file *filep)
{
    pr_info("gpio_led: Device opened\n");
    return 0;
}

static int led_release(struct inode *inodep, struct file *filep)
{
    pr_info("gpio_led: Device closed\n");
    return 0;
}

static ssize_t led_read(struct file *filep, char __user *buffer, size_t len, loff_t *offset)
{
    const char *s = led_state ? "on\n" : "off\n";
    size_t sl = strlen(s);
    ssize_t to_copy;

    pr_debug("gpio_led: Read requested len=%zu offset=%lld\n", len, *offset);
    if (*offset >= sl) {
        pr_debug("gpio_led: Read EOF\n");
        return 0;
    }
    to_copy = min(len, sl - *offset);
    if (copy_to_user(buffer, s + *offset, to_copy)) {
        pr_err("gpio_led: copy_to_user failed\n");
        return -EFAULT;
    }
    *offset += to_copy;
    pr_info("gpio_led: Read %zd bytes, new offset=%lld\n", to_copy, *offset);
    return to_copy;
}

static ssize_t led_write(struct file *filep, const char __user *buffer, size_t len, loff_t *offset)
{
    char cmd[16];
    size_t cmd_len = min(len, sizeof(cmd)-1);
    uint32_t reg;

    if (copy_from_user(cmd, buffer, cmd_len)) {
        pr_err("gpio_led: copy_from_user failed\n");
        return -EFAULT;
    }
    cmd[cmd_len] = '\0';
    while (cmd_len && (cmd[cmd_len-1]=='\n' || cmd[cmd_len-1]=='\r'))
        cmd[--cmd_len] = '\0';
    pr_info("gpio_led: Write command='%s'\n", cmd);

    if (strcmp(cmd, "on") == 0) {
        writel((1 << 17), gpio_virt + (GPSET0 - GPIO_BASE));
        led_state = true;
        pr_info("gpio_led: Turned ON (GPSET0 write)\n");
    } else if (strcmp(cmd, "off") == 0) {
        writel((1 << 17), gpio_virt + (GPCLR0 - GPIO_BASE));
        led_state = false;
        pr_info("gpio_led: Turned OFF (GPCLR0 write)\n");
    } else {
        pr_warn("gpio_led: Invalid write argument '%s'\n", cmd);
        return -EINVAL;
    }

    return len;
}

module_init(led_init);
module_exit(led_exit);
