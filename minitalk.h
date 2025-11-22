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
# define E_KILL "Error\nkill failed\n"

# define BUSY 0
# define READY 1

// utils
void    sig_handler(int, void *, bool);
void    send_signal(pid_t, int);

#endif
