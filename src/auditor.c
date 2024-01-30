#include "auditor.h"

static int nix_channel = CHANNEL_UNKNOWN;
static int audit_log_fd;
static char replit_ld_library_path[MAX_LD_LIBRARY_PATH_LENGTH] = {0};
static char search_result[MAX_PATH_LENGTH] = {0};

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
  audit_log_fd = sys_open("audit.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
}

char *dynamic_lookup(const char *libname) {
  int libname_len = my_strlen(libname);
  const char *rllp = replit_ld_library_path;
  const char *next_rllp = rllp;
  while (*next_rllp) {
    rllp = next_rllp;
    next_rllp = my_strchrnul(rllp, ':');

    size_t rllp_len = next_rllp - rllp;
    if (*next_rllp) {
      next_rllp++;
    }

    char current_lib_path[MAX_PATH_LENGTH] = {0};
    if (rllp_len + libname_len + 2 > sizeof(current_lib_path)) {
      // We need the lib_path to be able to fit the path, the separating slash,
      // the filename, and the null terminator.
      continue;
    }
    // TODO: check for path too long for buffer
    my_strncpy(current_lib_path, rllp, rllp_len);
    char *lib_path_suffix = current_lib_path + rllp_len;
    lib_path_suffix[0] = '/';
    my_strncpy(lib_path_suffix + 1, libname, libname_len);
    int lib_path_len = rllp_len + 1 + libname_len;
    lib_path_suffix[1 + libname_len] = '\0';

    log_write("  trying ");
    log_write(current_lib_path);
    log_write("\n");
    // check if we can read the file (i.e. it exists and we can read it).
    if (sys_access(current_lib_path, R_OK) == 0) {
      my_strncpy(search_result, current_lib_path, lib_path_len);
      search_result[lib_path_len] = '\0';
      return search_result;
    }
  }

  return NULL;
}

__attribute__((constructor))
static void init(void) {
  parse_env(replit_ld_library_path, MAX_LD_LIBRARY_PATH_LENGTH);
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
        result = dynamic_lookup(libname);
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

char *nix_channel_str(int channel) {
  switch (nix_channel) {
    case CHANNEL_STABLE_23_11:
    return "stable-23_11";
    break;
    case CHANNEL_STABLE_23_05:
    return "stable-23_05";
    break;
    case CHANNEL_STABLE_22_11:
    return "stable-22_11";
    break;
    case CHANNEL_STABLE_22_05:
    return "stable-22_05";
    break;
    default:
    return "invalid";
  }
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