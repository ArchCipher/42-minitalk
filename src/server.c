# include <minitalk.h>

static void    handler (int, siginfo_t *, void *);
static void    print_message(char, pid_t *);

/*
    NAME
        server
    USAGE
        ./server
    DESCRIPTION:
        Receives messages bit by bit from the client and prints them to the screen.
        If the USR1 is received, the bit is 0, if the USR2 is received, the bit is 1.
    EXTERNAL FUNC(S)
        sigaction, sigemptyset, sigaddset, kill, getpid, pause, write, exit
        ft_printf
*/

int main()
{
    pid_t   server_pid = getpid();
    ft_printf("Server PID: [%d]\n", server_pid);
    sig_handler(SIGUSR1, handler, true);
    sig_handler(SIGUSR2, handler, true);
    while(1)
        pause();
    return (EXIT_SUCCESS);
}

/*
DESCRIPTION:
    It is the signal handler for the server that receives the signal and the client PID.
    It builds the message bit by bit and prints it to the screen when the message is complete.
    If the signal is SIGUSR1, the bit is 0, if the signal is SIGUSR2, the bit is 1.
    It sends an acknowledgment signal to the client when the message is complete.
*/

void    handler (int sig, siginfo_t *info, void *uap)
{
    static char     c;
    static int      bit;
    static pid_t    client_pid;

    (void)uap;
    if (info == NULL || (info->si_pid && client_pid != 0 && info->si_pid != client_pid))
        return ;
    if (info->si_pid)
        client_pid = info->si_pid;
    c = c << 1;
    if (SIGUSR2 == sig)
        c |= 1;
    bit++;
    send_signal(client_pid, SIGUSR1);
    if (bit == CHAR_BIT)
    {
        print_message(c, &client_pid);
        bit = 0;
        c = 0;
    }
}

/*
DESCRIPTION:
    It prints the message to the screen and sends an end of message acknowledgment signal to the client.
*/

void print_message(char c, pid_t *client_pid)
{
    if (c == '\0')
    {
        write(STDOUT_FILENO, "\n", 1);
        send_signal(*client_pid, SIGUSR2);
        *client_pid = 0;
    }
    else
        write(STDOUT_FILENO, &c, 1);  
}
