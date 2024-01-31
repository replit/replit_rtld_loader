#!/usr/bin/env python

# This script takes a each path element in REPLIT_LD_LIBRARY_PATH, looks for .so
# files within it, and uses them to generate a registry .json for a the Nix channel
# specified in .replit
# These .json files are then checked in.
# This is meant to be used in a Repl

import os
import re
import json

channel_name_re = re.compile(r'channel = "(.*)"')
so_re = re.compile(r'.*\.so(?:$|\.)')
nix_re = re.compile(r'.*\-nix\-[0-9]+\.[0-9]+\.[0-9]+/.*$')
channel = None

with open('.replit') as f:
  for line in f:
    m = channel_name_re.match(line)
    if m:
      channel = m.group(1)

if channel is None:
  print("No channel found")
  exit(1)

paths = os.getenv('REPLIT_LD_LIBRARY_PATH').split(":")

all_sos = {}

for p in paths:
  if nix_re.match(p):
    continue
  st = os.stat(p)
  entries = os.listdir(p)
  for entry in entries:
    if so_re.match(entry):
      if entry not in all_sos:
        all_sos[entry] = []
      else:
        print('WARN: multiple matches for %s' % entry)
      all_sos[entry].append("%s/%s" % (p, entry))

registry_name = "registry/%s.json" % channel
with open(registry_name, "w") as f:
  json.dump(all_sos, f, sort_keys=True, indent=2)
  print("wrote %s" % registry_name)
