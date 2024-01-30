#include "rtld_loader.h"

static int nix_channel = CHANNEL_UNKNOWN;
static int audit_log_fd;
static char replit_ld_library_path[MAX_LD_LIBRARY_PATH_LENGTH] = {0};

void fprint(int fd, const char *message) {
  int len = my_strlen(message);
  sys_write(fd, message, len);
}

void fprint_int(int fd, int num) {
  char int_str[15];
  int len = itoa(num, int_str, 10);
  sys_write(fd, int_str, len);
}

void print(const char *message) {
  fprint(1, message);
}

void log_write(const char *message) {
  fprint(audit_log_fd, message);
}

void log_write_int(int num) {
  fprint_int(audit_log_fd, num);
}

void log_init() {
  audit_log_fd = sys_open("rtld_loader.log", O_WRONLY | O_CREAT | O_TRUNC, 0644);
}

__attribute__((constructor))
static void init(void) {
  int fd = sys_open("/proc/self/environ", O_RDONLY, 0);
  parse_env(fd, replit_ld_library_path);
  sys_close(fd);
  log_init();
}

unsigned int la_version(unsigned int version) {
  log_write("RTLD: la_version(");
  log_write_int(version);
  log_write(")\n");
  return version;
}

char * la_objsearch(const char *name, uintptr_t *cookie, unsigned int flag) {
  // log_write("la_objsearch(");
  // log_write(name);
  // log_write(", ");
  // log_write_int(flag);
  // log_write(")\n");
  if (flag == LA_SER_DEFAULT) {
    char *libname = my_strrchr(name, '/');
    if (libname != NULL) {
      libname++; // advance past the /
      log_write("ld library miss for ");
      log_write(libname);
      log_write("\n  searching...\n");
      char *result = NULL;
      if (nix_channel != CHANNEL_UNKNOWN) {
        result = lookup_by_channel(nix_channel, libname);
        if (result != NULL) {
          log_write("  found statically: ");
          log_write(result);
          log_write("\n");
          return result;
        }
      }
      if (result == NULL) {
        result = dynamic_lookup(libname, replit_ld_library_path);
        if (result != NULL) {
          log_write("  found dynamically: ");
          log_write(result);
          log_write("\n");
          return result;
        }
      }
      log_write("  not found.\n");
    }
  }
  return (char *)name;
}

unsigned int la_objopen(struct link_map *map, Lmid_t lmid,
 uintptr_t *cookie) {
  // log_write("la_objopen(");
  // log_write(map->l_name);
  // log_write(")\n");
  if (nix_channel == CHANNEL_UNKNOWN) {
    nix_channel = get_nix_channel(map->l_name);
    if (nix_channel != CHANNEL_UNKNOWN) {
      log_write("Found Nix channel: ");
      log_write(nix_channel_str(nix_channel));
      log_write("\n");
    }
  }
  return 0;
}

void la_preinit(uintptr_t *cookie) {
  log_write("la_preinit()\n");
}