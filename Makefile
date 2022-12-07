# Compilation variable
CC = gcc
CFLAGS = -Wall -Werror -g
THREADS = -lpthread -pthread
TESTS = -lcuint

# INGInious run to convert measures into csv
INGI_RUN = inginious/run2.txt

# Programs threads number for debugging
PHILO-THREADS = 16
PRODUCER-THREADS = 8
CONSUMER-THREADS = 8
WRITER-THREADS = 16
READERS-THREADS = 16
LOCKER = 16

# Compilation
compile: src/philosophes.c src/producer_consumer.c src/reader_writer.c src/test_and_set.c src/test_and_test_and_set.c
	@echo -n "Compiling... "
	@$(CC) $(CFLAGS) $(THREADS) -o bins/philosophes src/philosophes.c
	@$(CC) $(CFLAGS) $(THREADS) -o bins/producer_consumer src/producer_consumer.c src/buffer.c
	@$(CC) $(CFLAGS) $(THREADS) -o bins/reader_writer src/reader_writer.c

	@$(CC) $(CFLAGS) $(THREADS) -o bins/test_and_set src/test_and_set.c src/locker.c
	@$(CC) $(CFLAGS) $(THREADS) -o bins/test_and_test_and_set src/test_and_test_and_set.c src/locker.c

	@$(CC) $(CFLAGS) $(THREADS) -o bins/philosophes_tts src/philosophes_tts.c src/locker.c
	@$(CC) $(CFLAGS) $(THREADS) -o bins/producer_consumer_tts src/producer_consumer_tts.c src/buffer.c src/locker.c src/new_semaphore.c
	@$(CC) $(CFLAGS) $(THREADS) -o bins/reader_writer_tts src/reader_writer_tts.c src/locker.c src/new_semaphore.c
	@echo "Done."

# Run all the INGInious part
ingi: ingi_csv ingi_plot

# Transform INGInous measures into csv
ingi_csv: inginious/split.py
	@echo -n "Transforming INGInous measures into csv... "
	@python3 inginious/split.py $(INGI_RUN) measures/
	@echo "Done."

