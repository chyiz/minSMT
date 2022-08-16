#!/usr/bin/env python3
import sys
import glob
import csv

OUTPUT_FILE = 'syncs.time'

if __name__ == "__main__":
  path = sys.argv[1]

  db = {}

  for filename in glob.glob(path + "/tid-*.txt"):
    with open(filename) as infile:
      csvreader = csv.DictReader(infile, skipinitialspace=True)
      last_caller = "0"
      for row in csvreader:
        pair = (last_caller, row['eip'])
        
        if pair not in db:
          db[pair] = []
        db[pair].append(int(row['app_time']))

        last_caller = row['eip']
  
  # After processing all files
  with open(OUTPUT_FILE, 'w') as outfile:
    for key in db:
      outfile.write("%s|%s %d %d %d\n"%(key[0], key[1], max(db[key]), len(db[key]), sum(db[key])/len(db[key])))

