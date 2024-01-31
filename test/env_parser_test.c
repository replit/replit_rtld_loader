#include <stdio.h>
#include <assert.h>
#include <fcntl.h>
#include "syscalls.h"
#include "string_funs.h"
#include "env_parser.h"
#include "consts.h"
#include "logging.h"

void test0() {
  char replit_ld_library_path[MAX_LD_LIBRARY_PATH_LENGTH] = "BLARGH";
  int log_level = -1;
  int fd = sys_open("/tmp/fake_environ.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
  sys_close(fd);

  fd = sys_open("/tmp/fake_environ.txt", O_RDONLY, 0);
  parse_env(fd, (char *)&replit_ld_library_path, &log_level);
  assert(streql(replit_ld_library_path, ""));
  assert(0 == log_level);
}

void test1() {
  char replit_ld_library_path[MAX_LD_LIBRARY_PATH_LENGTH] = {0};
  int log_level = -1;
  char *content = "BLAH=FOOBAR\0REPLIT_LD_LIBRARY_PATH=/path/one:/path/two\0";
  int fd = sys_open("/tmp/fake_environ.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
  sys_write(fd, content, 55);
  sys_close(fd);

  fd = sys_open("/tmp/fake_environ.txt", O_RDONLY, 0);
  parse_env(fd, (char *)&replit_ld_library_path, &log_level);
  assert(streql(replit_ld_library_path, "/path/one:/path/two"));
  assert(0 == log_level);
}

void test2() {
  char replit_ld_library_path[MAX_LD_LIBRARY_PATH_LENGTH] = {0};
  int log_level = -1;
  char *content = "BLAH=FOOBAR\0REPLIT_RTLD_LOG_LEVEL=1\0";
  int fd = sys_open("/tmp/fake_environ.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
  sys_write(fd, content, 36);
  sys_close(fd);

  fd = sys_open("/tmp/fake_environ.txt", O_RDONLY, 0);
  parse_env(fd, (char *)&replit_ld_library_path, &log_level);
  assert(streql(replit_ld_library_path, ""));
  assert(1 == log_level);
}

void test3() {
  char replit_ld_library_path[MAX_LD_LIBRARY_PATH_LENGTH] = {0};
  int log_level = -1;
  char *content = "BLAH=FOOBAR\0REPLIT_RTLD_LOG_LEVEL=2\0";
  int fd = sys_open("/tmp/fake_environ.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
  sys_write(fd, content, 36);
  sys_close(fd);

  fd = sys_open("/tmp/fake_environ.txt", O_RDONLY, 0);
  parse_env(fd, (char *)&replit_ld_library_path, &log_level);
  assert(streql(replit_ld_library_path, ""));
  assert(2 == log_level);
}

void test4() {
  char replit_ld_library_path[MAX_LD_LIBRARY_PATH_LENGTH] = {0};
  int log_level = -1;
  char *content = "BLAH=FOOBAR\0REPLIT_LD_LIBRARY_PATH=/path/one:/path/two\0REPLIT_RTLD_LOG_LEVEL=1";
  int fd = sys_open("/tmp/fake_environ.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
  sys_write(fd, content, 78);
  sys_close(fd);

  fd = sys_open("/tmp/fake_environ.txt", O_RDONLY, 0);
  parse_env(fd, (char *)&replit_ld_library_path, &log_level);
  assert(streql(replit_ld_library_path, "/path/one:/path/two"));
  assert(1 == log_level);
}

int main() {
  test0();
  test1();
  test2();
  test4();
  printf("OK\n");
}