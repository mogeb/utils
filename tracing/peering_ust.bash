#!/bin/bash

lttng create -o .
lttng enable-channel --num-subbuf 32 --subbuf-size 8M -u u0

lttng enable-event osd:init_start,osd:init_end,osd:consume_map_start,osd:consume_map_end -u -c u0
lttng add-context -u -t vpid -t vtid -t procname
#lttng add-context -k -t procname -c c0

lttng start
lttng start; sleep 0.5; lttng stop

