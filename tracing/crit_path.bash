#!/bin/bash

lttng create -o .
lttng enable-channel --num-subbuf 16 --subbuf-size 8M -k c0
lttng enable-event sched_switch,sched_process_fork,sched_process_exec,sched_process_exit,sched_waking,irq_handler_entry,irq_handler_exit,irq_softirq_entry,irq_softirq_exit,irq_softirq_raise,sched_ttwu,sched_process_wait,sched_wait_task,sched_migrate_task,timer_init,timer_start,timer_expire_entry,timer_expire_exit,timer_hrtimer_init,timer_hrtimer_start,timer_hrtimer_cancel,timer_hrtimer_expire_entry,timer_hrtimer_expire_exit,timer_itimer_expire,sched_process_free,sched_wakeup_new,sched_wakeup,sched_pi_setprio,lttng_statedump_process_state,x86_irq_vectors_ipi_entry,x86_irq_vectors_ipi_exit,inet_sock_local_in,inet_sock_local_out,lttng_statedump_interrupt -k -c c0
lttng enable-event --syscall -a -k -c c0
#lttng add-context -k -t procname -c c0

lttng start

