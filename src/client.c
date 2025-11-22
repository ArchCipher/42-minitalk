# include <minitalk.h>

volatile sig_atomic_t g_server = BUSY;

static void    send_char(char, pid_t);
static void    ack_handler(int);
static void    end_handler(int);

/*
    NAME
        client
    USAGE
        ./client [server PID] [message]
    DESCRIPTION:
        Sends a message bit by bit to the server and receives an acknowledgment.
        If the bit is 0, sends SIGUSR1, if the bit is 1, sends SIGUSR2.
    EXTERNAL FUNC(S)
        sigaction, sigemptyset, sigaddset, kill, getpid, usleep, write, exit
        ft_printf
*/

int main (int ac, char **av)
{
    pid_t server_pid;
    char *msg;

    if (ac != 3)
        return (ft_printf(E_USAGE), EXIT_FAILURE);
    server_pid = ft_atoi(av[1]);
    msg = av[2];
    sig_handler(SIGUSR1, ack_handler, false);
    sig_handler(SIGUSR2, end_handler, false);
    while(*msg)
        send_char(*msg++, server_pid);
    send_char('\0', server_pid);
    return (EXIT_SUCCESS);
}

/*
DESCRIPTION:
    It sends the message bit by bit to the server.
    If the bit is 0, sends SIGUSR1, if the bit is 1, sends SIGUSR2.
*/

static void    send_char(char c, pid_t server_pid)
{
    int bit;

    bit = CHAR_BIT - 1;
    while(bit >= 0)
    {
        if (1 & (c >> bit))
            send_signal(server_pid, SIGUSR2);
        else
            send_signal(server_pid, SIGUSR1);
        bit--;
        if(g_server == BUSY)
            usleep(42);
        g_server = BUSY;
    }
}

/*
DESCRIPTION:
    It is the signal handler for the client that receives the acknowledgment signal from the server.
*/

static void ack_handler(int sig)
{
    ft_printf("ACK: %d\n", sig);
    (void)sig;
    g_server = READY;
}

/*
DESCRIPTION:
    It is the signal handler for the client that receives the end of message acknowledgment signal from the server.
*/

static void end_handler(int sig)
{
    ft_printf("END: %d\n", sig);
    (void)sig;
    write(STDOUT_FILENO, "OK!\n", 4);
    exit(EXIT_SUCCESS); // is this needed?
}