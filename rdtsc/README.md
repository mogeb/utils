Function Instrumentation
========================
Ceph supports instrumentation using GCC's `-finstrument-functions` flag.
Supported CMake flags are:

*   `-DWITH_OSD_INSTRUMENT_FUNCTIONS=ON`: instrument OSD code

Note that this instrumentation adds an extra function call on each function entry
and exit of Ceph code.

The only function tracing implementation at the moment is done using LTTng UST.
In order to use it, Ceph needs to be configured with LTTng using `-DWITH_LTTNG=ON`.
[TraceCompass](http://www.tracecompass.org) can be used as a complement to to generate flame charts/graphs and
other metrics.

It is also possible to use [libbabeltrace](http://diamon.org/babeltrace/#docs) to write custom analysis. The entry and
exit tracepoints are called `lttng_ust_cyg_profile:func_enter` and
`lttng_ust_cyg_profile:func_exit` respectively. The payload variable
`addr` holds the address of the function called and the payload variable
`call_site` holds the address where it is called. `nm` can be used to resolve
function addresses (`addr` to function name).
