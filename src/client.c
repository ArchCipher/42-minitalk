# include <minitalk.h>

volatile sig_atomic_t g_server = READY;

static void    send_char(char, pid_t);
static void    ack_handler(int);

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
        return (ft_printf(E_CLIENT_USAGE), EXIT_FAILURE);
    server_pid = ft_atoi(av[1]);
    if (server_pid == 0 || kill(server_pid, 0) < 0)
        return (ft_printf(E_INVAL_PID), EXIT_FAILURE);
    msg = av[2];
    sig_handler(SIGUSR1, ack_handler, false);
    sig_handler(SIGUSR2, ack_handler, false);
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
        g_server = BUSY;
        if (1 & (c >> bit))
            send_signal(server_pid, SIGUSR2);
        else
            send_signal(server_pid, SIGUSR1);
        bit--;
        while(g_server == BUSY)
            usleep(42);
    }
}

/*
DESCRIPTION:
    It is the signal handler for the client that receives the acknowledgment signal from the server.
    If the signal is SIGUSR1, the server is ready to receive the next character.
    If the signal is SIGUSR2, the server has received the message and is ready to receive the next message.
*/

static void ack_handler(int sig)
{
    if (SIGUSR1 == sig)
        g_server = READY;
    else if (SIGUSR2 == sig)
    {
        write(STDOUT_FILENO, "OK!\n", 4);
        exit(EXIT_SUCCESS);
    }
}
