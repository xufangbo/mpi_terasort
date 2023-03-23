MPICC = mpicc
MPICFLAGS = -std=c99 -fopenmp
MPICOPTFLAGS = -O2 -g -Wall
MPILDFLAGS = #-I/usr/local/openmpi/include

terasort: terasort_main.c terasort.c terarec.c
	$(MPICC) $(MPICFLAGS) $(MPICOPTFLAGS) -o $@ $^ $(MPILDFLAGS)

teragen: teragen_main.c terarec.c
	$(MPICC) $(MPICFLAGS) $(MPICOPTFLAGS) -o $@ $^ $(MPILDFLAGS)

teravalidate: teravalidate_main.c teravalidate.c terarec.c
	$(MPICC) $(MPICFLAGS) $(MPICOPTFLAGS) -o $@ $^ $(MPILDFLAGS)
	
terametrics: terametrics.c terasort.c terarec.c teravalidate.c
	$(MPICC) $(MPICFLAGS) $(MPICOPTFLAGS) -o $@ $^ $(MPILDFLAGS)

naivesort: naivesort.c terarec.c
	$(MPICC) $(MPICFLAGS) $(MPICOPTFLAGS) -o $@ $^ $(MPILDFLAGS)

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
