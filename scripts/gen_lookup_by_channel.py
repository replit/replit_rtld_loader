#!/usr/bin/env python

# This script generates src/lookup_by_channel.generated.[ch] files.
# The generated files provide a lookup_by_channel(nix_channel, libname) function
# that resolves libaries based on a list of known Nix packages for each supported
# Nix channel. It converts the .json files in the registry directory into
# pre-sorted static arrays of lib_entry structs.
# The lookup implementation uses binary search on those arrays.

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
    lines.append('const static struct lib_entry %s[] = {' % array_name )
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
  lines.append('const char *lookup_by_channel(int nix_channel, const char *libname) {')
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

def gen_nix_channel_str_fn(lines):
  lines.append('')
  lines.append('char *nix_channel_str(int channel) {')
  lines.append('  switch (channel) {')
  entries = os.listdir('registry')
  for entry in entries:
    with open('registry/%s' % entry) as f:
      channel = strip_ext(entry)
      const_name = channel_const_name(channel)
      lines.append('    case %s:' % const_name)
      lines.append('    return "%s";' % channel)
      lines.append('    break;')
  lines.append('    default:')
  lines.append('    return "invalid";')
  lines.append('  }')
  lines.append('}')

def gen_lib_entry_arrays(lines):
  entries = os.listdir('registry')
  for entry in entries:
    array_name = 'lib_entries_%s' % strip_ext(entry).replace('-', '_')
    gen_static_array_for('registry/%s' % entry, array_name, lines)
    lines.append('};')
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
  gen_nix_channel_str_fn(lines)
  
  with open('src/lookup_by_channel.generated.c', 'w') as f:
    f.write("\n".join(lines))

def gen_h_file():
  lines = [
    '/*',
    'This is an auto-generated file via scripts/gen_lookup_by_channel.py',
    '*/',
    'enum nix_channels {',
    '  CHANNEL_UNKNOWN,',
  ]
  entries = os.listdir('registry')
  for entry in entries:
    const_name = channel_const_name(strip_ext(entry))
    lines.append('  %s,' % const_name)
  lines.append('};')
  lines.append('')
  lines.append('// Find the Nix channel based on the absolute path of libc.so. Return CHANNEL_UNKNOWN if no match.')
  lines.append('int get_nix_channel(const char *libpath);')
  lines.append('')
  lines.append('// Lookup a library based a list of known libraries from Nix')
  lines.append('// * nix_channel - the Nix channel to search')
  lines.append('// * libname - the name of the library')
  lines.append('// Return value')
  lines.append('//   string containing absolute path of the shared object file. NULL if not found.')
  lines.append('const char *lookup_by_channel(int nix_channel, const char *libname);')
  lines.append('')
  
  lines.append('// Get a display string for a Nix channel')
  lines.append('char *nix_channel_str(int channel);')
  with open('src/lookup_by_channel.generated.h', 'w') as f:
    f.write("\n".join(lines))

if __name__ == '__main__':
  gen_c_file()
  gen_h_file()