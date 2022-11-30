# Compilation variable
CC = gcc
CFLAGS = -Wall -Werror -g
THREADS = -lpthread
TESTS = -lcuint

# Programs arguments
PHILO-THREADS = 100
PRODUCER-THREADS = 10
CONSUMER-THREADS = 10

# Rules
philo: src/philosophes.c performances/time_measures.sh
	@$(CC) $(CFLAGS) $(THREADS) -o bins/philosophes src/philosophes.c
	@echo "Compilation of philosophers done, beginning the measures."
	@./performances/time_measures.sh philosophes.csv bins/philosophes

debug_philo: src/philosophes.c
	$(CC) $(CFLAGS) $(THREADS) -o bins/philosophes src/philosophes.c
	gdb --args bins/philosophes $(PHILO-THREADS)
	valgrind ./bins/philosophes $(PHILO-THREADS)
	cppcheck src/philosophes.c

producer: src/prods-conso.c
	$(CC) $(CFLAGS) $(THREADS) -o bins/prods-conso src/prods-conso.c src/buffer.c
	valgrind ./bins/prods-conso $(CONSUMER-THREADS) $(PRODUCER-THREADS)
	cppcheck src/prods-conso.c

pdf: # TDOO : compile the report with the bash sript into the report/ folder
	./report/compile.sh

clean:
	@rm -f bins/*
	@rm -f bins/*.o
	@rm -f *.log

clean_time:
	@rm -f performances/*.csv