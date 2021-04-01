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

int strace(int pid, char **args);

void print_syscall(int pid, syscall_t syscall, struct user_regs_struct regs);

#endif //C_STRACE_H
