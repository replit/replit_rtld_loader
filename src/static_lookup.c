#include "string_funs.h"
#include "consts.h"

int get_nix_channel(const char *libpath) {
  if (streql("/nix/store/qn3ggz5sf3hkjs2c797xf7nan3amdxmp-glibc-2.38-27/lib/libc.so", libpath)) {
    return CHANNEL_23_11;
  }

  if (streql("/nix/store/qn3ggz5sf3hkjs2c797xf7nan3amdxmp-glibc-2.38-27/lib/libc.so.6", libpath)) {
    return CHANNEL_23_11;
  }

  if (streql("/nix/store/dg8mpqqykmw9c7l0bgzzb5znkymlbfjw-glibc-2.37-8/lib/libc.so.6", libpath)) {
    return CHANNEL_23_05;
  }

  if (streql("/nix/store/dg8mpqqykmw9c7l0bgzzb5znkymlbfjw-glibc-2.37-8/lib/libc.so", libpath)) {
    return CHANNEL_23_05;
  }

  return CHANNEL_UNKNOWN;
}

char *static_lookup(int nix_channel, const char *libname) {
  if (strstartswith(libname, )) {
    
  }
}