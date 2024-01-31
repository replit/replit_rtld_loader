void log_init();
void log_write(const char *message);
void log_write_int(int num);
void log_write_n(const char *message, int len);
void fprint(int fd, const char *message);
void fprint_int(int fd, int num);