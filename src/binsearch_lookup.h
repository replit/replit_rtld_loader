/*
LibEntry is a library entry containing a libname: say libc.so and
libpath: an absolute file path for locating the file containing the library.
*/
struct LibEntry {
  char *libname;
  char *libpath;
};

/*
Binary search lookup for a library by name. 

* libname - target library name to search for
* entries - an array of LibEntry structs to search within, must be sorted in
    ascending order
* entriesLength - the length of the entries array

Return value:
  the libpath value for the found entry with matching libname to the passed in libname.
  NULL if not found.
*/
char *binsearch_lookup(const char *libname, const struct LibEntry *entries, size_t entriesLength);
