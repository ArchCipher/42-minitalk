# include <minitalk.h>

volatile sig_atomic_t g_server = BUSY;

void ack_handler(int sig)
{
    (void)sig;
    g_server = READY;
}

void end_handler(int sig)
{
    (void)sig;
    write(STDOUT_FILENO, "OK!\n", 4);
    exit(EXIT_SUCCESS); // is this needed?
}

// 8 bits: 10000000
// c = 10
// 0000 0100
// USR1 if 0 and USR2 if 1

void    send_char(char c, pid_t server_pid)
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

int main (int ac, char **av)
{
    pid_t server_pid;
    char *msg;

    if (ac != 3)
        return (ft_printf(E_USAGE), 0);
    server_pid = ft_atoi(av[1]);
    msg = av[2];
    sig_handler(SIGUSR1, ack_handler, false);
    sig_handler(SIGUSR2, end_handler, false);
    while(*msg)
        send_char(*msg++, server_pid);
    send_char('\0', server_pid);
}