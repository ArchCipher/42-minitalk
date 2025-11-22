# include <minitalk.h>

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
    // block SIGUSR1 while processing SIGUSR2 and vice-versa
    sigemptyset(&sa.sa_mask);
    sigaddset(&sa.sa_mask, SIGUSR1);
    sigaddset(&sa.sa_mask, SIGUSR2);
    if (sigaction(sig, &sa, NULL) < 0)
    {
        perror("sigaction failed");
        exit(EXIT_FAILURE);
    }
}

void    send_signal(pid_t pid, int sig)
{
    if (kill(pid, sig) < 0)
    {
        perror("Kill failed");
        exit(EXIT_FAILURE);
    }
}