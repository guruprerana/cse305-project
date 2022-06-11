CXX = g++
CFLAGS = -pthread -std=c++17 -Wall

SEQUENCE_ALIGNMENT_FILES = sequence_alignment.hpp sequence_alignment.cpp sequence_alignment_parallel.cpp sequence_alignment_nonparallel.cpp utils.cpp

all: main.o $(SEQUENCE_ALIGNMENT_FILES)
	$(CXX) $(CFLAGS) -o main main.o

main.o: main.cpp
	$(CXX) -c $(CFLAGS) -o main.o main.cpp

TEST_OBJECTS = test.o

test: $(TEST_OBJECTS) $(SEQUENCE_ALIGNMENT_FILES)
	$(CXX) $(CFLAGS) -o test $(TEST_OBJECTS) -latomic

test.o: test.cpp
	$(CXX) -c $(CFLAGS) -o test.o test.cpp -I.

BENCHMARKER_OBJECTS = benchmarker.o

benchmarker: $(BENCHMARKER_OBJECTS) $(SEQUENCE_ALIGNMENT_FILES)
	$(CXX) $(CFLAGS) -o benchmarker $(BENCHMARKER_OBJECTS) -latomic

benchmarker.o: benchmarker.cpp
	$(CXX) -c $(CFLAGS) -o benchmarker.o benchmarker.cpp -I.

clean:
	rm -f *.o
	rm -f grader
	rm main
	rm test
