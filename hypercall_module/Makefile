obj-m := hyperc_mod.o

all:
	make -C /lib/modules/`uname -r`/build M=`pwd` modules

clean:
	rm -f hyperc_mod.ko  hyperc_mod.mod.c  hyperc_mod.mod.o  hyperc_mod.o  modules.order	Module.symvers .hyperc*
	rm -rf .tmp_versions
