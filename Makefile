#			Library and Project names
SERVER		= server
CLIENT		= client
BONUS_S		= server_bonus
BONUS_C		= client_bonus
PRINTF_LIB	= ft_printf

#			Headers
INC			= -I. -I$(PRINTF_LIB)

#			Sources & Objects
SRCDIR		= src
SSRCS		= server
CSRCS		= client

SRCS		= utils
# BSRCS		= 

SERVER_SRCS	= $(addprefix $(SRCDIR)/, $(addsuffix .c, $(SSRCS)))
CLIENT_SRCS	= $(addprefix $(SRCDIR)/, $(addsuffix .c, $(CSRCS)))
SERVER_OBJS	= $(SERVER_SRCS:.c=.o)
CLIENT_OBJS	= $(CLIENT_SRCS:.c=.o)
SHARED_SCRS	= $(addprefix $(SRCDIR)/, $(addsuffix .c, $(SRCS)))
SHARED_OBJS	= $(SHARED_SCRS:.c=.o)
# MAN_SRCS	= $(addprefix $(SRCDIR)/, $(addsuffix .c, $(MSRCS)))
# MAN_OBJS	= $(MAN_SRCS:.c=.o)
# BONUS_SCRS	= $(addprefix $(SRCDIR)/, $(addsuffix _bonus.c, $(BSRCS)))
# BONUS_OBJS	= $(BONUS_SCRS:.c=.o)

#			Compiler and Flags
CC			= cc
FLAGS		= -Wall -Werror -Wextra
SFLAGS		= -fsanitize=address

#			Library
FT_PRINTF	= -L$(PRINTF_LIB) -lftprintf

%.o: %.c
	$(CC) $(FLAGS) $(SFLAGS) $(INC) -g -c $< -o $@

all: lib $(SERVER) $(CLIENT) 

lib:
	@make -C $(PRINTF_LIB)

$(SERVER): $(SERVER_OBJS) $(SHARED_OBJS)
	$(CC) $(FLAGS) $(SFLAGS) -g $^ $(FT_PRINTF) -o $@

$(CLIENT): $(CLIENT_OBJS) $(SHARED_OBJS)
	$(CC) $(FLAGS) $(SFLAGS) -g $^ $(FT_PRINTF) -o $@

# bonus: $(BONUS)
# lib

# $(BONUS): $(SHARED_OBJS) $(BONUS_OBJS)
# 	$(CC) $(FLAGS) $(SFLAGS) -g $^ $(API_OS) -o $@
# $(FT_PRINTF)

clean:
	make -C $(PRINTF_LIB) clean
	rm -f $(SERVER_OBJS) $(CLIENT_OBJS) $(SHARED_OBJS)
# 	$(SHARED_OBJS) $(MAN_OBJS) $(BONUS_OBJS)

fclean: clean
	make -C $(PRINTF_LIB) fclean
	rm -f $(SERVER) $(CLIENT) $(SHARED_OBJS)

re: fclean all
# bonus

.PHONY: all clean fclean re lib 
# bonus
