// Parser modes
#define PARSE_VARNAME               0
#define PARSE_VALUE                 1
#define PARSE_LD_LIBRARY_PATH       2
#define PARSE_LOG_LEVEL             3

#define MAX_VARNAME_LENGTH          1024

void parse_env(
  int fd,
  char *replit_ld_library_path_buffer,
  int *log_level
);