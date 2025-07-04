/*
Entrypoint of the rtld loader. It uses the rtld-audit API provided by Linux:
https://man7.org/linux/man-pages/man7/rtld-audit.7.html
*/

#define _GNU_SOURCE

#include <fcntl.h>
#include <link.h>
#include <stdint.h>
#include <unistd.h>
#include "consts.h"
#include "dynamic_lookup.h"
#include "env_parser.h"
#include "logging.h"
#include "string_funs.h"
#include "syscalls.h"

static char replit_ld_library_path[MAX_LD_LIBRARY_PATH_LENGTH] = {0};

__attribute__((constructor)) static void init(void) {
  int log_level;
  int fd = sys_open("/proc/self/environ", O_RDONLY, 0);
  parse_env(fd, replit_ld_library_path, &log_level);
  sys_close(fd);
  log_init(log_level);
}

unsigned int la_version(unsigned int version) {
  log_info("RTLD: la_version(");
  log_info_int(version);
  log_info(")\n");
  return version;
}

char* la_objsearch(const char* name, uintptr_t* cookie, unsigned int flag) {
  log_debug("la_objsearch(");
  log_debug(name);
  log_debug(", ");
  log_debug_int(flag);
  log_debug(")\n");
  if (flag == LA_SER_DEFAULT) {
    char* libname = my_strrchr(name, '/');
    if (libname != NULL) {
      libname++;  // advance past the /
      log_info("ld library miss for ");
      log_info(libname);
      log_info("\n  searching...\n");
      const char* result = NULL;
      if (result == NULL) {
        const char* search_path = replit_ld_library_path;
        if (strstartswith("/nix/store/", name)) {
          // Count slashes to find the 5th one (after /nix/store/hash-name/lib/)
          int slash_count = 0;
          int i;
          for (i = 0; name[i] && slash_count < 5; i++) {
            if (name[i] == '/') slash_count++;
          }
          // Check if it's actually a /lib/ directory
            if (i >= 5 && i <= MAX_LD_LIBRARY_PATH_LENGTH && strneql(name + i - 5, "/lib/", 5)) {
            char extended_path[MAX_LD_LIBRARY_PATH_LENGTH];
            my_strncpy(extended_path, name, i);
            extended_path[i - 1] = ':';
            my_strncpy(extended_path + i, replit_ld_library_path, MAX_LD_LIBRARY_PATH_LENGTH - i - 1);
            search_path = extended_path;
          }
        }
        result = dynamic_lookup(libname, search_path);
        if (result != NULL) {
          log_info("  found dynamically: ");
          log_info(result);
          log_info("\n");
          return (char*)result;
        }
      }
      log_info("  not found.\n");
    }
  }
  return (char*)name;
}

unsigned int la_objopen(struct link_map* map, Lmid_t lmid, uintptr_t* cookie) {
  log_debug("la_objopen(");
  log_debug(map->l_name);
  log_debug(")\n");
  return 0;
}

void la_preinit(uintptr_t* cookie) {
  log_info("la_preinit()\n");
}
