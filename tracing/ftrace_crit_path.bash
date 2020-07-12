#!/bin/bash

schedevents=(sched_switch sched_process_fork sched_process_exec sched_process_exit sched_waking sched_ttwu sched_process_wait sched_wait_task sched_migrate_task sched_process_free sched_wakeup_new sched_wakeup sched_pi_setprio)

irqevents=(irq_handler_entry irq_handler_exit irq_softirq_entry irq_softirq_exit irq_softirq_raise)

timerevents=(hrtimer_cancel hrtimer_expire_exit hrtimer_start itimer_state timer_cancel timer_expire_exit timer_start hrtimer_expire_entry  hrtimer_init itimer_expire tick_stop timer_expire_entry timer_init)

find /sys/kernel/debug/tracing/events/sched -name enable -exec bash -c "echo 1 > {}" \;
find /sys/kernel/debug/tracing/events/irq -name enable -exec bash -c "echo 1 > {}" \;
find /sys/kernel/debug/tracing/events/timer -name enable -exec bash -c "echo 1 > {}" \;
find /sys/kernel/debug/tracing/events/syscalls -name enable -exec bash -c "echo 1 > {}" \;

# for ev in ${schedevents[*]} ; do
#     echo 1 > /sys/kernel/debug/tracing/events/sched/$ev/enable
# done
# for ev in ${irqevents[*]} ; do
#     echo 1 > /sys/kernel/debug/tracing/events/irq/$ev/enable
# done
# for ev in ${timerevents[*]} ; do
#     echo 1 > /sys/kernel/debug/tracing/events/timer/$ev/enable
# done

echo 1 > /sys/kernel/debug/tracing/tracing_on

sleep "$1"

echo 0 > /sys/kernel/debug/tracing/tracing_on
cp /sys/kernel/debug/tracing/trace trace

# for ev in ${schedevents[*]} ; do
#     echo 0 > /sys/kernel/debug/tracing/events/sched/$ev/enable
# done
# for ev in ${irqevents[*]} ; do
#     echo 0 > /sys/kernel/debug/tracing/events/irq/$ev/enable
# done
# for ev in ${timerevents[*]} ; do
#     echo 0 > /sys/kernel/debug/tracing/events/timer/$ev/enable
# done

find /sys/kernel/debug/tracing/events/sched -name enable -exec bash -c "echo 0 > {}" \;
find /sys/kernel/debug/tracing/events/irq -name enable -exec bash -c "echo 0 > {}" \;
find /sys/kernel/debug/tracing/events/timer -name enable -exec bash -c "echo 0 > {}" \;
find /sys/kernel/debug/tracing/events/syscalls -name enable -exec bash -c "echo 0 > {}" \;
