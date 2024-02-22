#include "logging.h"
#include <fcntl.h>
#include "string_funs.h"
#include "syscalls.h"

#define LOG_FILE_PREFIX "rtld_loader.log."
#define LOG_FILE_PREFIX_LENGTH 16

static int audit_log_fd = -1;
static int log_level;

void fprint(int fd, const char* message) {
  int len = my_strlen(message);
  sys_write(fd, message, len);
}

void fprint_int(int fd, int num) {
  char int_str[MAX_DECIMAL_INT_LEN];
  int len = itoa(num, int_str, 10);
  sys_write(fd, int_str, len);
}

// For troubleshooting purposes: output contents of /proc/self/cmdline to the
// log file
void _output_cmdline() {
  char buf[1024];
  if (log_level < INFO) {
    return;
  }
  log_info("cmdline: ");
  int cmdline_fd = sys_open("/proc/self/cmdline", O_RDONLY, 0);
  while (1) {
    int bytes = sys_read(cmdline_fd, buf, sizeof(buf));
    if (bytes <= 0) {
      break;
    }
    sys_write(audit_log_fd, buf, bytes);
  }
  log_info("\n");
}

void log_init(int ll) {
  log_level = ll;
  if (log_level > 0) {
    // Generated a log file name with a numeric suffix, i.e. rtld_loader.log.1
    // if the file already exists, increment the suffix
    char log_filename[LOG_FILE_PREFIX_LENGTH + MAX_DECIMAL_INT_LEN + 1];
    my_strncpy(log_filename, LOG_FILE_PREFIX, LOG_FILE_PREFIX_LENGTH);
    int suffix = 1;
    while (1) {
      int suffix_len = itoa(suffix, log_filename + LOG_FILE_PREFIX_LENGTH, 10);
      log_filename[LOG_FILE_PREFIX_LENGTH + suffix_len] = '\0';
      if (sys_access(log_filename, R_OK) == 0) {
        // file already exists, try the next suffix
        suffix++;
      } else {
        // we can use this one
        audit_log_fd =
            sys_open(log_filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        break;
      }
    }

    _output_cmdline();
  }
}

void log_write(const char* message, int level) {
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

void log_info(const char* message) {
  log_write(message, INFO);
}

void log_info_int(int num) {
  log_write_int(num, INFO);
}

void log_debug(const char* message) {
  log_write(message, DEBUG);
}

void log_debug_int(int num) {
  log_write_int(num, DEBUG);
}

void log_warn(const char* message) {
  log_write(message, WARN);
}

void log_warn_int(int num) {
  log_write_int(num, WARN);
}
