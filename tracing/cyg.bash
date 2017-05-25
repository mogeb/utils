#!/bin/bash

lttng create -o .
lttng enable-channel --num-subbuf 32 --subbuf-size 8M -u u0

lttng enable-event lttng_ust_cyg_profile:* -u -c u0
lttng add-context -u -t vpid -t vtid -t procname

lttng start
lttng start; sleep 0.5; lttng stop

