/*
** EPITECH PROJECT, 2021
** ${FOLDER_NAME}
** File description:
** main.c file
*/

#include <strace.h>

int main(int ac, char **ag)
{
    // parse args
    strace(0, ag);
    return 0;
}