/*
** EPITECH PROJECT, 2021
** ${FOLDER_NAME}
** File description:
** strace.h file
*/



#ifndef C_STRACE_H
#define C_STRACE_H

#include <sys/ptrace.h>
#include <unistd.h>
#include <stdio.h>
#include <syscalls.h>
#include <sys/user.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "syscalls.h"

#define FLAG_H      (1 << 0)
#define FLAG_S      (1 << 1)
#define FLAG_P      (1 << 2)

int *get_flags(void);
#define flags       (*get_flags())

int strace(char **args, int *pids);

void print_syscall(int pid, syscall_t syscall, struct user_regs_struct regs);

#endif //C_STRACE_H
