#include "syscalls.h"
#include <fcntl.h>
#include "string_funs.h"
#include "logging.h"

static int audit_log_fd = -1;
static int log_level;

void fprint(int fd, const char *message) {
  int len = my_strlen(message);
  sys_write(fd, message, len);
}

void fprint_int(int fd, int num) {
  char int_str[15];
  int len = itoa(num, int_str, 10);
  sys_write(fd, int_str, len);
}

void log_init(int ll) {
  log_level = ll;
  if (log_level > 0) {
    audit_log_fd = sys_open("rtld_loader.log", O_WRONLY | O_CREAT | O_TRUNC, 0644);
  }
}

void log_write(const char *message, int level) {
  if (audit_log_fd == -1) {
    return;
  }
  if (level > log_level) {
    return;
  }
  fprint(audit_log_fd, message);
}

void log_write_int(int num, int level) {
  if (audit_log_fd == -1) {
    return;
  }
  if (level > log_level) {
    return;
  }
  fprint_int(audit_log_fd, num);
}

void log_info(const char *message) {
  log_write(message, INFO);
}

void log_info_int(int num) {
  log_write_int(num, INFO);
}

void log_debug(const char *message) {
  log_write(message, DEBUG);
}

void log_debug_int(int num) {
  log_write_int(num, DEBUG);
}

void log_warn(const char *message) {
  log_write(message, WARN);
}

void log_warn_int(int num) {
  log_write_int(num, WARN);
}