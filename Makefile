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
LOCKER = 10

# Inginious tasks - compilation
compile: src/philosophes.c src/producer_consumer.c src/reader_writer.c src/test_and_set.c src/test_and_test_and_set.c
	$(CC) $(CFLAGS) $(THREADS) -o bins/philosophes src/philosophes.c
	$(CC) $(CFLAGS) $(THREADS) -o bins/producer_consumer src/producer_consumer.c src/buffer.c
	$(CC) $(CFLAGS) $(THREADS) -o bins/reader_writer src/reader_writer.c
	$(CC) $(CFLAGS) $(THREADS) -o bins/test_and_set src/test_and_set.c
	$(CC) $(CFLAGS) $(THREADS) -o bins/test_and_test_and_set src/test_and_test_and_set.c

# Make local tests and graphs
local: performances/time_measures.sh performances/plot_measures.py
	@make compile -s
	@echo "Compilation done."

	@echo "Launching local measure for philosophers."
	@./performances/time_measures.sh philosophes.csv bins/philosophes
	@echo "Launching local measure for producer_consumer."
	@./performances/time_measures.sh producer_consumer.csv bins/producer_consumer
	@echo "Launching local measure for reader_writer."
	@./performances/time_measures.sh reader_writer.csv bins/reader_writer
	@echo "Launching the plots"
	@python3 performances/plot_measures.py performances/philosophes.csv,performances/producer_consumer.csv,performances/reader_writer.csv part1.pdf different

	@echo "Launching local measure for test and set."
	@./performances/time_measures.sh test_and_set.csv bins/test_and_set
	@echo "Launching local measure for test and test and set."
	@./performances/time_measures.sh test_and_test_and_set.csv bins/test_and_test_and_set
	@echo "Launching the plots"
	@python3 performances/plot_measures.py performances/test_and_set.csv,performances/test_and_test_and_set.csv part2.pdf same

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


debug_test_and_set: src/test_and_set.c
	$(CC) $(CFLAGS) $(THREADS) -o bins/test_and_set src/test_and_set.c
	gdb --args bins/test_and_set $(LOCKER)
	valgrind ./bins/test_and_set $(LOCKER)
	cppcheck src/test_and_set.c

debug_test_and_test_and_set: src/test_and_test_and_set.c
	$(CC) $(CFLAGS) $(THREADS) -o bins/test_and_test_and_set src/test_and_test_and_set.c
	gdb --args bins/test_and_test_and_set $(LOCKER)
	valgrind ./bins/test_and_test_and_set $(LOCKER)
	cppcheck src/test_and_test_and_set.c

# Compile the pdf report
pdf: report/compile.sh
	./report/compile.sh

# Make zip for inginious tests
zip:
	@ zip -r project.zip . -x "*.git*" -x "*.pdf" -x"*.md" -x "*.code-workspace" -x "project.zip*" -x "create_zip.sh*" -x "report/*" -x "inginious/*" -x ".vscode/*"

# Clean the project
clean:
	@rm -f bins/*
	@rm -f *.o
	@rm -f *.log
	@rm -f performances/*.csv
	@rm -f *.csv
	@rm -f performances/*.pdf	
	@rm -f *.zip