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
#include <errno.h>
#include <stdlib.h>
#include <string.h>

void print_buffer(int child, unsigned long long addr, unsigned long long len)
{

    //  && (!len && buffer != 0)
    printf("\"");
    for (int i = 0; (i < len); ++i) {
        char buffer = (char) ptrace(PTRACE_PEEKDATA, child, addr + i, NULL);
        printf("%c", buffer);
        ++i;
    }
    printf("\"");
}

void print_arg(int pid, enum types type, unsigned long long int arg,
    unsigned long long int next_arg
)
{
    switch (type) {
    case NUMBER:
        printf("%lld", arg);
        break;
    case POINTER:
        printf("0x%llx", arg);
        break;
    case STRING:
//        print_buffer(pid, arg, next_arg);
        break;
    default:
        break;
    }
}

void print_syscall(int pid, syscall_t syscall, struct user_regs_struct regs)
{
    unsigned long long int args[] = {regs.rdi, regs.rsi, regs.rdx, regs.r10,
        regs.r8, regs.r9};

    printf("%s(", syscall.name);
    for (int i = 0; i < syscall.nbr_args; ++i) {
        if (i > 0)
            printf(", ");
        print_arg(pid, syscall.arg_type[i], args[i], args[i + 1]);
    }
    printf(") = ");
    print_arg(pid, syscall.return_type, regs.rax, 0);
    printf("\n");
}