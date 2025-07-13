// kernel_module/led_driver.c

#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/io.h>
#include <linux/uaccess.h>
#include <linux/string.h>
#include <linux/types.h>
#include <linux/printk.h>

#define DEVICE_NAME    "gpio_led"
#define PERIPH_BASE    0x3F000000UL
#define GPIO_BASE      (PERIPH_BASE + 0x200000UL)
#define GPFSEL1_OFS    0x04
#define GPSET0_OFS     0x1C
#define GPCLR0_OFS     0x28
#define GPIO_PIN       17

#define CMD_ON_LEN     2   // longitud de "on"
#define CMD_OFF_LEN    3   // longitud de "off"

static void __iomem *gpio_base;
static int  majorNumber;
static bool led_state;

// Prototipos
static int     led_open(struct inode *inode, struct file *file);
static ssize_t led_read(struct file *file, char __user *buf, size_t count, loff_t *ppos);
static ssize_t led_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos);
static int     led_release(struct inode *inode, struct file *file);

static const struct file_operations fops = {
	.owner   = THIS_MODULE,
	.open    = led_open,
	.read    = led_read,
	.write   = led_write,
	.release = led_release,
};

static int led_open(struct inode *inode, struct file *file)
{
	pr_info("gpio_led: device opened\n");
	return 0;
}

static ssize_t led_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
	char kbuf[CMD_OFF_LEN + 1];
	int  len;

	if (*ppos > 0)
		return 0;

	if (led_state)
		len = snprintf(kbuf, sizeof(kbuf), "on");
	else
		len = snprintf(kbuf, sizeof(kbuf), "off");

	if (len > count)
		len = count;

	if (copy_to_user(buf, kbuf, len))
		return -EFAULT;

	*ppos += len;
	return len;
}

static ssize_t led_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
	char kbuf[CMD_OFF_LEN + 1] = {0};
	size_t len = count > CMD_OFF_LEN ? CMD_OFF_LEN : count;

	if (copy_from_user(kbuf, buf, len))
		return -EFAULT;
	kbuf[len] = '\0';

	if (strncmp(kbuf, "on", CMD_ON_LEN) == 0) {
		/* Set pin high */
		writel(1 << GPIO_PIN, gpio_base + GPSET0_OFS);
		led_state = true;
		pr_info("gpio_led: LED ENCENDIDO\n");
	} else if (strncmp(kbuf, "off", CMD_OFF_LEN) == 0) {
		/* Clear pin */
		writel(1 << GPIO_PIN, gpio_base + GPCLR0_OFS);
		led_state = false;
		pr_info("gpio_led: LED APAGADO\n");
	} else {
		pr_warn("gpio_led: comando desconocido: %s\n", kbuf);
		return -EINVAL;
	}

	return count;
}

static int led_release(struct inode *inode, struct file *file)
{
	pr_info("gpio_led: device closed\n");
	return 0;
}

static int __init led_init(void)
{
	uint32_t v;

	pr_info("gpio_led: Init module (direct regs)\n");

	/* 1) Mapear registros GPIO */
	gpio_base = ioremap(GPIO_BASE, 0x1000);
	if (!gpio_base) {
		pr_err("gpio_led: fallo en ioremap\n");
		return -ENOMEM;
	}

	/* 2) Configurar GPIO_PIN como salida (bit 21 en GPFSEL1 para pin 17) */
	v  = readl(gpio_base + GPFSEL1_OFS);
	v &= ~(7 << 21);
	v |=  (1 << 21);
	writel(v, gpio_base + GPFSEL1_OFS);

	/* 3) Asegurar LED apagado */
	writel(1 << GPIO_PIN, gpio_base + GPCLR0_OFS);
	led_state = false;

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

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Persona B – OMANROQUI");
MODULE_DESCRIPTION("GPIO LED driver by direct register access");

module_init(led_init);
module_exit(led_exit);
