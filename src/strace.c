/*
** EPITECH PROJECT, 2021
** ${FOLDER_NAME}
** File description:
** strace.c file
*/

#include <errno.h>
#include <stdio.h>
#include <sys/ptrace.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <strace.h>

int follow_process(int pid)
{
    int status = 0;
    struct user_regs_struct regs;

    waitpid(pid, &(status), 0);

    while (1) {

        ptrace(PTRACE_SINGLESTEP, pid, NULL, NULL);

        waitpid(pid, &(status), 0);

        if (WIFEXITED(status))
            break;

        ptrace(PTRACE_GETREGS, pid, NULL, &regs);

        if (regs.orig_rax == -1)
            continue;
        print_syscall(pid, syscall_entries[regs.orig_rax], regs);
    }
    return status;
}

int trace_me(int pid, char **args)
{
    ptrace(PTRACE_TRACEME, pid, NULL, NULL);
    if (!pid) {
        execvp(args[1], args + 1);
        perror("Execve");
        return 1;
    }
    return 0;
}

int strace(int pid, char **args)
{

    if (!pid)
        pid = fork();
    if (pid == -1) {
        perror("fork");
        return 1;
    }
    if (pid == 0 && trace_me(pid, args))
        return 1;

    int status = follow_process(pid);
    return status;
}
