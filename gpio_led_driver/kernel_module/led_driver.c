// kernel_module/led_driver.c
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/string.h>
#include <linux/io.h>

#define DEVICE_NAME "gpio_led"
// Para Pi 3/4 usa 0xFE000000 en vez de 0x3F000000 si fuera necesario
#define PERIPH_BASE   0x3F000000UL
#define GPIO_BASE     (PERIPH_BASE + 0x200000UL)
#define GPFSEL1_OFS   0x04
#define GPSET0_OFS    0x1C
#define GPCLR0_OFS    0x28

static void __iomem *gpio_base;
static int majorNumber;
static bool led_state;

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Persona B – OMANROQUI");
MODULE_DESCRIPTION("GPIO LED driver por acceso directo a registros");

static int     led_open(struct inode *, struct file *);
static int     led_release(struct inode *, struct file *);
static ssize_t led_read(struct file *, char __user *, size_t, loff_t *);
static ssize_t led_write(struct file *, const char __user *, size_t, loff_t *);

static const struct file_operations fops = {
    .owner   = THIS_MODULE,
    .open    = led_open,
    .read    = led_read,
    .write   = led_write,
    .release = led_release,
};

static int __init led_init(void)
{
    uint32_t v;

    pr_info("gpio_led: Init module (direct regs)\n");

    gpio_base = ioremap(GPIO_BASE, 0x100);
    if (!gpio_base) {
        pr_err("gpio_led: ioremap failed\n");
        return -ENOMEM;
    }

    v = readl(gpio_base + GPFSEL1_OFS);
    v &= ~(7 << 21);
    v |=  (1 << 21);          // set GPIO17 as output
    writel(v, gpio_base + GPFSEL1_OFS);

    // asegurar LED apagado
    writel(1 << 17, gpio_base + GPCLR0_OFS);
    led_state = false;

    majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
    if (majorNumber < 0) {
        pr_err("gpio_led: register_chrdev failed: %d\n", majorNumber);
        iounmap(gpio_base);
        return majorNumber;
    }

    pr_info("gpio_led: registered with major %d\n", majorNumber);
    return 0;
}

static void __exit led_exit(void)
{
    // apagar LED
    writel(1 << 17, gpio_base + GPCLR0_OFS);

    unregister_chrdev(majorNumber, DEVICE_NAME);
    iounmap(gpio_base);
    pr_info("gpio_led: module unloaded\n");
}

static int led_open(struct inode *inodep, struct file *filep)
{
    pr_info("gpio_led: device opened\n");
    return 0;
}

static int led_release(struct inode *inodep, struct file *filep)
{
    pr_info("gpio_led: device closed\n");
    return 0;
}

static ssize_t led_read(struct file *filep, char __user *buf, size_t len, loff_t *offset)
{
    const char *s = led_state ? "on\n" : "off\n";
    size_t sl = strlen(s);
    ssize_t to_copy;

    if (*offset >= sl)
        return 0;
    to_copy = (len < sl - *offset) ? len : (sl - *offset);
    if (copy_to_user(buf, s + *offset, to_copy))
        return -EFAULT;
    *offset += to_copy;
    return to_copy;
}

static ssize_t led_write(struct file *filep, const char __user *buf, size_t len, loff_t *offset)
{
    char cmd[8];
    size_t l = min(len, sizeof(cmd)-1);
    uint32_t regbit = 1 << 17;
    if (copy_from_user(cmd, buf, l)) return -EFAULT;
    cmd[l] = '\0';
    if (!strncmp(cmd, "on", 2)) {
        writel(regbit, gpio_base + GPSET0_OFS);
        led_state = true;
    } else if (!strncmp(cmd, "off", 3)) {
        writel(regbit, gpio_base + GPCLR0_OFS);
        led_state = false;
    } else {
        return -EINVAL;
    }
    return len;
}

module_init(led_init);
module_exit(led_exit);
