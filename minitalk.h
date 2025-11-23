#ifndef MINITALK_H
# define MINITALK_H

# include "ft_printf/ft_printf.h"    //printf
# include <signal.h>    // sigaction, sigemptyset
# include <stdbool.h>   // boolean
# include <stdio.h>     // perror
# include <stdlib.h>    // EXIT_FAILURE
# include <unistd.h>    // write, getpid, pause

# define E_CLIENT_USAGE "Error\nUsage: ./client [server PID] [message]\n"
# define E_INVAL_PID "Error\nInvalid server PID\n"
# define E_SIGACTION "Error\nsigaction failed\n"
# define E_ACK "Error\nFailed to send acknowledgement\n"
# define E_NO_ACK "Error\nServer not responding\n"
# define E_NO_EOM_ACK "Error\nMessage sent, but no final acknowledgment\n"
# define SLEEP_MI_SEC 50
# define MAX_RETRIES 6000

enum
{
    BUSY,
    READY,
    EOM_ACK,
    ERROR,
    EOM,
};

typedef struct s_server_handler
{
    int     buf_idx;
    char    c;
    int     bit;
    pid_t   client_pid;
}           t_server_handler;

// utils
void    setup_handler(int, void *, bool);

#endif
