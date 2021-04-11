/*
 * Copyright (c) 2016-2021 The strace developers.
 * All rights reserved.
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#ifndef STRACE_SYSENT_H
# define STRACE_SYSENT_H

typedef struct sysent {
    unsigned nargs;
    int sys_flags;
    int sen;

    int (*sys_func)();

    const char *sys_name;
} struct_sysent;

# define TRACE_FILE            000000001
# define TRACE_IPC            000000002
# define TRACE_NETWORK            000000004
# define TRACE_PROCESS            000000010
# define TRACE_SIGNAL            000000020
# define TRACE_DESC            000000040
# define TRACE_MEMORY            000000100
# define SYSCALL_NEVER_FAILS        000000200
# define MEMORY_MAPPING_CHANGE        000000400
# define STACKTRACE_CAPTURE_ON_ENTER    000001000
# define TRACE_INDIRECT_SUBCALL        000002000
# define COMPAT_SYSCALL_TYPES        000004000
# define TRACE_STAT            000010000
# define TRACE_LSTAT            000020000
# define TRACE_STATFS            000040000
# define TRACE_FSTATFS            000100000
# define TRACE_STATFS_LIKE        000200000
# define TRACE_FSTAT            000400000
# define TRACE_STAT_LIKE        001000000
# define TRACE_PURE            002000000
# define TRACE_SECCOMP_DEFAULT        004000000
# define TRACE_CREDS            010000000
# define TRACE_CLOCK            020000000

#endif /* !STRACE_SYSENT_H */
