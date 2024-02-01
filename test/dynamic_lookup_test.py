import subprocess

# Fake lib directories base dir
basedir = "/tmp/dynamic_lookup_test"

subprocess.run(['rm', '-fr', basedir])
subprocess.run(['mkdir', '-p', basedir + '/cairo/lib'])
subprocess.run(['mkdir', '-p', basedir + '/libc++/lib'])
subprocess.run(['touch', basedir + '/cairo/lib/libcairo.so'])
subprocess.run(['touch', basedir + '/cairo/lib/libcairo.so.2'])
subprocess.run(['touch', basedir + '/libc++/lib/libc++.so'])
subprocess.run(['touch', basedir + '/libc++/lib/libc++.so.1.0'])

ld_library_path = '%s/cairo/lib:%s/libc++/lib' % (basedir, basedir)

output = subprocess.check_output(['./dynamic_lookup_test.bin', 'libcairo.so', ld_library_path])
assert str(output, 'UTF-8') == '%s/cairo/lib/libcairo.so' % basedir

output = subprocess.check_output(['./dynamic_lookup_test.bin', 'libcairo.so.2', ld_library_path])
assert str(output, 'UTF-8') == '%s/cairo/lib/libcairo.so.2' % basedir

output = subprocess.check_output(['./dynamic_lookup_test.bin', 'libc++.so', ld_library_path])
assert str(output, 'UTF-8') == '%s/libc++/lib/libc++.so' % basedir

output = subprocess.check_output(['./dynamic_lookup_test.bin', 'libc++.so.1.0', ld_library_path])
assert str(output, 'UTF-8') == '%s/libc++/lib/libc++.so.1.0' % basedir

output = subprocess.check_output(['./dynamic_lookup_test.bin', 'blargh.so', ld_library_path])
assert str(output, 'UTF-8') == ''

output = subprocess.check_output(['./dynamic_lookup_test.bin', 'libcairo.so', ''])
assert str(output, 'UTF-8') == ''

print('OK')
