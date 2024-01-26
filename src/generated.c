#include "string_funs.h"

int check_nix_channel(const char *libpath) {
  if (streql("/nix/store/qn3ggz5sf3hkjs2c797xf7nan3amdxmp-glibc-2.38-27/lib/libc.so", libpath)) {
    return CHANNEL_;
  }
}