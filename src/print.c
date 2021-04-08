/*
** EPITECH PROJECT, 2021
** ${FOLDER_NAME}
** File description:
** print.c file
*/

#include <sys/ptrace.h>
#include <unistd.h>
#include <stdio.h>
#include <syscalls.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "strace.h"

void print_buffer(int child, unsigned long long addr, int continuous)
{
    char *buffer = calloc(1, 1024);
    unsigned long long int temp = 0;
    unsigned long int i = 0;

    for (; i <= 1024;) {
        temp = ptrace(PTRACE_PEEKDATA, child, addr + i, NULL);
        int l = 0;
        for (char *t = &temp; l < sizeof(temp) && *t != '\0'; ++t, ++i, ++l)
            memcpy(&buffer[i], t, 1);
        if (l < sizeof(temp))
            break;
    }

    if (!continuous)
        printf("\"");
    printf("%s", buffer);
    free(buffer);
    if (i >= 1024)
        return print_buffer(child, addr + i, 1);
    else
        printf("\"");
}


void print_arg(int pid, enum types type, unsigned long long int arg)
{
    switch (type) {
    case NUMBER:
        printf("%lld", arg);
        break;
    case POINTER:
        printf("0x%llx", arg);
        break;
    case STRING:
        if (flags & FLAG_S) {
            print_buffer(pid, arg, 0); // todo use rax to get size ?
        } else {
            printf("0x%llx", arg);
        }
        break;
    default:
        break;
    }
}

void print_syscall(int pid, struct_sysent syscall, struct user_regs_struct regs, unsigned long long int args[])
{
    // if (flags & FLAG_P) {
    //     printf("[%d] ", pid);
    // }
    printf("%s(", syscall.sys_name);
    for (int i = 0; i < syscall.nargs; ++i) {
        if (i > 0)
            printf(", ");
        // print_arg(pid, syscall.arg_type[i], args[i]);
        printf("0x%llx", args[i]);
    }
    printf(") = ");
    if (!(syscall.sys_flags && syscall.sys_flags & SE)) {
        ptrace(PTRACE_SINGLESTEP, pid, NULL, NULL);
        waitpid(pid, NULL, 0);
        ptrace(PTRACE_GETREGS, pid, NULL, &regs);
    }
    printf("0x%llx", regs.rax);
    // print_arg(pid, syscall.return_type, regs.rax);
    printf("\n");
}