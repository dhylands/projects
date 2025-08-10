//! Example of a kernel module which generates my heartbeat pattern on a LED
//! This kernel module was created to explore using GPIO from a kernel module.

#include <linux/gpio/consumer.h>
#include <linux/jiffies.h>
#include <linux/module.h>
#include <linux/timer.h>

#define GPIO_UNUSED (~0)

static struct gpio_desc *led_desc;

static uint led = GPIO_UNUSED;
module_param(led, uint, S_IRUGO);
MODULE_PARM_DESC(led, "GPIO number that the LED is connected to.");

static struct timer_list heartbeat_timer;
static unsigned long heartbeat_jiffies;
static unsigned heartbeat_tick = 0;

#define LED_OFFSET  0   // GPIO chip is 0 for the interesting RPi GPIOs

static void heartbeat_timer_cb(struct timer_list* timer) {
    if (heartbeat_tick < 4) {
        gpiod_set_value(led_desc, (heartbeat_tick ^ 1) & 1);
    }

    heartbeat_jiffies += msecs_to_jiffies(100);
    mod_timer(&heartbeat_timer, heartbeat_jiffies);

    heartbeat_tick = (heartbeat_tick + 1) % 10;
}

static int __init heartbeat_init(void) {
    printk("heartbeat_init\n");

    if (led == GPIO_UNUSED) {
        printk("Must specify `led` parameter\n");
        return -EINVAL;
    }
    printk("heartbeat_init: led = %u\n", led);

    led_desc = gpio_to_desc(LED_OFFSET + led);
    if (led_desc == NULL) {
        printk("heartbeat_init: Unable to get GPIO descriptor for LED %u\n", led);
        return -ENODEV;
    }

    int rc;
    if ((rc = gpiod_direction_output(led_desc, 0)) != 0) {
        printk("heartbeat_init: Unable to set GPIO %u to output", led);
        return rc;
    }

    timer_setup(&heartbeat_timer, heartbeat_timer_cb, 0);
    heartbeat_jiffies = jiffies;
    heartbeat_jiffies += msecs_to_jiffies(100);
    mod_timer(&heartbeat_timer, heartbeat_jiffies);

    return 0;
}

static void __exit heartbeat_exit(void) {
    printk("heartbeat_exit\n");

    del_timer(&heartbeat_timer);
    gpiod_put(led_desc);
}

module_init(heartbeat_init);
module_exit(heartbeat_exit);

MODULE_AUTHOR("Dave Hylands (dhylands@gmail.com)");
MODULE_DESCRIPTION("Heartbeat Module");
MODULE_LICENSE("GPL");
