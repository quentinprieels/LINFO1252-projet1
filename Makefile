# Compilation variable
CC = gcc
CFLAGS = -Wall -Werror -g
THREADS = -lpthread -pthread
TESTS = -lcuint

# Programs threads number for debugging
PHILO-THREADS = 100
PRODUCER-THREADS = 10
CONSUMER-THREADS = 10
WRITER-THREADS = 10
READERS-THREADS = 10

# Inginious tasks - compilation
compile: src/philosophes.c src/producer_consumer.c src/reader_writer.c
	$(CC) $(CFLAGS) $(THREADS) -o bins/philosophes src/philosophes.c
	$(CC) $(CFLAGS) $(THREADS) -o bins/producer_consumer src/producer_consumer.c src/buffer.c
	$(CC) $(CFLAGS) $(THREADS) -o bins/reader_writer src/reader_writer.c

# Make local tests and graphs
local: performances/time_measures.sh performances/plot_measures.py
	@make compile -s
	@echo "Compilation done."

	@echo "Launching local measure for philosophers."
	@./performances/time_measures.sh philosophes.csv bins/philosophes
	@python3 performances/plot_measures.py performances/philosophes.csv philosophes.pdf

	@echo "Launching local measure for producer_consumer."
	@./performances/time_measures.sh producer_consumer.csv bins/producer_consumer
	@python3 performances/plot_measures.py performances/producer_consumer.csv producer_consumer.pdf

	@echo "Launching local measure for reader_writer."
	@./performances/time_measures.sh reader_writer.csv bins/reader_writer
	@python3 performances/plot_measures.py performances/reader_writer.csv reader_writer.pdf

# Debugging
debug_philo: src/philosophes.c
	$(CC) $(CFLAGS) $(THREADS) -o bins/philosophes src/philosophes.c
	gdb --args bins/philosophes $(PHILO-THREADS)
	valgrind ./bins/philosophes $(PHILO-THREADS)
	cppcheck src/philosophes.c

debug_prod_cons: src/producer_consumer.c
	$(CC) $(CFLAGS) $(THREADS) -o bins/producer_consumer src/producer_consumer.c
	gdb --args bins/producer_consumer $(PRODUCER-THREADS) $(CONSUMER-THREADS)
	valgrind ./bins/producer_consumer $(PRODUCER-THREADS) $(CONSUMER-THREADS)
	cppcheck src/producer_consumer.c

debug_read_write: src/reader_writer.c
	$(CC) $(CFLAGS) $(THREADS) -o bins/reader_writer src/reader_writer.c
	gdb --args bins/reader_writer $(WRITER-THREADS) $(READERS-THREADS)
	valgrind ./bins/reader_writer $(WRITER-THREADS) $(READERS-THREADS)
	cppcheck src/reader_writer.c

# Compile the pdf report
pdf: report/compile.sh
	./report/compile.sh

# Make zip for inginious tests
zip:
	@ zip -r project.zip . -x "*.git*" -x "*.pdf" -x"*.md" -x "*.code-workspace" -x "*.cls" -x "*.tex" -x "inginious/" -x ".vscode/" -x "project.zip*" -x "create_zip.sh*"

# Clean the project
clean:
	@rm -f bins/*
	@rm -f *.o
	@rm -f *.log
	@rm -f performances/*.csv
	@rm -f performances/*.pdf	
	@rm -f *.zip