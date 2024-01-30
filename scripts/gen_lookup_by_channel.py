#!/usr/bin/env python

import os
import json

def channel_const_name(channel):
  return 'CHANNEL_' + channel.replace('-', '_').upper()

def strip_ext(filename):
  return filename.split('.')[0]

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
      lines.append('  if (strstartswith("%s", libpath)) {' % libc_path)
      const_name = channel_const_name(strip_ext(entry))
      lines.append('    return %s;' % const_name)
      lines.append('  }')
  lines.append('  return CHANNEL_UNKNOWN;')
  lines.append('}')

def gen_lookup_fn(lines):
  lines.append('')
  lines.append('char *lookup_by_channel(int nix_channel, const char *libname) {')
  entries = os.listdir('registry')
  for entry in entries:
    with open('registry/%s' % entry) as f:
      const_name = channel_const_name(strip_ext(entry))
      array_name = 'lib_entries_%s' % strip_ext(entry).replace('-', '_')
      lines.append('  if (nix_channel == %s) {' % const_name)
      lines.append('    return binsearch_lookup(libname, %s, %s);' % (array_name, array_name + '_length'))
      lines.append('  }')
  lines.append('  return NULL;')
  lines.append('}')

def gen_lib_entry_arrays(lines):
  entries = os.listdir('registry')
  for entry in entries:
    array_name = 'lib_entries_%s' % strip_ext(entry).replace('-', '_')
    gen_static_array_for('registry/%s' % entry, array_name, lines)
    lines.append('};');
    lines.append('')

def gen_c_file():
  lines = [
    '/*',
    'This is an auto-generated file via scripts/gen_lookup_by_channel.py',
    '*/',
    '#include "string_funs.h"',
    '#include "binsearch_lookup.h"',
    '#include "lookup_by_channel.generated.h"',
    ''
    '',
  ]
  
  gen_lib_entry_arrays(lines)
  gen_get_nix_channel_fn(lines)
  gen_lookup_fn(lines)
  
  with open('src/lookup_by_channel.generated.c', 'w') as f:
    f.write("\n".join(lines))

def gen_h_file():
  lines = [
    '/*',
    'This is an auto-generated file via scripts/gen_lookup_by_channel.py',
    '*/',
    '#define CHANNEL_UNKNOWN    0'
  ]
  counter = 1
  entries = os.listdir('registry')
  for entry in entries:
    const_name = channel_const_name(strip_ext(entry))
    lines.append('#define %s    %d' % (const_name, counter))
    counter += 1
  lines.append('')
  lines.append('int get_nix_channel(const char *libpath);')
  lines.append('char *lookup_by_channel(int nix_channel, const char *libname);')
  with open('src/lookup_by_channel.generated.h', 'w') as f:
    f.write("\n".join(lines))

if __name__ == '__main__':
  gen_c_file()
  gen_h_file()