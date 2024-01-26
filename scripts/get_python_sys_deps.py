import json

f = open('upm/internal/nix/python_map.json', 'r')
data = json.load(f)
# print(data)
f.close()

deps = set()
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