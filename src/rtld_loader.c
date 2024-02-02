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
#include "lookup_by_channel.generated.h"
#include "string_funs.h"
#include "syscalls.h"

static int nix_channel = CHANNEL_UNKNOWN;
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
      if (nix_channel != CHANNEL_UNKNOWN) {
        result = lookup_by_channel(nix_channel, libname);
        if (result != NULL) {
          log_info("  found statically: ");
          log_info(result);
          log_info("\n");
          return (char*)result;
        }
      }
      if (result == NULL) {
        result = dynamic_lookup(libname, replit_ld_library_path);
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
  if (nix_channel == CHANNEL_UNKNOWN) {
    nix_channel = get_nix_channel(map->l_name);
    if (nix_channel != CHANNEL_UNKNOWN) {
      log_info("Found Nix channel: ");
      log_info(nix_channel_str(nix_channel));
      log_info("\n");
    }
  }
  return 0;
}

void la_preinit(uintptr_t* cookie) {
  log_info("la_preinit()\n");
}