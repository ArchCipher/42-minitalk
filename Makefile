#				Library and Project names
SERVER			= server
CLIENT			= client
SERVER_BONUS	= server_bonus
CLIENT_BONUS	= client_bonus
PRINTF_LIB		= ft_printf

#				Headers
INC				= -I. -I$(PRINTF_LIB)

#				Sources & Objects
SRCS			= server.c client.c utils.c

OBJS			= $(SRCS:.c=.o)
SERVER_OBJS		= server.o utils.o
CLIENT_OBJS		= client.o utils.o

#				Compiler and Flags
CC				= cc
FLAGS			= -Wall -Werror -Wextra
SFLAGS			= -fsanitize=address

#				Library
FT_PRINTF		= -L$(PRINTF_LIB) -lftprintf

%.o: %.c
	$(CC) $(FLAGS) $(SFLAGS) $(INC) -g -c $< -o $@

all: lib $(SERVER) $(CLIENT) 

lib:
	@make -C $(PRINTF_LIB)

$(SERVER): $(SERVER_OBJS)
	$(CC) $(FLAGS) $(SFLAGS) -g $^ $(FT_PRINTF) -o $@

$(CLIENT): $(CLIENT_OBJS)
	$(CC) $(FLAGS) $(SFLAGS) -g $^ $(FT_PRINTF) -o $@

bonus: all $(SERVER_BONUS) $(CLIENT_BONUS)

$(SERVER_BONUS): $(SERVER_OBJS)
	$(CC) $(FLAGS) $(SFLAGS) -g $^ $(FT_PRINTF) -o $@

$(CLIENT_BONUS): $(CLIENT_OBJS)
	$(CC) $(FLAGS) $(SFLAGS) -g $^ $(FT_PRINTF) -o $@

clean:
	make -C $(PRINTF_LIB) clean
	rm -f $(OBJS)

fclean: clean
	make -C $(PRINTF_LIB) fclean
	rm -f $(SERVER) $(CLIENT) $(SERVER_BONUS) $(CLIENT_BONUS)

re: fclean all bonus

.PHONY: all clean fclean re bonus lib
