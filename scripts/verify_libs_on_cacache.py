#!/usr/bin/env python

# This script checks to ensure each referenced shared library object file in the registry .json files
# are already present in the cacache disk
# It is meant to be used in a Repl

import os
import json

entries = os.listdir('registry')
for entry in entries:
  with open('registry/%s' % entry) as f:
    registry = json.load(f)
    for key, libs in registry.items():
      for lib in libs:
        cacache_path = '/mnt/cacache%s' % lib
        st = os.stat(cacache_path)
        if st:
          print('OK %s' % cacache_path)