# Compilation variable
CC = gcc
CFLAGS = -Wall -Werror -g
THREADS = -lpthread -pthread
TESTS = -lcuint

# Programs threads number for debugging
PHILO-THREADS = 100
PRODUCER-THREADS = 8
CONSUMER-THREADS = 8
WRITER-THREADS = 16
READERS-THREADS = 16
LOCKER = 16

# Compilation
compile: src/philosophes.c src/producer_consumer.c src/reader_writer.c src/test_and_set.c src/test_and_test_and_set.c
	$(CC) $(CFLAGS) $(THREADS) -o bins/philosophes src/philosophes.c
	$(CC) $(CFLAGS) $(THREADS) -o bins/producer_consumer src/producer_consumer.c src/buffer.c
	$(CC) $(CFLAGS) $(THREADS) -o bins/reader_writer src/reader_writer.c

	$(CC) $(CFLAGS) $(THREADS) -o bins/test_and_set src/test_and_set.c src/locker.c
	$(CC) $(CFLAGS) $(THREADS) -o bins/test_and_test_and_set src/test_and_test_and_set.c src/locker.c

	$(CC) $(CFLAGS) $(THREADS) -o bins/philosophes_tts src/philosophes_tts.c src/locker.c
	$(CC) $(CFLAGS) $(THREADS) -o bins/producer_consumer_tts src/producer_consumer_tts.c src/buffer.c src/locker.c src/new_semaphore.c
	$(CC) $(CFLAGS) $(THREADS) -o bins/reader_writer_tts src/reader_writer_tts.c src/locker.c src/new_semaphore.c

# Collecting performances
measure: performances/time_measures.sh performances/plot_measures.py
	@echo "Launching local measure for philosophers."
	@./performances/time_measures.sh philosophes.csv bins/philosophes
	@echo "Launching local measure for producer_consumer."
	@./performances/time_measures.sh producer_consumer.csv bins/producer_consumer
	@echo "Launching local measure for reader_writer."
	@./performances/time_measures.sh reader_writer.csv bins/reader_writer

	@echo "Launching local measure for test and set."
	@./performances/time_measures.sh test_and_set.csv bins/test_and_set
	@echo "Launching local measure for test and test and set."
	@./performances/time_measures.sh test_and_test_and_set.csv bins/test_and_test_and_set

	@echo "Launching local measure for philosophers tts"
	@./performances/time_measures.sh philosophes_tts.csv bins/philosophes_tts
	@echo "Launching local measure for producer_consumer tts."
	@./performances/time_measures.sh producer_consumer_tts.csv bins/producer_consumer_tts
	@echo "Launching local measure for reader_writer tts."
	@./performances/time_measures.sh reader_writer_tts.csv bins/reader_writer_tts

# Plotting performances
plot: performances/plot_measures.py
	@echo "Plotting local performances for first task."
	@python3 performances/plot_measures.py performances/philosophes.csv performances/philosophes.pdf
	@python3 performances/plot_measures.py performances/producer_consumer.csv performances/producer_consumer.pdf
	@python3 performances/plot_measures.py performances/reader_writer.csv performances/reader_writer.pdf

	@echo "Plotting local performances for lockers"
	@python3 performances/plot_measures.py performances/test_and_set.csv performances/test_and_set.pdf
	@python3 performances/plot_measures.py performances/test_and_test_and_set.csv performances/test_and_test_and_set.pdf

	@echo "Plotting local performances for second task."
	@python3 performances/plot_measures.py performances/philosophes_tts.csv performances/philosophes_tts.pdf
	@python3 performances/plot_measures.py performances/producer_consumer_tts.csv performances/producer_consumer_tts.pdf
	@python3 performances/plot_measures.py performances/reader_writer_tts.csv performances/reader_writer_tts.pdf

# All plot in local tests
local:
	@make compile -s
	@make measure
	@make plot

