MPICC = mpicc
CFLAGS = -g -Wall

TARGETS = PI_MPI_det PI_MPI_det_sleep PI_MPI_det_master_worker

all: $(TARGETS)

run: $(TARGETS)
	@echo "\033[36mRunning PI_MPI_det with 1 process:\033[0m"; \
	mpirun -np 1 ./PI_MPI_det; \
	echo "\033[36mRunning PI_MPI_det with 2 processes:\033[0m"; \
	mpirun -np 2 ./PI_MPI_det; \
	echo "\033[36mRunning PI_MPI_det with 4 processes:\033[0m"; \
	mpirun -np 4 ./PI_MPI_det; \
	echo "\033[36mRunning PI_MPI_det_sleep with 4 processes:\033[0m"; \
	mpirun -np 4 ./PI_MPI_det_sleep; \
	echo "\033[36mRunning PI_MPI_det_master_worker with 4 processes:\033[0m"; \
	mpirun -np 4 ./PI_MPI_det_master_worker; \

PI_MPI_det: PI_MPI_det.c
	$(MPICC) $(CFLAGS) -o $@ $^

PI_MPI_det_sleep: PI_MPI_det_sleep.c
	$(MPICC) $(CFLAGS) -o $@ $^

PI_MPI_det_master_worker: PI_MPI_det_master_worker.c
	$(MPICC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGETS)
