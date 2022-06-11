CXX = g++
CFLAGS = -pthread -std=c++17 -Wall

SOURCES = gradinglib/gradinglib.cpp grading/grading.cpp main.cpp 
OBJECTS = sequence_alignment.o
STUDENTS_SOURCES = td7.cpp

grader: $(OBJECTS)
	$(CXX) $(CFLAGS) -o grader $(OBJECTS) -latomic 

gradinglib.o: gradinglib/gradinglib.cpp gradinglib/gradinglib.hpp
	$(CXX) -c $(CFLAGS) -o gradinglib.o gradinglib/gradinglib.cpp

grading.o: grading/grading.cpp gradinglib/gradinglib.hpp td7.cpp
	$(CXX) -c $(CFLAGS) -o grading.o grading/grading.cpp -I.

main.o: main.cpp grading/grading.hpp
	$(CXX) -c $(CFLAGS) -o main.o main.cpp

TEST_OUTPUT_OBJECTS = test_output.o

test_output: $(TEST_OUTPUT_OBJECTS)
	$(CXX) $(CFLAGS) -o test_output $(TEST_OUTPUT_OBJECTS) -latomic

test_output.o: test_output.cpp
	$(CXX) -c $(CFLAGS) -o test_output.o test_output.cpp -I.

clean:
	rm -f *.o
	rm -f grader