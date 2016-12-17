CC = mpicxx
INCLUDE = /mnt/d/1/libs/libboost/include/
#INCLUDE = ../boost_1_47_0/boost_1_47_0/
#LIBPATH = /home/edu-cmc-stud16-615-09/dev/libs/libboost/lib/
LIBPATH = /mnt/d/1/libs/libboost/lib/
LIBS = libboost_mpi libboost_system libboost_graph libboost_graph_parallel libboost_serialization
LIBSSUFFIX = .a
LIBRARIES = $(addprefix $(LIBPATH), $(addsuffix $(LIBSSUFFIX), $(LIBS)))
OPTIONS =
BGPTASKNAME = bgp_run.jcf
NTHREADS = 4

test_boruvka: test_boruvka.cpp
	$(CC) -o test_boruvka test_boruvka.cpp -I $(INCLUDE) $(LIBRARIES) $(OPTIONS)

boruvka: boruvka.cpp
	$(CC) -o boruvka boruvka.cpp -I $(INCLUDE) $(LIBRARIES) $(OPTIONS)

boruvka_preprocessor: boruvka.cpp
	cpp boruvka.cpp > boruvka.cpp.preprocessed -I $(INCLUDE)

boruvka_obj: boruvka.cpp
	$(CC) -c boruvka.cpp -I $(INCLUDE) $(LIBRARIES) $(OPTIONS)	

llsubmit: boruvka
	llsubmit $(BGPTASKNAME)

runlocal: boruvka generate
	mpiexec -n $(NTHREADS) -host localhost /mnt/d/1/mst/boruvka

generate: generator small.graph big.graph
	./generator $(NTHREADS)

generator: generator.cpp
	g++ -o generator generator.cpp -std=gnu++0x

clean:
	rm test_boruvka boruvka
	rm generator
	rm small.graph big.graph
	rm small.part big.part
