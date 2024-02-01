#include <stddef.h>

/*
Do without libc and call system calls directly.

Resource for Linux syscalls:
https://syscalls64.paolostivanin.com/

These stubs use the trick that function parameters in C are passed by
registers, which is also how system call parameters are passed
(we are lucky the Linux kernel is written in C).

In each case

1. first parameter is passed in the %rdi register
2. second param in %rsi
3. third in %rdx

Example:
  when sys_access("/path/to/file", R_OK) is called,

  the pointer to the string constant is put in %rdi
  and the value of R_OK in %rsi. Because Linux syscalls use
  these same registers, all we have to do
  is set the syscall number in %rax to let the kernel
  know which one - for sys_access
  it's 21 - and the parameters get passed through
  to the system call as they are.

*/

int sys_access(const char* path, int mode);
__asm__(
    "sys_access:\n"
    "  mov $21, %rax\n"
    "  syscall\n"
    "  ret\n");

int sys_write(int fd, const char* buf, size_t count);
__asm__(
    "sys_write:\n"
    "  mov $1, %rax\n"
    "  syscall\n"
    "  ret\n");

int sys_open(const char* path, int flags, int mode);
__asm__(
    "sys_open:\n"
    "  mov $2, %rax\n"
    "  syscall\n"
    "  ret\n");

int sys_close(int fd);
__asm__(
    "sys_close:\n"
    "  mov $3, %rax\n"
    "  syscall\n"
    "  ret\n");

int sys_read(int fd, char* buf, size_t count);
__asm__(
    "sys_read:\n"
    "  mov $0, %rax\n"
    "  syscall\n"
    "  ret\n");