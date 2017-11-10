NAME := test

FILES := get_two_tests.c unit_tests.c period_ten.c
OBJ = $(FILES:.c=.o) 

all: $(NAME)

$(OBJ):%.o: %.c
	gcc -c -I include/ $< -o $@

$(NAME): $(OBJ)
	gcc $(OBJ) -lcgreen -o $(NAME)
clean:
	rm -f $(OBJ)
fclean:
	rm -f $(NAME)
