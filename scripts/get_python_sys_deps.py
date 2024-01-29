#!/usr/bin/env python

import json

f = open('upm/internal/nix/python_map.json', 'r')
data = json.load(f)
# print(data)
f.close()

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