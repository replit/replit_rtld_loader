int sys_access(const char *path, int mode);
int sys_write(int fd, const char *buf, size_t count);
int sys_open(const char *path, int flags, int mode);
int sys_close(int fd);
int sys_read(int fd, char *buf, size_t count);