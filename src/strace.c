/*
** EPITECH PROJECT, 2021
** ${FOLDER_NAME}
** File description:
** strace.c file
*/

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include "strace.h"

int strace(char **args, int pid)
{
    if ((!args || !*args) && !pid) {
        return 84;
    }
    if (pid && attach_to_pid(pid)) {
        return 84;
    } else if (!pid) {
        pid = fork();
        if (pid == -1) {
            perror("fork");
            return 1;
        } else if (pid == 0 && trace_me(args)) {
            return 84;
        }
    }
    return follow_process(pid);
}
