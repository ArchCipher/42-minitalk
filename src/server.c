# include <minitalk.h>

/*
if siginfo: 
    union __sigaction_u = void (*__sa_handler)(int);
     };
else
    union __sigaction_u = void (*__sa_sigaction)(int, siginfo_t *, void *);

uap: pointer to ucontext_t
It’s mostly there for advanced debugging, context manipulation, or low-level OS programming
*/

void    handler (int sig, siginfo_t *info, void *uap)
{
    static char     c;
    static int      bit;
    static pid_t    client_pid;

    (void)uap;
    if (info->si_pid)
        client_pid = info->si_pid;
    c = c << 1;
    if (SIGUSR1 == sig)
        c |= 0;
    else if (SIGUSR2 == sig)
        c |= 1;
    bit++;
    // write(STDOUT_FILENO, "hello\n", 6);
    if (bit == CHAR_BIT)
    {
        if (c == '\0')
        {
            write(STDOUT_FILENO, "\n", 1);
            send_signal(client_pid, SIGUSR2);
        }
        else
            write(STDOUT_FILENO, &c, 1);
        c = 0;
        bit = 0;
    }
    send_signal(client_pid, SIGUSR1);
}

int main()
{
    pid_t   server_pid = getpid();
    ft_printf("Server PID: [%d]\n", server_pid);
    sig_handler(SIGUSR1, handler, true);
    sig_handler(SIGUSR2, handler, true);
    while(1)
        pause();
}

/*
if SIUSR1 0
if SIGUSR2 1
*/