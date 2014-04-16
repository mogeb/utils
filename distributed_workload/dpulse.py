from fabric.api import *
from fabric.context_managers import settings
from fabric import contrib
import time
import os
import sys
import threading

# TODO use environment variable to define sleep values: fab --set sleep-value=4 ....

env.use_ssh_config = True
env.roledefs ={
  #'vms': ['jessie', 'jessieclone'],
  'vms': ['jessie'],
  'host': [ '127.0.0.1' ]
}

# Global variables and their default values
g_sleep_value = 5
g_guest_wl = 'wk-pulse'
g_host_wl= 'sleep'

# Global general variables
g_tracing_command = 'lttng-simple -e vmsync -c -k --stateless -- '
g_local_trace_dir = time.strftime("%m_%d__%H_%M") + "/"

'''
'workload-name': {
    'command': 'The shell command to be executed',
    'argument': 'The arguments of previously defined command, all as one string',
    'trace_name': 'The name of the trace as defined by lttng-simple. Usually command-k for kernel traces',
    'trace_folder': 'The remote directory that contains previously defined trace_name, as a relative path from the remote system's $HOME',
    'description': 'Optional description'
'''
workloads = {
  'sleep': {
    'command': 'sleep',
    'argument': str(g_sleep_value),
    'trace_name': 'sleep-k',
    'trace_folder': 'traces/sleep',
    'description': 'Sleeps for ' + str(g_sleep_value) + ' seconds'
  },
  'wk-pulse': {
    'command': 'wk-pulse',
    'argument': '--spin 100000 --frequency $FREQ --thread 1 --duration 5000000 --cpuset 0',
    'trace_name': 'wk-pulse-k',
    'trace_folder': 'traces/pulse',
    'description': 'Pulse from wokload-kit'
  },
  'script': {
    'command': 'bash',
    'argument': 'script',
    'trace_name': 'bash-k',
    'trace_folder': '/home/mogeb/traces/script',
    'description': 'Execute a bash script as the traced command. Script should be in local file named script'
  },
  'testing': {
    'command': 'echo',
    'argument': 'test',
    'trace_name': 'echo-k',
    'trace_folder': '/home/mogeb/traces/testing',
    'description': 'Testing command'
  }
}
  
def usage():
  print('Usage:')
  print('')
  print('fab -f <this file> _function_')
  print('')
  print('Where _function_ can be:')
  print('   available_wk                         Lists available workloads')
  print('   pulse_wrapper:guest_work,host_work   Where guest_work and host_work are workloads from the available workloads')
  print('                                        Default values are: guest_work = wk-pulse and host_work = sleep')


def available_wk():
  print(remote_script)
  print('Available workloads:')
  print('')
  for name, opts in workloads.items():
    print(name + ': ')
    print('   ' + opts['description'])
    print('')


def pulse_wrapper(guest_work = 'wk-pulse', host_work = 'sleep'):
  global g_guest_wl
  g_guest_wl = guest_work
  global g_host_wl
  g_host_wl = host_work
  
  if not workloads.has_key(g_host_wl) or not workloads.has_key(g_guest_wl):
    print('Error: Workload not defined.')
    available_wk()
    sys.exit(0)
  
  t = threading.Thread(name='local_trace', target=local_trace)
  t.start()
  execute(pulse, role='vms')
  t.join()


''' Local function '''
@parallel
def local_trace():
  if not os.path.exists(g_local_trace_dir):
    os.mkdir(g_local_trace_dir)
  
  if g_host_wl == 'script':
    local('cp script ' + g_local_trace_dir)
    
  os.chdir(g_local_trace_dir)
  local_command = g_tracing_command + workloads[g_host_wl]['command'] + ' ' + workloads[g_host_wl]['argument']
  local(local_command)

''' Remote function '''
@parallel
def pulse():
  remote_command = g_tracing_command + workloads[g_guest_wl]['command'] + ' ' + workloads[g_guest_wl]['argument']
  trace_folder = workloads[g_guest_wl]['trace_folder']

  if not contrib.files.exists(trace_folder, False, True):
    run('mkdir ' + trace_folder)
  
  with cd(trace_folder):
    if g_guest_wl == 'script':
      put('script', 'script')
    
    if(env.host_string == 'jessie'):
      run(remote_command.replace('$FREQ', '4'))
    elif(env.host_string == 'jessieclone'):
      run(remote_command.replace('$FREQ', '2'))
  copy()


@parallel
def copy():
  #                             DATE_HOUR / workload-vmname-k
  local_target_name = g_local_trace_dir + g_guest_wl + '-' + env.host_string + '-k'
  remote_trace_name = workloads[g_guest_wl]['trace_folder'] + '/' + workloads[g_guest_wl]['trace_name']
  get(remote_trace_name, local_target_name)



if __name__ == '__main__':
  usage()





