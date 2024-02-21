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
  subprocess.run(["bash", "-c", "rm rtld_loader.log.*"])
  code = "import ctypes; print(ctypes.cdll.LoadLibrary('libACE_ETCL.so'))"

  env = rtld_env.copy()
  env.update({
    'REPLIT_LD_LIBRARY_PATH': '%s/lib' % ace
  })
  output = subprocess.check_output(
    ["%s/bin/python" % python_23_11, '-c', code],
    env = env
  )

  print(str(output, 'UTF-8'))
  assert str(output, 'UTF-8').startswith("<CDLL 'libACE_ETCL.so'")
  print('OK test_ace_dynamic')

def test_multiple_logs_for_subprocess():
  subprocess.run(["bash", "-c", "rm rtld_loader.log.*"])
  code = "import subprocess; subprocess.run(['echo', 'hello, world!'])"
  output = subprocess.check_output(
    ["%s/bin/python" % python_23_11, '-c', code],
    env = {
      "LD_AUDIT": "%s/rtld_loader.so" % pwd,
      "REPLIT_RTLD_LOG_LEVEL": "3"
    }
  )
  assert os.path.exists("rtld_loader.log.1")
  assert os.path.exists("rtld_loader.log.2")
  with open("rtld_loader.log.1", "r") as f:
    content = f.read()
    assert "/bin/python" in content
  with open("rtld_loader.log.2", "r") as f:
    content = f.read()
    assert "echo" in content
    assert "hello, world!" in content
  print('OK test_multiple_logs_for_subprocess')

test_ace_dynamic()
test_multiple_logs_for_subprocess()