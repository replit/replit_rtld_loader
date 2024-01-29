#!/usr/bin/env python

import os
import json

def gen_static_array_for(channel_file, array_name, lines):
  with open(channel_file) as f:
    registry = json.load(f)
    entries = sorted(registry.items(), key=lambda e: e[0])
    lines.append('static int %s_length = %d;' % (array_name, len(entries)))
    lines.append('static struct LibEntry %s[] = {' % array_name )
    for key, libs in entries:
      lines.append('  { .libname = "%s", .libpath = "%s" },' % (
        key, libs[0]
      ))

def gen_get_nix_channel_fn(lines):
  lines.append('int get_nix_channel(const char *libpath) {')
  entries = os.listdir('registry')
  for entry in entries:
    with open('registry/%s' % entry) as f:
      registry = json.load(f)
      libc_path = registry['libc.so'][0]
      lines.append('  if (strstartswith("%s")) {' % libc_path)
      channel_const_name = 'CHANNEL_%s' % entry.split('.')[0].replace('-', '_')[7:]
      lines.append('    return %s;' % channel_const_name)
      lines.append('  }')
  lines.append('  return CHANNEL_UNKNOWN;')
  lines.append('}')
  

def gen_lib_entry_arrays(lines):
  entries = os.listdir('registry')
  for entry in entries:
    array_name = 'lib_entries_%s' % entry.split('.')[0].replace('-', '_')
    gen_static_array_for('registry/%s' % entry, array_name, lines)
    lines.append('};');
    lines.append('')

lines = [
  '/*',
  'This is an auto-generated file via scripts/gen_lookup_by_channel.py',
  '*/',
  '#include "string_funs.h"',
  '#include "consts.h"',
  '',
  'struct LibEntry {',
  '  char *libname;',
  '  char *libpath;',
  '};',
  ''
]

gen_lib_entry_arrays(lines)
gen_get_nix_channel_fn(lines)

with open('src/lookup_by_channel.generated.c', 'w') as f:
  f.write("\n".join(lines))
      