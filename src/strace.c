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



// https://stackoverflow.com/questions/23928530/how-does-ptrace-work-in-linux
// https://nullprogram.com/blog/2018/06/23/
// https://stackoverflow.com/questions/33431994/extracting-system-call-name-and-arguments-using-ptrace



_Noreturn int follow_process(int pid);

int trace_me(int pid, char **args)
{
    //    printf("%d: %s\n", pid, "trace me");
    ptrace(PTRACE_TRACEME, pid, NULL, NULL);
    if (!pid) {
        //        printf("%d: %s\n", pid, "exec");
        execvp(args[1], args + 1);
        perror("Execve");
        return 1;
    }
    //    printf("%d: done\n", pid);
    return 0;
}

int strace(int pid, char **args)
{
    int status = 0;

    if (!pid)
        pid = fork();
    if (pid == -1) {
        perror("fork");
        return 1;
    }
    //    printf("%d => %d\n", getpid(), pid);
    if (pid == 0 && trace_me(pid, args)) // only child
        return 1;
    //    kill(pid, SIGSTOP);
    follow_process(pid);
    waitpid(pid, &status, 0);
    //    printf("%d: exited with code : %d\n", pid, WEXITSTATUS(status));
    return 0;
}

_Noreturn int follow_process(int pid) // todo _Noreturn not sure
{
    int status = 0;

    waitpid(pid, &(status), 0);
    ptrace(PTRACE_SETOPTIONS, pid, 0, PTRACE_O_TRACESYSGOOD);
    ptrace(PTRACE_SETOPTIONS, pid, 0, PTRACE_O_EXITKILL);


    while (1) {

        ptrace(PTRACE_SINGLESTEP, pid, NULL, NULL); // todo cant use this

        waitpid(pid, &(status), 0);

        if (WIFEXITED(status))
            break;

        struct user_regs_struct regs;

        ptrace(PTRACE_GETREGS, pid, NULL, &regs);


        // todo use peekdata to get function names & constants
        // todo use peekuser to get variables

        // https://www.linuxjournal.com/article/6100


        if (regs.orig_rax == -1)
            continue;

//        if ((long) regs.orig_rax == -ENOSYS) {
            printf("[%lld] %lld %lld %lld %lld %lld %lld = %lld\n",
                regs.orig_rax, regs.rdi, regs.rsi, regs.rdx, regs.r10,
                regs.r8, regs.r9, regs.rax);
//        } else {
//            printf("%lld\n", regs.rax);
//        }


        /*Todo
             * - stop before function entry -> get function & parmeters
             * - stop after function exit -> get rax
             *
             *Todo
             * - need to recover function name from elf or switch
             * - Get argument in text / int
             * */
    }
    return 0;
}

//int main(int argc, char **argv)
//{
//    pid_t pid = fork();
//
//    /* Vérification: pid == -1 */
//    if (pid == 0) { /* Enfant */
//
//        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
//        kill(getpid(), SIGSTOP);
//        execvp(argv[1], argv + 1);
//        perror("Execve");
//    } else { /* Parent */
//
//        int status;
//
//        waitpid(pid, &status, 0);
//        ptrace(PTRACE_SETOPTIONS, pid, 0, PTRACE_O_TRACESYSGOOD);
//
//        while (1) {
//            ptrace(PTRACE_SYSCALL, pid, NULL, NULL); // todo cant use this
//
//
//
//            waitpid(pid, &(status), 0);
//
//            /* Nous allons améliorer ça au Follow-Up */
//            if (WIFEXITED(status)) {
//                break;
//            }
//
//            /* Comportement spécifique à l'utilisation de PTRACE_SYSCALL: */
//            if (WIFSTOPPED(status) && WSTOPSIG(status) == (SIGTRAP | 0x80)) {
//                struct user_regs_struct regs;
//
//                ptrace(PTRACE_GETREGS, pid, NULL, &(regs));
//
//                /* Vérification syscall-enter-stop */
//                if ((long) regs.rax == -ENOSYS) {
//                    printf("[%lld] %lld %lld %lld %lld %lld %lld = \n",
//                        regs.orig_rax, regs.rdi, regs.rsi, regs.rdx, regs.r10,
//                        regs.r8, regs.r9);
//                } else {
//                    printf("%lld\n", regs.rax);
//                }
//            }
//        }
//    }
//
//    /* Affichage du résultat de exit() */
//    printf("?\n");
//    return 0;
//}