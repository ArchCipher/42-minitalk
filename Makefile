#				Library and Project names
SERVER			= server
CLIENT			= client
SERVER_BONUS	= server_bonus
CLIENT_BONUS	= client_bonus
PRINTF_LIB		= ft_printf

#				Headers
INC				= -I. -I$(PRINTF_LIB)

#				Sources & Objects
SRCDIR			= src/

SRCS			= utils

S_SRCS			= server
C_SRCS			= client
S_BSRCS			= server_bonus
# C_BSRCS			= client

# BSRCS		= 

SHARED_SRCS		= $(addprefix $(SRCDIR), $(addsuffix .c, $(SRCS)))
SERVER_SRCS		= $(addprefix $(SRCDIR), $(addsuffix .c, $(S_SRCS)))
CLIENT_SRCS		= $(addprefix $(SRCDIR), $(addsuffix .c, $(C_SRCS)))
SERVER_BSRCS	= $(addprefix $(SRCDIR), $(addsuffix .c, $(S_BSRCS)))
# CLIENT_BSRCS	= $(addprefix $(SRCDIR), $(addsuffix .c, $(C_BSRCS)))

SHARED_OBJS		= $(SHARED_SRCS:.c=.o)
SERVER_OBJS		= $(SERVER_SRCS:.c=.o)
CLIENT_OBJS		= $(CLIENT_SRCS:.c=.o)
SERVER_B_OBJS	= $(SERVER_BSRCS:.c=.o)
# CLIENT_B_OBJS	= $(CLIENT_BSRCS:.c=.o)

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

bonus: lib $(SERVER_BONUS) $(CLIENT)

$(SERVER_BONUS): $(SERVER_B_OBJS) $(SHARED_OBJS)
	$(CC) $(FLAGS) $(SFLAGS) -g $^ $(FT_PRINTF) -o $@

# $(CLIENT_BONUS): $(CLIENT_B_OBJS) $(SHARED_OBJS)
# 	$(CC) $(FLAGS) $(SFLAGS) -g $^ $(FT_PRINTF) -o $@

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
