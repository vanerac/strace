/*
** EPITECH PROJECT, 2021
** B-PSU-402-BDX-4-1-strace-corentin.mas
** File description:
** tracee
*/

#include <sys/ptrace.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include "strace.h"

static int *pid_glob(void)
{
    static int pid = 0;

    return &pid;
}

static void sigint_handler(int sig)
{
    (void)sig;
    int pid = *pid_glob();
    if (pid) {
        if (ptrace(PTRACE_INTERRUPT, pid, NULL, NULL) < 0
        || waitpid(pid, NULL, 0) < 0
        || ptrace(PTRACE_CONT, pid, NULL, NULL) < 0
        || waitpid(pid, NULL, 0) < 0
        || ptrace(PTRACE_DETACH, pid, NULL, NULL) < 0) {
            exit(84);
        }
        printf("strace: Process %d detached\n", pid);
    }
    exit(0);
}

int attach_to_pid(int pid)
{
    if (!ptrace(PTRACE_SEIZE, pid, NULL, NULL)) {
        printf("strace: Process %d attached\n", pid);
        *pid_glob() = pid;
        signal(SIGINT, sigint_handler);
        kill(pid, SIGSTOP);
        return 0;
    }
    return 1;
}

int trace_me(char **args)
{
    ptrace(PTRACE_TRACEME, 0, NULL, NULL);
    kill(getpid(), SIGSTOP);
    execvp(*args, args);
    perror("Execve");
    return 1;
}