# Create inginious plots
ingi_plot: performances/*
	@echo -n "Plotting INGInious measures... "
	@python3 performances/plot_simple.py measures/philosophes_ingi.csv plots/philosophes_ingi.pdf
	@python3 performances/plot_simple.py measures/producer_consumer_ingi.csv plots/producer_consumer_ingi.pdf
	@python3 performances/plot_simple.py measures/reader_writer_ingi.csv plots/reader_writer_ingi.pdf

	@python3 performances/plot_same.py measures/test_and_set_ingi.csv measures/test_and_test_and_set_ingi.csv plots/ts_tts_ingi.pdf

	@python3 performances/plot_same.py measures/philosophes_tts_ingi.csv measures/philosophes_ingi.csv plots/philosophes_tts_ingi.pdf
	@python3 performances/plot_same.py measures/producer_consumer_tts_ingi.csv measures/producer_consumer_ingi.csv plots/producer_consumer_tts_ingi.pdf
	@python3 performances/plot_same.py measures/reader_writer_tts_ingi.csv measures/reader_writer_ingi.csv plots/reader_writer_tts_ingi.pdf
	@echo "Done."

# Run all the local part
local: compile local_csv local_plot

# Collecting local performances
local_csv: performances/time_measures.sh
	@echo -n "Launching local measure for philosophers... "
	@./performances/time_measures.sh measures/philosophes_local.csv bins/philosophes
	@echo -n "Done.\nLaunching local measure for producer consumer... "
	@./performances/time_measures.sh measures/producer_consumer_local.csv bins/producer_consumer
	@echo -n "Done.\nLaunching local measure for reader writer... "
	@./performances/time_measures.sh measures/reader_writer_local.csv bins/reader_writer

	@echo -n "Done.\nLaunching local measure for test and set... "
	@./performances/time_measures.sh measures/test_and_set_local.csv bins/test_and_set
	@echo -n "Done.\nLaunching local measure for test and test and set... "
	@./performances/time_measures.sh measures/test_and_test_and_set_local.csv bins/test_and_test_and_set

	@echo -n "Done.\nLaunching local measure for philosophers with tts (this might take some time)... "
	@./performances/time_measures.sh measures/philosophes_tts_local.csv bins/philosophes_tts
	@echo -n "Done.\nLaunching local measure for producer consumer with tts (this might take some time)... "
	@./performances/time_measures.sh measures/producer_consumer_tts_local.csv bins/producer_consumer_tts
	@echo -n "Done.\nLaunching local measure for reader writer with tts (this might take some time)... "
	@./performances/time_measures.sh measures/reader_writer_tts_local.csv bins/reader_writer_tts
	@echo "Done."

# Create local plots
local_plot: performances/*
	@echo -n "Plotting local measures... "
	@python3 performances/plot_simple.py measures/philosophes_local.csv plots/philosophes_local.pdf
	@python3 performances/plot_simple.py measures/producer_consumer_local.csv plots/producer_consumer_local.pdf
	@python3 performances/plot_simple.py measures/reader_writer_local.csv plots/reader_writer_local.pdf

	@python3 performances/plot_same.py measures/test_and_set_local.csv measures/test_and_test_and_set_local.csv plots/ts_tts_local.pdf

	@python3 performances/plot_same.py measures/philosophes_tts_local.csv measures/philosophes_local.csv plots/philosophes_tts_local.pdf
	@python3 performances/plot_same.py measures/producer_consumer_tts_local.csv measures/producer_consumer_local.csv plots/producer_consumer_tts_local.pdf
	@python3 performances/plot_same.py measures/reader_writer_tts_local.csv measures/reader_writer_local.csv plots/reader_writer_tts_local.pdf
	@echo "Done."

# Create plot for local vs inginious (ts and tts only)
local_vs_ingi: performances/*
	@echo -n "Plotting local vs inginious measures... "
	@python3 performances/plot_subplot.py measures/test_and_set_local.csv measures/test_and_set_ingi.csv plots/ts_local_vs_ingi.pdf
	@python3 performances/plot_subplot.py measures/test_and_test_and_set_local.csv measures/test_and_test_and_set_ingi.csv plots/tts_local_vs_ingi.pdf
	@echo "Done."

# Verify code
verify: verify_philo verify_prod_cons verify_read_write verify_ts verify_tts

verify_philo: src/philosophes.c
	cppcheck src/philosophes.c
	cppcheck src/philosophes_tts.c
	valgrind ./bins/philosophes $(PHILO-THREADS)
	valgrind ./bins/philosophes_tts $(PHILO-THREADS)

verify_prod_cons: src/producer_consumer.c
	cppcheck src/producer_consumer.c
	cppcheck src/producer_consumer_tts.c
	valgrind ./bins/producer_consumer $(PRODUCER-THREADS) $(CONSUMER-THREADS)
	# valgrind ./bins/producer_consumer_tts $(PRODUCER-THREADS) $(CONSUMER-THREADS)

verify_read_write: src/reader_writer.c
	cppcheck src/reader_writer.c
	cppcheck src/reader_writer_tts.c
	valgrind ./bins/reader_writer $(READERS-THREADS) $(WRITER-THREADS)
	valgrind ./bins/reader_writer_tts $(READERS-THREADS) $(WRITER-THREADS)

verify_ts: src/test_and_set.c
	cppcheck src/test_and_set.c
	valgrind ./bins/test_and_set $(LOCKER)

verify_tts: src/test_and_test_and_set.c
	cppcheck src/test_and_test_and_set.c
	valgrind ./bins/test_and_test_and_set $(LOCKER)

# Launch the experiments scirpt
experiments: bins/*
	@echo "Launching experiments... "
	@./experiments.sh
	@echo "Done."

# Make zip for inginious tests
zip_for_measures:
	@ zip -r project.zip . -x "*.git*" -x "*.pdf" -x"*.md" -x "*.code-workspace" -x "project.zip*" -x "inginious/*" -x ".vscode/*" -x "measures/*" -x "plots/*"

# Make zip for inginious tests
zip_for_ingi:
	@ zip -r project.zip . -x "*.git*" -x "consignes.pdf" -x "project.zip*" -x ".vscode/*"

# Clean the project
clean:
	@rm -f bins/*
	@rm -f *.o
	@rm -f *.log
	@rm -f *.csv
	@rm -f *.zip

clean_force:
	@rm -f measures/*.csv
	@rm -f plots/*.pdf

help:
	@echo "compile : compile tous les fichiers relatifs aux questions"
	@echo "ingi : crée les plots relatifs aux mesures retournées par inginious"
	@echo "local : crée les plots relatifs aux mesures faites en local"
	@echo "local_vs_ingi : crée les plots relatifs aux mesures faites en local et sur inginious. Nécessite un appel à ingi et local avant."
	@echo "verify : vérifie les potentielles fuites mémoires"
	@echo "experiments : lance le script experiments.sh"
	@echo "zip_for_measures : crée une archive zip utilisée pour les mesures sur inginious"
	@echo "zip_for_ingi : crée une archive zip utilisée pour rendre le projet final"
	@echo "clean : efface tous les fichiers compilés"
	@echo "clean_force : efface toutes les mesures"