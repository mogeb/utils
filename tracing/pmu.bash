#!/bin/bash

lttng create -o .
lttng enable-channel --num-subbuf 16 --subbuf-size 8M -k k0
lttng enable-channel --num-subbuf 16 --subbuf-size 8M -u u0
lttng enable-event sched_switch,irq_handler_entry,irq_handler_exit,irq_softirq_entr,irq_softirq_exit -k -c k0

lttng enable-event osd:ShardedOpWQ__enqueue_begin,osd:ShardedOpWQ__enqueue_end,osd:ShardedOpWQ_pq_enqueue_begin,osd:ShardedOpWQ_pq_enqueue_end,osd:ShardedOpWQ_pq_enqueue_strict_begin,osd:ShardedOpWQ_pq_enqueue_strict_end -u -c u0
lttng enable-event common:bl_append -u -c u0
lttng add-context -u -t perf:thread:instructions -t perf:thread:cycles -t perf:thread:L1-dcache-load-misses -t perf:thread:dTLB-load-misses -c u0
#lttng add-context -k -t procname -c c0

lttng start
lttng start; sleep 0.5; lttng stop

