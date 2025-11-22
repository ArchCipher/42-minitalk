# include <minitalk.h>

/*
DESCRIPTION:
    It is a wrapper function that calls sigaction to set up a signal handler.
    If the sigaction fails, it prints an error message and exits the program.

    sigaction blocks SIGUSR1 while processing SIGUSR2 and vice-versa.
    2 signatures for the sigaction handler:
        union __sigaction_u = void (*__sa_handler)(int);
        union __sigaction_u = void (*__sa_sigaction)(int, siginfo_t *, void *);

    If this bit is set, the handler function prototype is assumed to be the one pointed
    to by the sa_sigaction member of struct sigaction (the second prototype).
    The third parameter of sigaction is uap: pointer to ucontext_t.
    It’s mostly there for advanced debugging, context manipulation, or low-level OS programming
*/

void    sig_handler(int sig, void *handler, bool siginfo)
{
    struct sigaction sa;
    if (siginfo)
    {
        sa.sa_sigaction = handler;
        sa.sa_flags = SA_SIGINFO;
    }
    else
        sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sigaddset(&sa.sa_mask, SIGUSR1);
    sigaddset(&sa.sa_mask, SIGUSR2);
    if (sigaction(sig, &sa, NULL) < 0)
    {
        ft_printf("Error: sigaction failed\n");
        exit(EXIT_FAILURE);
    }
}

/*
DESCRIPTION:
    It is a wrapper function that calls kill to send a signal to a process.
    If the kill fails, it prints an error message and exits the program.
*/

void    send_signal(pid_t pid, int sig)
{
    if (kill(pid, sig) < 0)
    {
        ft_printf("Error: kill failed\n");
        exit(EXIT_FAILURE);
    }
}