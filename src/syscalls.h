#include <stddef.h>

/*
Do without libc and call system calls directly.

Resource for Linux syscalls:
https://syscalls64.paolostivanin.com/
*/

int sys_access(const char *path, int mode);
__asm__(
"sys_access:\n"
"  mov $21, %rax\n"
"  syscall\n"
"  ret\n"
);


int sys_write(int fd, const char *buf, size_t count);
__asm__(
"sys_write:\n"
"  mov $1, %rax\n"
"  syscall\n"
"  ret\n"
);

int sys_open(const char *path, int flags, int mode);
__asm__(
"sys_open:\n"
"  mov $2, %rax\n"
"  syscall\n"
"  ret\n"
);

int sys_close(int fd);
__asm__(
"sys_close:\n"
"  mov $3, %rax\n"
"  syscall\n"
"  ret\n"
);

int sys_read(int fd, char *buf, size_t count);
__asm__(
"sys_read:\n"
"  mov $0, %rax\n"
"  syscall\n"
"  ret\n"
);