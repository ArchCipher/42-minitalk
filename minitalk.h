#ifndef MINITALK_H
# define MINITALK_H

# include "ft_printf/ft_printf.h"    //printf
# include <signal.h>    // sigaction, sigemptyset
# include <stdbool.h>   // boolean
# include <stdio.h>     // perror
# include <stdlib.h>    // EXIT_FAILURE
# include <unistd.h>    // write, getpid, pause

# define E_USAGE "Usage: ./client.c [server PID] [message]\n"

enum
{
    READY,
    BUSY
};

// utils
void    sig_handler(int, void *, bool);
void    send_signal(pid_t, int);

#endif
