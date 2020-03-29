#uncomment this for debug
#FLAGS = -std=gnu99 -DDEBUG -Wall -lpthread
FLAGS = -std=gnu99 -Wall -pthread -o -lpthread

ex2: ex2_runner.c ex2_mmalloc.c
	gcc $(FLAGS) ex2_runner.c ex2_mmalloc.c -o ex2

clean:
	rm ex2

