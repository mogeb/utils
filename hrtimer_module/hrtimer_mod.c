#include <linux/module.h>
#include <linux/hrtimer.h>

MODULE_LICENSE("Dual BSD/GPL");

#define PRINT_HRTIMER_MOD     KERN_INFO "hrtimer_mod: "

#define USEC                  1000
#define MSEC                  1000000

#define ITER                  700
#define WAIT_SEC              0
#define WAIT_NSEC             10*MSEC

static struct hrtimer hr_timer;
ktime_t ktime;
int count = 0;

enum hrtimer_restart hrtimer_handler(struct hrtimer *timer)
{
	printk(PRINT_HRTIMER_MOD "HR timer function\n");
	count++;
	if(count < ITER) {
		ktime = ktime_set(WAIT_SEC, WAIT_NSEC);
		hrtimer_start(&hr_timer, ktime, HRTIMER_MODE_REL);
		printk(PRINT_HRTIMER_MOD "Resetting the hrtimer\n");
		printk(PRINT_HRTIMER_MOD "WAIT BEFORE REMOVING THE MODULE!\n");
	} else {
		printk(PRINT_HRTIMER_MOD "OK to remove module\n");
	}
	return HRTIMER_NORESTART;
}

static int hrtimer_mod_init(void) {

	ktime = ktime_set(WAIT_SEC, WAIT_NSEC);
	printk(PRINT_HRTIMER_MOD "Hello hrtimer_mod!\n");
	hrtimer_init(&hr_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	hr_timer.function = &hrtimer_handler;
	printk(PRINT_HRTIMER_MOD "Start HR timer \n");
	hrtimer_start(&hr_timer, ktime, HRTIMER_MODE_REL);
	return 0;
}

static void hrtimer_mod_exit(void) {
	printk(PRINT_HRTIMER_MOD "Bye, hrtimer_mod\n");
}

module_init(hrtimer_mod_init);
module_exit(hrtimer_mod_exit);
