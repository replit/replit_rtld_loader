import subprocess
import os

repl_home = os.getenv('REPL_HOME')
rtld_env = {
  "LD_AUDIT": "%s/rtld_loader.so" % repl_home,
  "REPLIT_RTLD_LOG_LEVEL": "2"
}
python_23_11 = '/nix/store/2miairdnqbrsjlcllj2vypnvmk2k9z6j-python3-3.10.13'
python_22_11 = '/nix/store/hd4cc9rh83j291r5539hkf6qd8lgiikb-python3-3.10.8'
cairo_23_11 = '/nix/store/b6b71vq4ahf6habxzk8ajkizq0f3hiln-cairo-1.18.0/lib/libcairo.so'
cairo_22_11 = '/nix/store/72mqxxhh3cy5yifh92wh2yb8cl2hikyy-cairo-1.16.0/lib/libcairo.so'
ace = '/nix/store/mk39yjfi7n0z9qy0wv56pg696y1aj3d8-ace-7.0.8/'

subprocess.run(['nix-store', '-r', python_23_11], stderr=subprocess.PIPE, stdout=subprocess.PIPE)
subprocess.run(['nix-store', '-r', python_22_11], stderr=subprocess.PIPE, stdout=subprocess.PIPE)
subprocess.run(['nix-store', '-r', ace], stderr=subprocess.PIPE, stdout=subprocess.PIPE)

def test_cairo_23_11():
  code = "import ctypes; print(ctypes.cdll.LoadLibrary('libcairo.so'))"
  
  output = subprocess.check_output(
    ["%s/bin/python" % python_23_11, '-c', code],
    env = rtld_env
  )
  
  assert str(output, 'UTF-8').startswith("<CDLL 'libcairo.so'")
  
  with open("%s/rtld_loader.log" % repl_home) as f:
    log_content = f.read()
    assert ('found statically: %s' % cairo_23_11) in log_content

  print('OK test_cairo_23_11')

def test_cairo_22_11():
  code = "import ctypes; print(ctypes.cdll.LoadLibrary('libcairo.so'))"

  output = subprocess.check_output(
    ["%s/bin/python" % python_22_11, '-c', code],
    env = rtld_env
  )

  assert str(output, 'UTF-8').startswith("<CDLL 'libcairo.so'")

  with open("%s/rtld_loader.log" % repl_home) as f:
    log_content = f.read()
    assert ('found statically: %s' % cairo_22_11) in log_content

  print('OK test_cairo_22_11')

def test_ace_dynamic():
  code = "import ctypes; print(ctypes.cdll.LoadLibrary('libACE_ETCL.so'))"

  env = rtld_env.copy()
  env.update({
    'REPLIT_LD_LIBRARY_PATH': '%s/lib' % ace
  })
  output = subprocess.check_output(
    ["%s/bin/python" % python_22_11, '-c', code],
    env = env
  )
  
  assert str(output, 'UTF-8').startswith("<CDLL 'libACE_ETCL.so'")
  print('OK test_ace_dynamic')

test_cairo_23_11()
test_cairo_22_11()
test_ace_dynamic()