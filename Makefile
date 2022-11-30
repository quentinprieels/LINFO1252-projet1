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
philo: src/philosophes.c performances/time_measures.sh performances/plot_measures.py
	@$(CC) $(CFLAGS) $(THREADS) -o bins/philosophes src/philosophes.c
	@echo "Compilation of philosophers done, beginning the measures."
	@./performances/time_measures.sh philosophes.csv bins/philosophes
	@python3 performances/plot_measures.py performances/philosophes.csv philosophes.pdf

debug_philo: src/philosophes.c
	$(CC) $(CFLAGS) $(THREADS) -o bins/philosophes src/philosophes.c
	gdb --args bins/philosophes $(PHILO-THREADS)
	valgrind ./bins/philosophes $(PHILO-THREADS)
	cppcheck src/philosophes.c

prod_cons: src/producer_consumer.c performances/time_measures.sh performances/plot_measures.py
	@$(CC) $(CFLAGS) $(THREADS) -o bins/producer_consumer src/producer_consumer.c src/buffer.c
	@echo "Compilation of producer_consumer done, beginning the measures."
	@./performances/time_measures.sh producer_consumer.csv bins/producer_consumer
	@python3 performances/plot_measures.py performances/producer_consumer.csv producer_consumer.pdf

debug_prod_cons: src/producer_consumer.c
	$(CC) $(CFLAGS) $(THREADS) -o bins/producer_consumer src/producer_consumer.c
	gdb --args bins/producer_consumer $(PRODUCER-THREADS) $(CONSUMER-THREADS)
	valgrind ./bins/producer_consumer $(PRODUCER-THREADS) $(CONSUMER-THREADS)
	cppcheck src/producer_consumer.c


pdf: # TDOO : compile the report with the bash sript into the report/ folder
	./report/compile.sh

clean:
	@rm -f bins/*
	@rm -f *.o
	@rm -f *.log
	@rm -f performances/*.csv
	@rm -f performances/*.pdf	