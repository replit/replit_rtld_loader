#include "env_parser.h"
#include <fcntl.h>
#include "consts.h"
#include "string_funs.h"
#include "syscalls.h"

/*
  Simple DFA-like parser (reads char by char; has a state)
*/
void parse_env(int fd, char* replit_ld_library_path_buffer, int* nix_channel) {
  char buf[1024];
  char varname[MAX_VARNAME_LENGTH];
  int varname_cursor = 0;
  int state = PARSE_VARNAME;
  int ld_library_path_cursor = 0;
  replit_ld_library_path_buffer[0] = '\0';
  *nix_channel = 0;
  while (1) {
    int bytes = sys_read(fd, buf, sizeof(buf));
    if (bytes <= 0) {
      break;
    }
    for (int i = 0; i < bytes; i++) {
      char chr = buf[i];
      if (state == PARSE_VARNAME) {
        if (chr == '=') {
          if (strneql(varname, "REPLIT_LD_LIBRARY_PATH", varname_cursor)) {
            state = PARSE_LD_LIBRARY_PATH;
            ld_library_path_cursor = 0;
          } else if (strneql(varname, "REPLIT_RTLD_NIX_CHANNEL",
                             varname_cursor)) {
            state = PARSE_NIX_CHANNEL;
          } else {
            state = PARSE_IGNORED;
          }
        } else {
          if (varname_cursor >= MAX_VARNAME_LENGTH) {
            continue;  // truncate the varname if too long
          }
          varname[varname_cursor++] = chr;
        }
      } else if (state == PARSE_IGNORED) {
        if (chr == '\0') {
          state = PARSE_VARNAME;
          varname_cursor = 0;
        }
      } else if (state == PARSE_LD_LIBRARY_PATH) {
        if (ld_library_path_cursor >= MAX_LD_LIBRARY_PATH_LENGTH - 1) {
          // too long. truncate it
          replit_ld_library_path_buffer[MAX_LD_LIBRARY_PATH_LENGTH - 1] = '\0';
          state = PARSE_IGNORED;
        } else if (chr == '\0') {
          replit_ld_library_path_buffer[ld_library_path_cursor] = '\0';
          state = PARSE_VARNAME;
          varname_cursor = 0;
        } else {
          replit_ld_library_path_buffer[ld_library_path_cursor++] = chr;
        }
      } else if (state == PARSE_NIX_CHANNEL) {
        if (chr >= '0' && chr <= '9') {
          *nix_channel = chr - '0';
        }
        // Only take one character, ignore the
        // rest of the value. This means double
        // digit values will not work as expected.
        state = PARSE_IGNORED;
      }
    }
  }
}