local_tts: performances/time_measures.sh performances/plot_measures.py	
	@echo "Launching local ts vs tts."
	@./performances/time_measures.sh test_and_set.csv bins/test_and_set
	@./performances/time_measures.sh test_and_test_and_set.csv bins/test_and_test_and_set

	@echo "Plot tts vs ts."
	@python3 performances/plot_measures.py performances/test_and_set.csv performances/test_and_set_local.pdf
	@python3 performances/plot_measures.py performances/test_and_test_and_set.csv performances/test_and_test_and_set_local.pdf

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

debug_philo_tts: src/philosophes_tts.c
	$(CC) $(CFLAGS) $(THREADS) -o bins/philosophes_tts src/philosophes_tts.c
	$(CC) $(CFLAGS) $(THREADS) -o bins/philosophes src/philosophes.c
	# ./bins/philosophes_tts $(PHILO-THREADS)
	./bins/philosophes $(PHILO-THREADS)

debug_prod_cons_tts: src/producer_consumer_tts.c
	$(CC) $(CFLAGS) $(THREADS) -o bins/producer_consumer_tts src/producer_consumer_tts.c
	# gdb --args bins/producer_consumer_tts $(PRODUCER-THREADS) $(CONSUMER-THREADS)
	# valgrind ./bins/producer_consumer_tts $(PRODUCER-THREADS) $(CONSUMER-THREADS)
	# cppcheck src/producer_consumer_tts.c
	./bins/producer_consumer_tts $(PRODUCER-THREADS) $(CONSUMER-THREADS)

debug_read_write_tts: src/reader_writer_tts.c
	$(CC) $(CFLAGS) $(THREADS) -o bins/reader_writer_tts src/reader_writer_tts.c src/locker.c src/new_semaphore.c
	gdb --args bins/reader_writer_tts $(WRITER-THREADS) $(READERS-THREADS)
	valgrind ./bins/reader_writer_tts $(WRITER-THREADS) $(READERS-THREADS)
	cppcheck src/reader_writer_tts.c

final_plot:
	@python3 inginious/split.py inginious/run2.txt
	
	@echo "Plotting the comparison of algorithms..."
	@python3 performances/plot_measures.py performances/test-and-set_ingi.csv,performances/test-and-test-and-set_ingi.csv performances/ts_vs_tts.pdf line
	@python3 performances/plot_measures.py performances/philosophes_ingi.csv,performances/philosophes-tts_ingi.csv performances/philosophes_vs_tts.pdf line
	@python3 performances/plot_measures.py performances/producer-consumer_ingi.csv,performances/producer-consumer-tts_ingi.csv performances/producer_consumer_vs_tts.pdf line
	@python3 performances/plot_measures.py performances/reader-writer_ingi.csv,performances/reader-writer-tts_ingi.csv performances/reader_writer_vs_tts.pdf line
	@python3 performances/plot_measures.py performances/test-and-test-and-set_ingi.csv,performances/test-and-set_ingi.csv performances/ts_and_tts.pdf multiple

plot_tts_local: performances/plot_compare.py
	@python3 performances/plot_compare.py performances/test_and_set.csv performances/test-and-set_ingi.csv performances/ts_local_vs_ingi.pdf
	@python3 performances/plot_compare.py performances/test_and_test_and_set.csv performances/test-and-test-and-set_ingi.csv performances/tts_local_vs_ingi.pdf

plot_vs_tts: performances/plot_time.py
	python3 performances/plot_duo.py performances/philosophes_ingi.csv performances/philosophes-tts_ingi.csv performances/philosophes_vs_tts.pdf
	python3 performances/plot_duo.py performances/producer-consumer_ingi.csv performances/producer-consumer-tts_ingi.csv performances/producer_consumer_vs_tts.pdf
	python3 performances/plot_duo.py performances/reader-writer_ingi.csv performances/reader-writer-tts_ingi.csv performances/reader_writer_vs_tts.pdf

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