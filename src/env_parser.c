#include "string_funs.h"
#include "syscalls.h"
#include <fcntl.h>
#include "env_parser.h"
#include "consts.h"

/*
  Simple state-based parser to read
  * REPLIT_LD_LIBRARY_PATH - colon-separated directory paths
  * REPLIT_RTLD_LOG_LEVEL - 0, 1, or 2 for verbosity
  from a file descriptor pointing to /proc/self/environ
*/

void parse_env(
  int fd,
  char *replit_ld_library_path_buffer) {
  char buf[1024];
  char varname[1024];
  int varnameCursor = 0;
  int mode = PARSE_VARNAME;
  int ldLibraryPathCursor = 0;
  while (1) {
    int bytes = sys_read(fd, buf, sizeof(buf));
    if (bytes <= 0) {
      break;
    }
    for (int i = 0; i < bytes; i++) {
      char chr = buf[i];
      if (mode == PARSE_VARNAME) {
        if (chr == '=') {
          if (strneql(varname, "REPLIT_LD_LIBRARY_PATH", varnameCursor)) {
            mode = PARSE_LD_LIBRARY_PATH;
            ldLibraryPathCursor = 0;
          } else {
            mode = PARSE_VALUE;
          }
        } else {
          if (varnameCursor >= sizeof(varname)) {
            continue; // truncate the varname if too long
          }
          varname[varnameCursor++] = chr;
        }
      } else if (mode == PARSE_VALUE) {
        if (chr == '\0') {
          mode = PARSE_VARNAME;
          varnameCursor = 0;
        }
      } else if (mode == PARSE_LD_LIBRARY_PATH) {
        if (ldLibraryPathCursor >= MAX_LD_LIBRARY_PATH_LENGTH - 1) {
          // too long. truncate it
            replit_ld_library_path_buffer[MAX_LD_LIBRARY_PATH_LENGTH - 1] = '\0';
          goto done;
        } else if (chr == '\0') {
            replit_ld_library_path_buffer[ldLibraryPathCursor] = '\0';
          goto done;
        } else {
            replit_ld_library_path_buffer[ldLibraryPathCursor++] = chr;
        }
      }
    }
  }

done:
}
