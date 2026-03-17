MPICC = mpicc
CFLAGS = -g -Wall

TARGETS = PI_MPI_det PI_MPI_det_sleep PI_MPI_det_master_worker

all: $(TARGETS)

PI_MPI_det: PI_MPI_det.c
	$(MPICC) $(CFLAGS) -o $@ $^

PI_MPI_det_sleep: PI_MPI_det_sleep.c
	$(MPICC) $(CFLAGS) -o $@ $^

PI_MPI_det_master_worker: PI_MPI_det_master_worker.c
	$(MPICC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGETS)
