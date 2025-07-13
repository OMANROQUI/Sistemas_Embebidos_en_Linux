// kernel_module/led_driver.c
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/string.h>
#include <linux/io.h>
#include <linux/gpio.h>      // gpio_set_value(), gpio_is_valid()


#define DEVICE_NAME "gpio_led"
// Para Pi 3/4 usa 0xFE000000 en vez de 0x3F000000 si fuera necesario
#define PERIPH_BASE   0x3F000000UL
#define GPIO_BASE     (PERIPH_BASE + 0x200000UL)
#define GPFSEL1_OFS   0x04
#define GPSET0_OFS    0x1C
#define GPCLR0_OFS    0x28
#define GPIO_PIN    17
#define CMD_ON_LEN   2   // longitud de la cadena "on"
#define CMD_OFF_LEN  3   // longitud de la cadena "off"


static void __iomem *gpio_base;
static int majorNumber;
static bool led_state;

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Persona B – OMANROQUI");
MODULE_DESCRIPTION("GPIO LED driver por acceso directo a registros");

static int     led_open(struct inode *, struct file *);
static int     led_release(struct inode *, struct file *);
static ssize_t led_read(struct file *, char __user *, size_t, loff_t *);
static ssize_t led_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos);

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

    /* 1) Mapear regiones de GPIO */
    gpio_base = ioremap(GPIO_BASE, 0x1000);
    if (!gpio_base) {
        pr_err("gpio_led: fallo en ioremap\n");
        return -ENOMEM;
    }

    /* 2) Configurar GPIO_PIN como salida */
	v = readl(gpio_base + GPFSEL1_OFS);
	v &= ~(7 << 21);         // limpiar bits para GPIO17
	v |=  (1 << 21);         // 001 = salida
	writel(v, gpio_base + GPFSEL1_OFS);

    /* 3) Asegurar LED apagado */
    writel(1 << GPIO_PIN, gpio_base + GPCLR0_OFS);

    /* 4) Registrar char device */
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

static ssize_t led_write(struct file *file, const char __user *buf,
                         size_t count, loff_t *ppos)
{
    char kbuf[CMD_OFF_LEN + 1] = {0};

    if (count > CMD_OFF_LEN)
        count = CMD_OFF_LEN;
    if (copy_from_user(kbuf, buf, count))
        return -EFAULT;
    kbuf[count] = '\0';

    if (strncmp(kbuf, "on", CMD_ON_LEN) == 0) {
        /* Encender: set bit */
        writel(1 << GPIO_PIN, gpio_base + GPSET0_OFS);
        pr_info("led_driver: LED ENCENDIDO\n");
    } else if (strncmp(kbuf, "off", CMD_OFF_LEN) == 0) {
        /* Apagar: clear bit */
        writel(1 << GPIO_PIN, gpio_base + GPCLR0_OFS);
        pr_info("led_driver: LED APAGADO\n");
    } else {
        pr_warn("led_driver: comando desconocido: %s\n", kbuf);
        return -EINVAL;
    }

    return count;
}



module_init(led_init);
module_exit(led_exit);
