#!/usr/bin/env python

import json
import subprocess

python_map_url = 'https://raw.githubusercontent.com/replit/upm/main/internal/nix/python_map.json'

output = subprocess.check_output(['curl', python_map_url])
data = json.loads(output)

deps = set()

deps.add('pkgs.xorg.libX11')
deps.add('pkgs.xorg.libXcursor')
deps.add('pkgs.xorg.libXinerama')
deps.add('pkgs.xorg.libXext')
deps.add('pkgs.xorg.libXrandr')
deps.add('pkgs.xorg.libXi')
deps.add('pkgs.xorg.libXxf86vm')

deps.add('pkgs.stdenv.cc.cc.lib')
deps.add('pkgs.zlib')

for key, value in data.items():
  for dep in value["deps"]:
    deps.add(dep)


output = """
{pkgs}: {
  deps = [
    """ + "\n    ".join(deps) + """
  ];
}"""

with open("replit.nix", "w") as f:
  f.write(output)
  print('Wrote replit.nix')