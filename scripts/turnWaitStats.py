#!/usr/bin/env python3

import sys
import glob
import csv

OUTPUT_FILE = 'turnWaitStats.txt'
WAITING_SYNCS_FILE = 'waitedSyncs.txt'



if __name__ == "__main__":
  path = sys.argv[1]
  pid = sys.argv[2]
  print_syncs = 0
  if len(sys.argv) > 3:
    print_syncs = int(sys.argv[3])

  turn_to_thread = {}
  
  total_turns = 0
  total_waited = 0
  total_spinlock_timeouts = 0

  # TODO: per thread wait stats

  with open(WAITING_SYNCS_FILE, 'w') as syncsFile:
    for filename in glob.glob(path + "/tid-" + pid + "-*.txt"):
      tid = int(filename.split('.')[0].split('-')[-1])
      with open(filename, 'r') as tid_file:
        tid_file.readline()

        for line in tid_file:
          s_logline = line.split(' ')

          op = s_logline[1]
          insid = s_logline[2]
          turn = int(s_logline[3])
          sched_time_sec, sched_time_nsec = s_logline[6].split(':')
          sched_time_usec = int(sched_time_sec)*1000000 + int(sched_time_nsec)/1000
          wait_type = int(s_logline[8])
          
          total_turns += 1
          if wait_type > 0:
            total_waited += 1
            if wait_type > 1:
              total_spinlock_timeouts += 1

          if wait_type >= print_syncs:
            syncsFile.write('%d,%d,%s,%s,%d,%d\n'%(turn, tid, op, insid, sched_time_usec, wait_type))

  with open(OUTPUT_FILE, 'w') as outfile:
    outfile.write('Total turns: \t\t\t%d\n'%(total_turns))
    outfile.write('Total waited syncs: \t\t%d\n'%(total_waited))
    outfile.write('Total spin-lock timeouts: \t%d\n'%(total_spinlock_timeouts))