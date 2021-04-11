/*
** EPITECH PROJECT, 2021
** ${FOLDER_NAME}
** File description:
** strace.h file
*/



#ifndef C_STRACE_H
#define C_STRACE_H

#include "sysent.h"
#include "sysent_shorthand_defs.h"

#define SEN(syscall_name) 0, NULL

typedef union instruction_u {
    char byte[sizeof(long int)];
    long int inst;
} instruction_t;

#define FLAG_H      (1 << 0)
#define FLAG_S      (1 << 1)
#define FLAG_P      (1 << 2)

int *get_flags(void);

#define flags       (*get_flags())

int strace(char **args, int pid);

int follow_process(int pid);

int trace_me(char **args);

int attach_to_pid(int pid);

#endif //C_STRACE_H
