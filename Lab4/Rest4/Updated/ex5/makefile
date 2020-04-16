#uncomment this for debug
#FLAGS = -std=gnu99 -DDEBUG -Wall -lpthread
FLAGS = -std=gnu99 -Wall -lpthread -pthread

ex5: ex5_runner.c ex5_mmalloc.c
	gcc $(FLAGS) ex5_runner.c ex5_mmalloc.c -o ex5

clean:
	rm ex5

