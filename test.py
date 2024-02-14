import subprocess
import os
import json

channels = [
  "stable-22_05",
  "stable-22_11",
  "stable-23_05",
  "stable-23_11"
]

channel_to_python = {
  "stable-22_05": "/nix/store/xrs3m0ymsscgsb4k5r1d5x7rc9sg42b6-python3-3.10.4/bin/python",
  "stable-22_11": "/nix/store/hd4cc9rh83j291r5539hkf6qd8lgiikb-python3-3.10.8/bin/python",
  "stable-23_05": "/nix/store/xf54733x4chbawkh1qvy9i1i4mlscy1c-python3-3.10.11/bin/python",
  "stable-23_11": "/nix/store/2miairdnqbrsjlcllj2vypnvmk2k9z6j-python3-3.10.13/bin/python"
}

channel_to_enum = {
  "stable-22_05": 1,
  "stable-22_11": 2,
  "stable-23_05": 3,
  "stable-23_11": 4
}

pwd = os.getcwd()
def test_lib(libname, verbose=False):
  print(libname)
  hypothesis_holds = True
  bad_so = False
  results = {}
  for py_chan in channels:
    for so_chan in channels:
      code = "import ctypes; print(ctypes.cdll.LoadLibrary('%s'))" % libname
      python = channel_to_python[py_chan]
      env = {
        "LD_AUDIT": "%s/rtld_loader.so" % pwd,
        "REPLIT_RTLD_NIX_CHANNEL": str(channel_to_enum[so_chan])
      }
      proc = subprocess.run([python, "-c", code], env = env, capture_output=True)
      if proc.returncode == 0:
        if verbose:
          print("  py_chan=%s, so_chan=%s: ok" % (py_chan, so_chan))
        results[(py_chan, so_chan)] = True
      else:
        results[(py_chan, so_chan)] = False
        display = 'error'
        if py_chan >= so_chan:
          if results[(so_chan, so_chan)]:
            display = 'unexpected error'
            hypothesis_holds = False
          else:
            bad_so = True
        if verbose:
          print("  py_chan=%s, so_chan=%s: %s" % (py_chan, so_chan, display))
  if not bad_so:
    print('  good')
  return hypothesis_holds

def long_test():
  with open("registry/stable-23_11.json", "r") as f:
    dct = json.load(f)
    libs = dct.keys()
    for lib in libs:
      result = test_lib(lib)
      if not result:
        raise Exception("Hypothesis failed")

long_test()
# test_lib("libFLAC++.so.10", True)