
/*
Search for a library in a sequence of directory paths, similar to how
LD_LIBRARY_PATH works.

* libname - the name of the library
* ld_library_path - a colon-separated string of directory paths to be searched

Return value
  a string containing the absolute path of the shared object file if found, otherwise
  NULL
*/
const char *dynamic_lookup(const char *libname, const char *ld_library_path);