MPICC = mpicc
MPICFLAGS = -std=c99 -fopenmp
MPICOPTFLAGS = -O2 -g -Wall
MPILDFLAGS = #-I/usr/local/openmpi/include

alltoall: mpi_alltoall.c
	$(MPICC) $(MPICFLAGS) $(MPICOPTFLAGS) -o $@ $^ $(MPILDFLAGS)	

alltoallv: mpi_alltoallv.c
	$(MPICC) $(MPICFLAGS) $(MPICOPTFLAGS) -o $@ $^ $(MPILDFLAGS)

gather: mpi_gather.c
	$(MPICC) $(MPICFLAGS) $(MPICOPTFLAGS) -o $@ $^ $(MPILDFLAGS)

allgather: mpi_allgather.c
	$(MPICC) $(MPICFLAGS) $(MPICOPTFLAGS) -o $@ $^ $(MPILDFLAGS)	

.PHONY: clean

clean:
	rm -f *.o teragen terasort terapartition teravalidate terametrics naivesort data.dat splits.dat
