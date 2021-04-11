/*
** EPITECH PROJECT, 2021
** B-PSU-402-BDX-4-1-strace-corentin.mas
** File description:
** subcallent
*/

#define sys_semtimedop sys_semtimedop_time32
#define sys_recvmmsg sys_recvmmsg_time32

#include "../generic/subcallent.h"

#undef sys_recvmmsg
#undef sys_semtimedop
