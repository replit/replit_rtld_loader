#include "syscalls.h"
#include <fcntl.h>
#include "string_funs.h"
#include "logging.h"

static int audit_log_fd;

void fprint(int fd, const char *message) {
  int len = my_strlen(message);
  sys_write(fd, message, len);
}

void fprint_int(int fd, int num) {
  char int_str[15];
  int len = itoa(num, int_str, 10);
  sys_write(fd, int_str, len);
}

void log_init() {
  audit_log_fd = sys_open("rtld_loader.log", O_WRONLY | O_CREAT | O_TRUNC, 0644);
}

void log_write(const char *message) {
  fprint(audit_log_fd, message);
}

void log_write_int(int num) {
  fprint_int(audit_log_fd, num);
}