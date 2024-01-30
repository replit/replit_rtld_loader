struct LibEntry {
  char *libname;
  char *libpath;
};

char *binsearch_lookup(const char *libname, struct LibEntry *entries, int entriesLength);
