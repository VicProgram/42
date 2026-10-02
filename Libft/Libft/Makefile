NAME = libft.a

CC = cc

CFLAGS = -Wall -Wextra -Werror

SRC = ft_lstclear.c    ft_putnbr_fd.c  ft_strnstr.c \
ft_atoi.c          ft_lstdelone.c   ft_putstr_fd.c  ft_strrchr.c\
ft_bzero.c         ft_lstlast.c     ft_split.c      ft_strtrim.c\
ft_calloc.c        ft_lstnew.c      ft_strchr.c     ft_substr.c\
ft_isalnum.c       ft_lstsize.c     ft_strdup.c     ft_tolower.c\
ft_isalpha.c       ft_memchr.c      ft_striteri.c   ft_toupper.c\
ft_isascii.c       ft_memcmp.c      ft_strjoin.c    ft_isdigit.c\
ft_memcpy.c      ft_strlcat.c		ft_isprint.c    ft_memmove.c\
ft_strlcpy.c	ft_itoa.c          ft_memset.c      ft_strlen.c\
ft_lstadd_back.c   ft_putchar_fd.c  ft_strmapi.c	ft_lstadd_front.c\
ft_putendl_fd.c  ft_strncmp.c ft_lstiter.c	ft_lstmap.c


OBJ = $(SRC:.c=.o)

INCLUDE = libft.h

AR = ar rcs
RM = rm -f

.PHONY: all clean fclean re


all: $(NAME)

$(NAME): $(OBJ)
	$(AR) $(NAME) $(OBJ)


%.o: %.c $(INCLUDE)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re : fclean all
