// kernel_module/led_driver.c
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/string.h>
#include <linux/io.h>

#define DEVICE_NAME "gpio_led"
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
MODULE_DESCRIPTION("LED driver por acceso directo a registros");

// Prototipos
static int     led_open(struct inode *, struct file *);
static int     led_release(struct inode *, struct file *);
static ssize_t led_read(struct file *, char __user *, size_t, loff_t *);
static ssize_t led_write(struct file *, const char __user *, size_t, loff_t *);

static struct file_operations fops = {
    .owner   = THIS_MODULE,
    .open    = led_open,
    .release = led_release,
    .read    = led_read,
    .write   = led_write,
};

static int __init led_init(void)
{
    uint32_t val;

    // 1) Reserva espacio para MMIO
    gpio_virt = ioremap(GPIO_BASE, 0x100);
    if (!gpio_virt) {
        pr_err("gpio_led: ioremap failed\n");
        return -ENOMEM;
    }

    // 2) Configura GPIO17 (bits 21-23 en GPFSEL1) a '001' = output
    val = readl(gpio_virt + (GPFSEL1 - GPIO_BASE));
    val &= ~(7 << 21);
    val |=  (1 << 21);
    writel(val, gpio_virt + (GPFSEL1 - GPIO_BASE));

    // Aseguramos LED apagado
    writel((1 << 17), gpio_virt + (GPCLR0 - GPIO_BASE));
    led_state = false;

    // 3) Registro del device file
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
    // Apagar LED
    writel((1 << 17), gpio_virt + (GPCLR0 - GPIO_BASE));

    unregister_chrdev(majorNumber, DEVICE_NAME);
    iounmap(gpio_virt);
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

static ssize_t led_read(struct file *filep, char __user *buffer, size_t len, loff_t *offset)
{
    const char *s = led_state ? "on\n" : "off\n";
    size_t sl = strlen(s);
    if (*offset >= sl) return 0;
    if (len > sl - *offset) len = sl - *offset;
    if (copy_to_user(buffer, s + *offset, len)) return -EFAULT;
    *offset += len;
    return len;
}

static ssize_t led_write(struct file *filep, const char __user *buffer, size_t len, loff_t *offset)
{
    char cmd[16];
    size_t cmd_len = len;
    uint32_t reg;

    if (cmd_len >= sizeof(cmd)) return -EINVAL;
    if (copy_from_user(cmd, buffer, cmd_len)) return -EFAULT;
    cmd[cmd_len] = '\0';
    while (cmd_len && (cmd[cmd_len-1]=='\n'||cmd[cmd_len-1]=='\r'))
        cmd[--cmd_len] = '\0';

    if (strcmp(cmd, "on") == 0) {
        // SET GPIO17
        writel((1 << 17), gpio_virt + (GPSET0 - GPIO_BASE));
        led_state = true;
        pr_info("gpio_led: Turned ON\n");
    } else if (strcmp(cmd, "off") == 0) {
        // CLEAR GPIO17
        writel((1 << 17), gpio_virt + (GPCLR0 - GPIO_BASE));
        led_state = false;
        pr_info("gpio_led: Turned OFF\n");
    } else {
        return -EINVAL;
    }

    return len;
}

module_init(led_init);
module_exit(led_exit);

