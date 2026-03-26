MPICC = mpicc
CFLAGS = -g -Wall

TARGETS = PI_MPI_sto

all: $(TARGETS)

N ?= 1
run: PI_MPI_sto
	@echo "\033[32mRunning $(N) times\033[0m"; \
	echo "\033[36mRunning with 3 processes:\033[0m"; \
	for i in $$(seq 1 $(N)); do \
		mpirun -np 3 ./PI_MPI_sto; \
	done; \
	echo "\033[36mRunning with 5 processes:\033[0m"; \
	for i in $$(seq 1 $(N)); do \
		mpirun -np 5 ./PI_MPI_sto; \
	done;

PI_MPI_sto: PI_MPI_sto.c
	$(MPICC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGETS)
