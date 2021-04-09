/*
** EPITECH PROJECT, 2021
** B-PSU-402-BDX-4-1-strace-corentin.mas
** File description:
** tracer
*/

#include <stdio.h>
#include <sys/ptrace.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <string.h>
#include "strace.h"

static const struct_sysent sysentx86_64[] = {
    #include "x86_64/syscallent.h"
};

static const struct_sysent sysenti386[] = {
    #include "i386/syscallent.h"
};

static int handle_status(int status, int *didexit)
{
    if (didexit) {
        *didexit = 1;
    }
    if (WIFSIGNALED(status)) {
        printf("+++ killed by %s +++\n", strsignal(WTERMSIG(status)));
        return WTERMSIG(status);
    }
    if (WIFSTOPPED(status)) {
        if (WSTOPSIG(status) != SIGSTOP && WSTOPSIG(status) != SIGTRAP) {
            printf("+++ killed by %s +++\n", strsignal(WSTOPSIG(status)));
            return WSTOPSIG(status);
        }
    } else if (WIFEXITED(status)) {
        printf("+++ exited with %d +++\n", WEXITSTATUS(status));
        return WEXITSTATUS(status);
    }
    if (didexit) {
        *didexit = 0;
    }
    return 0;
}

static int print(const struct_sysent *syscall, unsigned long long int args[],
        int pid, int part)
{
    if (!syscall)
        return 0;
    struct user_regs_struct regs;
    if (part == 1) {
        if (ptrace(PTRACE_GETREGS, pid, NULL, &regs) < 0) {
            printf("?\n");
            return -1;
        } else if ((syscall->sys_flags && syscall->sys_flags & SE)) {
            printf("?");
        } else {
            printf("0x%llx", regs.rax);
        }
        printf("\n");
        return 0;
    }
    printf("%s(", syscall->sys_name);
    const char *s = "";
    for (unsigned int i = 0; i < syscall->nargs;
        printf("%s0x%llx", s, args[i]), s = ", ", ++i);
    printf(") = ");
    return 0;
}

static int fill_args(int pid, unsigned long long int *args,
                    const struct_sysent **syscall)
{
    struct user_regs_struct regs;
    instruction_t inst;

    if (ptrace(PTRACE_GETREGS, pid, NULL, &regs) < 0
    || (inst.inst = ptrace(PTRACE_PEEKTEXT, pid, regs.rip, NULL)) == -1) {
        return -1;
    }
    if ((inst.byte[0] & 0xFF) == 0xCD && (inst.byte[1] & 0xFF) == 0x80) {
        unsigned long long int argss[6] =
                {regs.rbx, regs.rcx, regs.rdx, regs.rsi, regs.rdi, regs.rbp};
        memcpy(args, argss, sizeof(unsigned long long int) * 6);
        *syscall = &sysenti386[regs.rax];
    } else if ((inst.byte[0] & 0xFF) == 0xF && (inst.byte[1] & 0xFF) == 0x5) {
        unsigned long long int argss[6] =
                {regs.rdi, regs.rsi, regs.rdx, regs.rcx, regs.r8, regs.r9};
        memcpy(args, argss, sizeof(unsigned long long int) * 6);
        *syscall = &sysentx86_64[regs.rax];
    }
    return 0;
}

int follow_process(int pid)
{
    int status = 0, didexit = 0;
    const struct_sysent *syscall = NULL;
    unsigned long long int args[6] = {};

    if (waitpid(pid, &(status), 0) < 0)
        return 84;
    while (1) {
        if (ptrace(PTRACE_SINGLESTEP, pid, NULL, NULL) < 0 || (waitpid(pid,
        &(status), 0)) < 0 || (print(syscall, args, pid, 1) < 0 &&
        ((status = handle_status(status, &didexit)) || 1) && !didexit)) {
            return 84;
        }
        if (didexit)
            break;
        syscall = NULL;
        if (fill_args(pid, args, &syscall) < 0
        || print(syscall, args, pid, 0) < 0) {
            return 84;
        }
    }
    return status;
}