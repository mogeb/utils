obj-m := hrtimer_mod.o

all:
	make -C /lib/modules/`uname -r`/build M=`pwd` modules

clean:
	rm -f hrtimer_mod.ko  hrtimer_mod.mod.c  hrtimer_mod.mod.o  hrtimer_mod.o  modules.order	Module.symvers
