#include "string_funs.h"
#include "syscalls.h"
#include "consts.h"
#include <fcntl.h>
#include "logging.h"

// We cannot using malloc, so here is the buffer used to return
// search results.
static char search_result[MAX_PATH_LENGTH] = {0};

char *dynamic_lookup(const char *libname, const char *ld_library_path) {
  int libname_len = my_strlen(libname);
  const char *rllp = ld_library_path;
  const char *next_rllp = rllp;
  while (*next_rllp) {
    rllp = next_rllp;
    next_rllp = my_strchrnul(rllp, ':');

    size_t rllp_len = next_rllp - rllp;
    if (*next_rllp) {
      // Advance past colon
      next_rllp++;
    }

    char current_lib_path[MAX_PATH_LENGTH] = {0};
    if (rllp_len + libname_len + 2 > sizeof(current_lib_path)) {
      // We need the lib_path to be able to fit the path, the separating slash,
      // the filename, and the null terminator. If not, we are ignoring it
      continue;
    }
    my_strncpy(current_lib_path, rllp, rllp_len);
    char *lib_path_suffix = current_lib_path + rllp_len;
    lib_path_suffix[0] = '/';
    my_strncpy(lib_path_suffix + 1, libname, libname_len);
    int lib_path_len = rllp_len + 1 + libname_len;
    lib_path_suffix[1 + libname_len] = '\0';

    log_debug("  trying ");
    log_debug(current_lib_path);
    log_debug("\n");
    // check if we can read the file (i.e. it exists and we can read it).
    if (sys_access(current_lib_path, R_OK) == 0) {
      my_strncpy(search_result, current_lib_path, lib_path_len);
      search_result[lib_path_len] = '\0';
      return search_result;
    }
  }

  return NULL;
}