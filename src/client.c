# include <minitalk.h>

volatile sig_atomic_t g_server_state = READY;

static void send_char(char, pid_t);
static void ack_handler(int);
static void send_signal(pid_t, int);
static void wait_for_ack(int *, bool);

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
    pid_t   server_pid;
    char    *msg;
    int     retry;

    if (ac != 3)
        return (ft_printf(E_CLIENT_USAGE), EXIT_FAILURE);
    server_pid = ft_atoi(av[1]);
    if (server_pid == 0 || kill(server_pid, 0) < 0)
        return (ft_printf(E_INVAL_PID), EXIT_FAILURE);
    msg = av[2];
    setup_handler(SIGUSR1, ack_handler, false);
    setup_handler(SIGUSR2, ack_handler, false);
    while(*msg)
        send_char(*msg++, server_pid);
    send_char('\0', server_pid);
    retry = 0;
    while (g_server_state != EOM_ACK)
        wait_for_ack(&retry, true);
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
    int retry;

    bit = CHAR_BIT - 1;
    while(bit >= 0)
    {
        g_server_state = BUSY;
        if (1 & (c >> bit))
            send_signal(server_pid, SIGUSR2);
        else
            send_signal(server_pid, SIGUSR1);
        bit--;
        retry = 0;
        while(g_server_state == BUSY)
            wait_for_ack(&retry, false);
    }
}

/*
DESCRIPTION:
    It is the signal handler for the client that receives the acknowledgment signal from the server.
    If the signal is SIGUSR1, it is per bit acknowledgement from the server.
    If the signal is SIGUSR2, it is the end of message acknowledgment.
*/

static void ack_handler(int sig)
{
    if (SIGUSR1 == sig && g_server_state != EOM_ACK)
        g_server_state = READY;
    else if (SIGUSR2 == sig)
    {
        write(STDOUT_FILENO, "Message sent successfully!\n", 27);
        g_server_state = EOM_ACK;
    }
}

/*
DESCRIPTION:
    It is a wrapper function that calls kill to send a signal to a process.
    If the kill fails, it prints an error message and exits the program.
*/

static void send_signal(pid_t pid, int sig)
{
    if (kill(pid, sig) < 0)
    {
        write(STDERR_FILENO, "Error\nkill failed\n", 18);
        exit (EXIT_FAILURE);
    }
}

static void wait_for_ack(int *retry, bool eom)
{
    usleep(SLEEP_MI_SEC);
    (*retry)++;
    if ((*retry) == MAX_RETRIES)
    {
        if (!eom)
            ft_printf(E_NO_ACK);
        else
            ft_printf(E_NO_EOM_ACK);
        exit(EXIT_FAILURE); 
    }
}
