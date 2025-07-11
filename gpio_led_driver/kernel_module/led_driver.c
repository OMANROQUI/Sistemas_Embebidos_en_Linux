#include <linux/module.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Persona A – Omar");
MODULE_DESCRIPTION("GPIO LED driver – init/exit functions");

static int __init led_init(void) {
    printk(KERN_INFO "gpio_led: Module init\n");
    return 0;
}

static void __exit led_exit(void) {
    printk(KERN_INFO "gpio_led: Module exit\n");
}

module_init(led_init);
module_exit(led_exit);
