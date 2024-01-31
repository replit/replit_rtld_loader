#define DEBUG   3
#define INFO    2
#define WARN    1
#define OFF     0

/*
Initializes the logger, setting the log level. If log level == 0,
no log file is created.
*/
void log_init(int level);

/* Write message to the log with the specified level. If
 level is higher than the log level used to initialize the logger,
 the message will be discarded.
*/
void log_write(const char *message, int level);
// Write integer to the log wit the specified level.
void log_write_int(int num, int level);

/*
  The functions below are just conviniences for log_write and log_write_int with
 predefined levels.
*/
void log_info(const char *message);
void log_info_int(int num);

void log_debug(const char *message);
void log_debug_int(int num);

void log_warn(const char *message);
void log_warn_int(int num);

// functions for printing to something other than the log file, for example:
// 1 for stdout
// 2 for stderr
void fprint(int fd, const char *message);
void fprint_int(int fd, int num);