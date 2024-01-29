#!/usr/bin/env python

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