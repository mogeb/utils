#include <linux/module.h>
#include <linux/hrtimer.h>
#include <trace/events/periodic_hyperc.h>

#define CREATE_TRACE_POINTS

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

static inline void hypercall(int num)
{
	trace_periodic_hypercall(num);
	asm volatile(".byte 0x0F,0x01,0xC1\n"::"a"(5), "b"(num));
}

enum hrtimer_restart hrtimer_handler(struct hrtimer *timer)
{
	printk(PRINT_HRTIMER_MOD "HR timer function\n");
	count++;
	if(count < ITER) {
		ktime = ktime_set(WAIT_SEC, WAIT_NSEC);
		hrtimer_start(&hr_timer, ktime, HRTIMER_MODE_REL);
		printk(PRINT_HRTIMER_MOD "Resetting the hrtimer\n");
		printk(PRINT_HRTIMER_MOD "WAIT BEFORE REMOVING THE MODULE!\n");

		hypercall(count);
	} else {
		printk(PRINT_HRTIMER_MOD "OK to remove module\n");
	}
	return HRTIMER_NORESTART;
}

static int periodic_hyperc_init(void)
{
	ktime = ktime_set(WAIT_SEC, WAIT_NSEC);
	hrtimer_init(&hr_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	hr_timer.function = &hrtimer_handler;
	hrtimer_start(&hr_timer, ktime, HRTIMER_MODE_REL);

	return 0;
}

static void periodic_hyperc_exit(void)
{
	printk(KERN_INFO "Bye, cruel world\n");
}

module_init(periodic_hyperc_init);
module_exit(periodic_hyperc_exit);
