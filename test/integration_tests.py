import subprocess
import os

pwd = os.getcwd()
rtld_env = {
  "LD_AUDIT": "%s/rtld_loader.so" % pwd,
  "REPLIT_RTLD_LOG_LEVEL": "2"
}
python_23_11 = '/nix/store/2miairdnqbrsjlcllj2vypnvmk2k9z6j-python3-3.10.13'
ace = '/nix/store/mk39yjfi7n0z9qy0wv56pg696y1aj3d8-ace-7.0.8/'

def realize(pkgpath):
  subprocess.run(['nix-store', '-r', pkgpath], stderr=subprocess.PIPE, stdout=subprocess.PIPE)

realize(python_23_11)
realize(ace)

def test_ace_dynamic():
  code = "import ctypes; print(ctypes.cdll.LoadLibrary('libACE_ETCL.so'))"

  env = rtld_env.copy()
  env.update({
    'REPLIT_LD_LIBRARY_PATH': '%s/lib' % ace
  })
  output = subprocess.check_output(
    ["%s/bin/python" % python_23_11, '-c', code],
    env = env
  )
  
  assert str(output, 'UTF-8').startswith("<CDLL 'libACE_ETCL.so'")
  print('OK test_ace_dynamic')

test_ace_dynamic()