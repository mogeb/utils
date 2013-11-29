obj-m := periodic_hyperc.o

all:
	make -C /lib/modules/`uname -r`/build M=`pwd` modules

clean:
	rm -f periodic_hyperc.ko  periodic_hyperc.mod.c  periodic_hyperc.mod.o  periodic_hyperc.o  modules.order	Module.symvers
	rm -f .periodic*
	rm -rf .tmp_versions
