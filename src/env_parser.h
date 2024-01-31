// Parser states
#define PARSE_VARNAME               0
#define PARSE_VALUE                 1
#define PARSE_LD_LIBRARY_PATH       2
#define PARSE_LOG_LEVEL             3

#define MAX_VARNAME_LENGTH          1024

/*
Parses /proc/self/environ file to set config options for the rtld loader.
Which are:

* REPLIT_LD_LIBRARY_PATH - colon-separated directory paths
* REPLIT_RTLD_LOG_LEVEL - 0, 1, 2, or 3 for verbosity. See logging.h

* fd - the file descriptor to read from. Expects it to be in
  format of /proc/self/environ
* replit_ld_library_path_buffer - buffer to output the value of
  REPLIT_LD_LIBARY_PATH
* log_level - location to output value of REPLIT_RTLD_LOG_LEVEL
*/
void parse_env(
  int fd,
  char *replit_ld_library_path_buffer,
  int *log_level
);