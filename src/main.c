/*
** EPITECH PROJECT, 2021
** ${FOLDER_NAME}
** File description:
** main.c file
*/

#include "strace.h"

static const int nb_flags = 5;

static char const * const flags_name[] = {
    "-h",
    "--help",
    "-s",
    "-p",
    "--attach",
};

static const int flags_value[] = {
    FLAG_H,
    FLAG_H,
    FLAG_S,
    FLAG_P,
    FLAG_P,
};

int *get_flags(void)
{
    static int f = 0;

    return &f;
}

static int is_uint(const char *str)
{
    for (; *str; ++str) {
        if (*str < '0' || *str > '9') {
            return 0;
        }
    }
    return 1;
}

static int manage_flags(int *ac, char **av)
{
    int pid = 0;

    for (int i = 0, cp = *ac, f = 0; i < cp && **av == '-'; ++i, ++av, --*ac) {
        for (f = 0; f < nb_flags && strcmp(*av, flags_name[f]); ++f);
        if (f == nb_flags || flags_value[f] == FLAG_H
        || (flags_value[f] == FLAG_P &&
        (i + 1 >= cp || !is_uint(*(av + 1)) || !atoi(*(av + 1)) || pid))) {
            flags = (f < nb_flags && flags_value[f] == FLAG_H) ? FLAG_H : -1;
            break;
        } else if (flags_value[f] == FLAG_P) {
            pid = atoi(*(av + 1));
            ++i, ++av, --*ac;
        }
        flags |= flags_value[f];
    }
    return pid;
}

int main(int ac, char **av)
{
    char *bin_name = *av;
    int cp = --ac;
    int pid = manage_flags(&ac, ++av);

    av += cp - ac;
    if (flags == FLAG_H) {
        printf("USAGE: %s [-s] [-p <pid>|<command>]\n", bin_name);
        return 0;
    } else if (flags == -1 || (!(flags & FLAG_P) && !ac)
    || (flags & FLAG_P && ac)) {
        return 84;
    }
    return strace(av, pid);
}