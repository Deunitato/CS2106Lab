#uncomment this for debug
#FLAGS = -std=gnu99 -DDEBUG -Wall -lpthread
FLAGS = -std=gnu99 -Wall -lpthread -pthread -g

ex4: ex4_runner.c ex4_mmalloc.c
	gcc $(FLAGS) ex4_runner.c ex4_mmalloc.c -o ex4

clean:
	rm ex4

