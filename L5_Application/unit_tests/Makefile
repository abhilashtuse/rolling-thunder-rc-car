NAME := test

FILES := get_two_tests.c unit_tests.c
OBJ = $(FILES:.c=.o) 

all: $(NAME)

$(OBJ):%.o: %.c
	gcc -c $< -o $@

$(NAME): $(OBJ)
	gcc $(OBJ) -lcgreen -o $(NAME)