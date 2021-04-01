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


void print_arg(int pid, enum types type, unsigned long long int arg
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
        print_buffer(pid, arg, 0); // todo use rax to get size ?
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
        print_arg(pid, syscall.arg_type[i], args[i]);
    }
    printf(") = ");
    print_arg(pid, syscall.return_type, regs.rax);
    printf("\n");
}