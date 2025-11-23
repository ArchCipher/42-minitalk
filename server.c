# include <minitalk.h>

volatile sig_atomic_t g_state = READY;

static void handler (int, siginfo_t *, void *);
static int  send_signal(pid_t, int);
static void flush_message(char *, t_server_handler *);

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
    pid_t   server_pid;

    server_pid = getpid();
    ft_printf("Server PID: [%d]\n", server_pid);
    setup_handler(SIGUSR1, handler, true);
    setup_handler(SIGUSR2, handler, true);
    while(1)
    {
        pause();
        if (g_state == ERROR)
        {
            g_state = READY;
            ft_printf(E_ACK);
        }
    }
    return (EXIT_SUCCESS);
}

/*
DESCRIPTION:
    It is the signal handler for the server that receives the signal and the client PID.
    It builds the message bit by bit and prints it to the screen when the message is complete.
    If the signal is SIGUSR1, the bit is 0, if the signal is SIGUSR2, the bit is 1.

    It sends an acknowledgment signal to the client when the message is complete.
    If sends SIGUSR1 as a per bit acknowledgement from the server.
    If sends SIGUSR2 as an end of message acknowledgment.

    The third parameter of sigaction is uap: pointer to ucontext_t.
    It’s mostly there for advanced debugging, context manipulation, or low-level OS programming
*/

static void    handler (int sig, siginfo_t *info, void *uap)
{
    static t_server_handler msg;
    static char             msg_buffer[BUFSIZE];

    (void)uap;
    if (info == NULL || (info->si_pid && msg.client_pid != 0 && info->si_pid != msg.client_pid))
        return ;
    if (msg.client_pid == 0)
        msg.client_pid = info->si_pid;
    (msg.c) <<= 1;
    if (SIGUSR2 == sig)
        (msg.c) |= 1;
    (msg.bit)++;
    if (!send_signal(msg.client_pid, SIGUSR1))
    {
        ft_memset(&msg, 0, sizeof(t_server_handler));
        return ;
    }
    if (msg.bit == CHAR_BIT)
        flush_message(msg_buffer, &msg);
    if  (g_state == EOM)
    {
        g_state = READY;
        send_signal(msg.client_pid, SIGUSR2);
        msg.client_pid = 0;
    }
}

/*
DESCRIPTION:
    It is a wrapper function that calls kill to send a signal to a process.
    If the kill fails, it prints an error message and returns 0.
*/


static int send_signal(pid_t pid, int sig)
{
    if (kill(pid, sig) < 0)
    {
        write(STDERR_FILENO, "Error\nkill failed\n", 18);
        g_state = ERROR;
        return (0);
    }
    return (1);
}

/*
DESCRIPTION:
    It prints the message to the stdout or saves the char in the message buffer.
*/

static void flush_message(char *msg_buffer, t_server_handler *msg)
{
    if (msg->c == '\0')
    {
        msg_buffer[msg->buf_idx++] = '\n';
        g_state = EOM;
    }
    else
        msg_buffer[msg->buf_idx++] = msg->c;
    if (msg->buf_idx == BUFSIZE || msg->c == '\0')
    {
        write(STDOUT_FILENO, msg_buffer, msg->buf_idx);
        msg->buf_idx = 0;
    }
    msg->bit = 0;
    msg->c = 0;
}
