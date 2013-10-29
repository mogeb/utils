#include <linux/module.h>

MODULE_LICENSE("Dual BSD/GPL");

inline volatile long long rdtsc()
{
	long lo, hi;
	__asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
	return (long long)hi << 32 | lo;
}

static int hello_init(void) {
	printk(KERN_INFO "Hello world!\n");
	asm volatile(".byte 0x0F,0x01,0xC1\n"::"a"(5));
	printk(KERN_INFO "MG tsc = %lld\n", rdtsc());
	return 0;
}

static void hello_exit(void) {
	printk(KERN_INFO "Bye, cruel world\n");
}

module_init(hello_init);
module_exit(hello_exit);
