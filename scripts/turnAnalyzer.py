#!/usr/bin/env python3

import sys
import glob
import csv

OUTPUT_FILE = 'synctable.csv'



if __name__ == "__main__":
  path = sys.argv[1]
  pid = sys.argv[2]

  turn_to_thread = {}
  
  logfiles = {}

  for filename in glob.glob(path + "/tid-" + pid + "-*.txt"):
    tid = int(filename.split('.')[0].split('-')[-1])
    logfiles[tid] = open(filename, 'r')
    logfiles[tid].readline()

  num_threads = len(logfiles)
  print("Working with %d threads."%(num_threads))

  last_turn = [(-1, '')] * num_threads

  with open(path + "/serializer.log") as sfile, open(OUTPUT_FILE, 'w') as outfile:
    csvwriter = csv.writer(outfile);

    csvwriter.writerow(['Turn'] + list(range(num_threads)))
    
    for line in sfile:
      # print(line)
      row = [''] * (num_threads + 1)
      tid, turn = [int(x) for x in line.split(' ')]
      # print ("Turn %d Tid %d"%(turn, tid))
      row[0] = turn

      while last_turn[tid][0] < turn:
        s_logline = logfiles[tid].readline().split(' ')
        if len(s_logline) > 3:
          last_turn[tid] = (int(s_logline[3]), s_logline[1])
        else:
          last_turn[tid] = (turn, 'unknown')
          break
      
      if last_turn[tid][0] == turn:
        row[tid+1] = last_turn[tid][1]
      else:
        row[tid+1] = 'blocked'

      csvwriter.writerow(row)
        
      
