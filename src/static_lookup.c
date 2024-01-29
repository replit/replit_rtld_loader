#include "string_funs.h"
#include "consts.h"

int get_nix_channel(const char *libpath) {
  if (strstartswith("/nix/store/qn3ggz5sf3hkjs2c797xf7nan3amdxmp-glibc-2.38-27/lib/", libpath)) {
    return CHANNEL_23_11;
  }

  if (strstartswith("/nix/store/dg8mpqqykmw9c7l0bgzzb5znkymlbfjw-glibc-2.37-8/lib/", libpath)) {
    return CHANNEL_23_05;
  }

  if (strstartswith("/nix/store/4nlgxhb09sdr51nc9hdm8az5b08vzkgx-glibc-2.35-163/lib/", libpath)) {
    return CHANNEL_22_11;
  }

  if (strstartswith("/nix/store/scd5n7xsn0hh0lvhhnycr9gx0h8xfzsl-glibc-2.34-210/lib/", libpath)) {
    return CHANNEL_22_05;
  }


  return CHANNEL_UNKNOWN;
}

char *static_lookup(int channel, const char *libname) {
  if (channel == CHANNEL_23_11) {
    if (streql(libname, "libstdc++.so")) {
      return "/nix/store/myw67gkgayf3s2mniij7zwd79lxy8v0k-gcc-12.3.0-lib/lib/libstdc++.so";
    }
    if (streql(libname, "libstdc++.so.6")) {
      return "/nix/store/myw67gkgayf3s2mniij7zwd79lxy8v0k-gcc-12.3.0-lib/lib/libstdc++.so.6";
    }
    if (streql(libname, "libX11.so.6")) {
      return "/nix/store/b7pb0mmcwjp973nj8gcaa2fnmqxckc8q-libX11-1.8.7/lib/libX11.so.6";
    }
    if (streql(libname, "libXcursor.so")) {
      return "/nix/store/70iyy9bchq62rl5wv9qhcpqpwnd3hysx-libXcursor-1.2.1/lib/libXcursor.so";
    }
    if (streql(libname, "libXcursor.so.1")) {
      return "/nix/store/70iyy9bchq62rl5wv9qhcpqpwnd3hysx-libXcursor-1.2.1/lib/libXcursor.so.1";
    }
  }

  if (channel == CHANNEL_23_05) {
    if (streql(libname, "libstdc++.so")) {
      return "/nix/store/sm14bmd3l61p5m0q7wa5g7rz2bl6azqf-gcc-12.2.0-lib/lib/libstdc++.so";
    }
    if (streql(libname, "libstdc++.so.6")) {
      return "/nix/store/sm14bmd3l61p5m0q7wa5g7rz2bl6azqf-gcc-12.2.0-lib/lib/libstdc++.so.6";
    }
    if (streql(libname, "libX11.so.6")) {
      return "/nix/store/gzs13l92w3308ichywy2013bw2n6l413-libX11-1.8.4/lib/libX11.so.6";
    }
    if (streql(libname, "libXcursor.so")) {
      return "/nix/store/jf4bifqwrj95skzspw195bbdly6lgkvk-libXcursor-1.2.0/lib/libXcursor.so";
    }
    if (streql(libname, "libXcursor.so.1")) {
      return "/nix/store/jf4bifqwrj95skzspw195bbdly6lgkvk-libXcursor-1.2.0/lib/libXcursor.so.1";
    }
    
  }

  return NULL;
}