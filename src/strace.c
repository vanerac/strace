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
#include <pthread.h>
#include <signal.h>

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

void *launch_thread(void *pid)
{
    if (!ptrace(PTRACE_ATTACH, *((int *)pid), NULL, NULL)) {
        printf("strace: Process %d attached\n", *((int *)pid));
        follow_process(*((int *)pid));
    }
    return NULL;
}

int **pids_glob()
{
    static int *pids = NULL;

    return &pids;
}

void sigint_handler(int sig)
{
    (void)sig;
    int *pids = *pids_glob();
    for (; pids && *pids; ++pids) {
        printf("\nRes: [%ld]\n", ptrace(PTRACE_DETACH, *pids, NULL, NULL));
        printf("strace: Process %d detached\n", *pids);
    }
    exit(0);
}

void launch_threads(int *pids)
{
    int nb = 0;
    for (; pids[nb]; ++nb);
    *pids_glob() = pids;
    pthread_t *t = malloc(sizeof(pthread_t) * nb);
    signal(SIGINT, sigint_handler);
    for (int i = 0; pids[i]; ++i) {
        pthread_create(&t[i], NULL, launch_thread, &pids[i]);
    }
    for (int i = 0; i < nb; ++i) {
        pthread_join(t[i], NULL);
    }
}

int trace_me(char **args)
{
    ptrace(PTRACE_TRACEME, 0, NULL, NULL);
    execvp(*args, args);
    perror("Execve");
    return 1;
}

int strace(char **args, int *pids)
{
    int pid = 0;

    if (!args && !pids) {
        return 84;
    }
    // if (pids && *pids) {
    //     launch_threads(pids);
    // }
    // return 0;
    pid = fork();
    if (pid == -1) {
        perror("fork");
        return 1;
    } else if (pid == 0 && trace_me(args)) {
        return 84;
    }

    int status = follow_process(pid);
    return status;
}
