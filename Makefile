# Compilation variable
CC = gcc
CFLAGS = -Wall -Werror -g
THREADS = -lpthread
TESTS = -lcuint

# Programs arguments
PHILO-THREADS = 100

# Rules
philo: src/philosophes.c
	$(CC) $(CFLAGS) $(THREADS) -o bins/philosophes src/philosophes.c
	valgrind ./bins/philosophes $(PHILO-THREADS)
	cppcheck src/philosophes.c

debug_philo: src/philosophes.c
	$(CC) $(CFLAGS) $(THREADS) -o bins/philosophes src/philosophes.c
	gdb --args bins/philosophes $(PHILO-THREADS)

pdf: # TDOO : compile the report with the bash sript into the report/ folder

clean:
	@rm -f bins/*.o
	@rm -f *.log